#include "app.h"
#include "usb_uart_tx.h"
#include "responder.h"
#include "task_signal.h"
#include "HAL_error.h"
#include "HAL_uwb.h"
#include "circular_buffers.h"
#include "usb_uart_tx.h"
#include "cmd_fn.h"
#include "flushTask.h"
#include "cmd.h"
#include "int_priority.h"
#include "qirq.h"
#include "deca_dbg.h"
#include "duz_app_config.h"
#include "qmalloc.h"


static task_signal_t responderTask;

extern const struct command_s known_subcommands_responder;
extern bool responder_calib_mode;

#define RESPONDER_TASK_STACK_SIZE_BYTES       2048
#define MAX_PRINT_FAST_RESPONDER              6


error_e print_responder_info(uint32_t id, uint32_t seq_count, uint8_t* ts, int16_t cfo, int rssi, int fsl)
{
    error_e ret = _ERR_Cannot_Alloc_Memory;
    uint16_t hlen;
    char     *str;

    str = qmalloc(MAX_STR_SIZE);
    if (str)
    {

        hlen = sprintf(str, "JS%04X", 0x5A5A); // reserve space for length of JS object
        sprintf(&str[strlen(str)], "{");
        sprintf(&str[strlen(str)], "ID:0x%08lX, ", id);
        sprintf(&str[strlen(str)], "SEQ:%lu, ", seq_count);
        sprintf(&str[strlen(str)], "TS4ns:0x%02X%02X%02X%02X, ", ts[4], ts[3], ts[2], ts[1]);
        sprintf(&str[strlen(str)], "CFO:%d, ", (int)((float)cfo * (CLOCK_OFFSET_PPM_TO_RATIO * 1e6 * 100)));
        sprintf(&str[strlen(str)], "rssi:%d.%02ddBm, fsl:%d.%02ddBm", rssi / 100, (rssi * -1) % 100, fsl / 100, (fsl * -1) % 100);
        sprintf(&str[strlen(str)], "%s", "\r\n");
        sprintf(&str[2], "%04X", strlen(str) - hlen);   // add formatted 4X of length, this will erase first '{'
        str[hlen] = '{';                                // restore the start bracket
        ret = copy_tx_msg((uint8_t *)str, strlen(str)); // do not notify flush task, only copy the message for print
        qfree(str);
    }

    return (ret);
}



static void ResponderTask(void *arg)
{
    (void)arg;
    int head, tail, size;
    responder_info_t *pResponderInfo;
    int signal_value;
    unsigned int lock;
    while (!(pResponderInfo = getResponderInfoPtr()))
    {
        qtime_msleep(5);
    }

    size = sizeof(pResponderInfo->rxPcktBuf.buf) / sizeof(pResponderInfo->rxPcktBuf.buf[0]);

    responderTask.Exit = 0;

    if (responder_calib_mode)
    {
        // Calibration Mode
        diag_printf("Responder: Calibration (Only TX)\r\n"); 
    }

    while (responderTask.Exit == 0)
    {

        if (responder_calib_mode)
        {
            start_responder_tx();
            qtime_msleep_yield(10);
            continue;
        }

        /* Start reception on the Responder for RESPONDER_RECEIVER_ON_MS [ms]. */
        lock = qirq_lock();
        dwt_rxenable(DWT_START_RX_IMMEDIATE);
        dwt_setrxtimeout(1000*RESPONDER_RECEIVER_ON_MS);
        qirq_unlock(lock);

        /* ISR is delivering RxPckt via circ_buf & Signal */
        if (qsignal_wait(responderTask.signal, &signal_value, RESPONDER_RECEIVER_ON_MS) != QERR_SUCCESS)
        {
            qtime_msleep_yield(RESPONDER_RECEIVER_OFF_MS);
            continue;
        }

        if (signal_value == STOP_TASK)
        {
            break;
        }

        lock = qirq_lock();
        head = pResponderInfo->rxPcktBuf.head;
        tail = pResponderInfo->rxPcktBuf.tail;
        qirq_unlock(lock);

        if (CIRC_CNT(head, tail, size) > 0)
        {
#if DEBUG_PRINT
            rx_responder_pckt_t *pRx_responder_Pckt = &pResponderInfo->rxPcktBuf.buf[tail];
            print_responder_info(pRx_responder_Pckt->id,
                                pRx_responder_Pckt->seq_count,
                                pRx_responder_Pckt->timeStamp,
                                pRx_responder_Pckt->clock_offset,
                                pRx_responder_Pckt->rssi,
                                pRx_responder_Pckt->fsl);
#endif
            lock = qirq_lock();
            tail = (tail + 1) & (size - 1);
            pResponderInfo->rxPcktBuf.tail = tail;
            qirq_unlock(lock);
            NotifyFlushTask();
        }
        dwt_forcetrxoff(); // Stop RXTX
        qtime_msleep_yield(RESPONDER_RECEIVER_OFF_MS);
    };
    responderTask.Exit = 2;
    while (responderTask.Exit == 2)
    {
        qtime_msleep(1);
    }
}

void responder_task_notify(void)
{
    if (responderTask.thread) // RTOS : responderTask can be not started yet
    {
        // Sends the Signal to the application level via OS kernel.
        // This will add a small delay of few us, but
        // this method make sense from a program structure point of view.
        if (qsignal_raise(responderTask.signal, 2) == 0x80000000)
        {
            error_handler(1, _ERR_Signal_Bad);
        }
    }
}

bool responder_task_started(void)
{
    return responderTask.thread != NULL;
}

//-----------------------------------------------------------------------------

static void responder_setup_tasks(void)
{
    responderTask.signal = qsignal_init();
    if (!responderTask.signal)
    {
        error_handler(1, _ERR_Create_Task_Bad);
    }

    /* Create Data Transfer task. */
    size_t task_size = RESPONDER_TASK_STACK_SIZE_BYTES;
    responderTask.task_stack = qmalloc(task_size);

    responderTask.thread = qthread_create(ResponderTask, NULL, "Responder", responderTask.task_stack, RESPONDER_TASK_STACK_SIZE_BYTES, QTHREAD_PRIORITY_HIGH);
    if (!responderTask.thread)
    {
        error_handler(1, _ERR_Create_Task_Bad);
    }
}


void responder_terminate(void)
{
    diag_printf("Responder: Stopped\r\n"); 
    /* Need to switch off UWB chip's RX and IRQ before killing tasks. */
    hal_uwb.stop_all_uwb();

    terminate_task(&responderTask);
    responder_process_terminate();

    hal_uwb.sleep_enter();
}


void responder_starter(void const *argument)
{
    error_e tmp;

    diag_printf("Responder: Started\r\n"); 

    /* Not used. */
    (void)argument;
    /* "RTOS-independent" part : initialization of two-way ranging process */
    tmp = responder_process_init();

    if (tmp != _NO_ERR)
    {
        error_handler(1, tmp);
    }

    /* "RTOS-based" : setup (not start) all necessary tasks for the Node operation. */
    responder_setup_tasks();
    /* IRQ is enabled from MASTER chip and it may receive UWB immediately after this point. */
    hal_uwb.enableIRQ();
}


static const struct subcommand_group_s responder_subcommands = {"RESPONDER Options", &known_subcommands_responder, 1};

const app_definition_t helpers_app_responder[] __attribute__((section(".known_apps"))) = {
    {"R", mAPP, responder_starter, responder_terminate, waitForCommand, command_parser, &responder_subcommands}};
