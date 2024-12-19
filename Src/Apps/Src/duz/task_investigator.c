
#include "app.h"
#include "usb_uart_tx.h"
#include "investigator.h"
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

static task_signal_t investigatorTask;

extern const struct command_s known_subcommands_investigator;

#define INVESTIGATOR_TASK_STACK_SIZE_BYTES       2048
#define MAX_PRINT_FAST_INVESTIGATOR              6


error_e print_investigator_info(uint8_t *data, uint8_t size, uint8_t *ts, int16_t cfo, int rssi, int fsl)
{
    error_e ret = _ERR_Cannot_Alloc_Memory;

    uint16_t cnt;
    uint16_t hlen;
    int      cfo_pphm;
    char     *str;

    str = qmalloc(MAX_STR_SIZE);
    //size = MIN((sizeof(str) - 21) / 3, size); // 21 is an overhead
    if (str)
    {

        hlen = sprintf(str, "JS%04X", 0x5A5A); // reserve space for length of JS object
        sprintf(&str[strlen(str)], "{DATA:[");
        // Loop over the received data
        for (cnt = 0; cnt < size; cnt++)
        {
            sprintf(&str[strlen(str)], "%02X,", data[cnt]); // Add the byte and the delimiter - "XX,"
        }
        sprintf(&str[strlen(str) - 1], "], TS4ns:0x%02X%02X%02X%02X, ", ts[4], ts[3], ts[2], ts[1]);
        cfo_pphm = (int)((float)cfo * (CLOCK_OFFSET_PPM_TO_RATIO * 1e6 * 100));
        sprintf(&str[strlen(str)], "CFO:%d, ", cfo_pphm);
        sprintf(&str[strlen(str)], "rssi:%d.%02ddBm, fsl:%d.%02ddBm", rssi / 100, (rssi * -1) % 100, fsl / 100, (fsl * -1) % 100);
        sprintf(&str[strlen(str)], "%s", "\r\n");
        sprintf(&str[2], "%04X", strlen(str) - hlen);   // add formatted 4X of length, this will erase first '{'
        str[hlen] = '{';                                // restore the start bracket
        ret = copy_tx_msg((uint8_t *)str, strlen(str)); // do not notify flush task, only copy the message for print
        qfree(str);
    }

    return (ret);
}



static void InvestigatorTask(void *arg)
{
    (void)arg;
    int head, tail, size;
    investigator_info_t *pInvestigatorInfo;
    int signal_value;
    unsigned int lock;
    while (!(pInvestigatorInfo = getInvestigatorInfoPtr()))
    {
        qtime_msleep(5);
    }

    size = sizeof(pInvestigatorInfo->rxPcktBuf.buf) / sizeof(pInvestigatorInfo->rxPcktBuf.buf[0]);

    investigatorTask.Exit = 0;

    lock = qirq_lock();
    /* Start reception on the Listener. */
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
    qirq_unlock(lock);

    while (investigatorTask.Exit == 0)
    {
        /* ISR is delivering RxPckt via circ_buf & Signal.
         * This is the fastest method. */
        if (qsignal_wait(investigatorTask.signal, &signal_value, QOSAL_WAIT_FOREVER) != QERR_SUCCESS)
        {
            continue;
        }

        if (signal_value == STOP_TASK)
        {
            break;
        }

        lock = qirq_lock();
        head = pInvestigatorInfo->rxPcktBuf.head;
        tail = pInvestigatorInfo->rxPcktBuf.tail;
        qirq_unlock(lock);

        if (CIRC_CNT(head, tail, size) > 0)
        {
#if DEBUG_PRINT
            rx_investigator_pckt_t *pRx_investigator_Pckt = &pInvestigatorInfo->rxPcktBuf.buf[tail];
            print_investigator_info(pRx_investigator_Pckt->data,
                                     pRx_investigator_Pckt->rxDataLen,
                                     pRx_investigator_Pckt->timeStamp,
                                     pRx_investigator_Pckt->clock_offset,
                                     pRx_investigator_Pckt->rssi,
                                     pRx_investigator_Pckt->fsl);
#endif
            lock = qirq_lock();
            tail = (tail + 1) & (size - 1);
            pInvestigatorInfo->rxPcktBuf.tail = tail;
            qirq_unlock(lock);

            NotifyFlushTask();
        }

        qthread_yield();
    };
    investigatorTask.Exit = 2;
    while (investigatorTask.Exit == 2)
    {
        qtime_msleep(1);
    }
}

void investigator_task_notify(void)
{
    if (investigatorTask.thread) // RTOS : investigatorTask can be not started yet
    {
        // Sends the Signal to the application level via OS kernel.
        // This will add a small delay of few us, but
        // this method make sense from a program structure point of view.
        if (qsignal_raise(investigatorTask.signal, investigator_DATA) == 0x80000000)
        {
            error_handler(1, _ERR_Signal_Bad);
        }
    }
}

bool investigator_task_started(void)
{
    return investigatorTask.thread != NULL;
}

//-----------------------------------------------------------------------------

static void investigator_setup_tasks(void)
{
    investigatorTask.signal = qsignal_init();
    if (!investigatorTask.signal)
    {
        error_handler(1, _ERR_Create_Task_Bad);
    }

    /* Create Data Transfer task. */
    size_t task_size = INVESTIGATOR_TASK_STACK_SIZE_BYTES;
    investigatorTask.task_stack = qmalloc(task_size);

    investigatorTask.thread = qthread_create(InvestigatorTask, NULL, "Investigator", investigatorTask.task_stack, INVESTIGATOR_TASK_STACK_SIZE_BYTES, QTHREAD_PRIORITY_HIGH);
    if (!investigatorTask.thread)
    {
        error_handler(1, _ERR_Create_Task_Bad);
    }
}


void investigator_terminate(void)
{
    /* Need to switch off UWB chip's RX and IRQ before killing tasks. */
    hal_uwb.stop_all_uwb();

    terminate_task(&investigatorTask);
    investigator_process_terminate();

    hal_uwb.sleep_enter();
}

void investigator_starter(void const *argument)
{
    error_e tmp;

    /* Not used. */
    (void)argument;
    /* "RTOS-independent" part : initialization of two-way ranging process */
    tmp = investigator_process_init();

    if (tmp != _NO_ERR)
    {
        error_handler(1, tmp);
    }
    /* "RTOS-based" : setup (not start) all necessary tasks for the Node operation. */
    investigator_setup_tasks();
    /* IRQ is enabled from MASTER chip and it may receive UWB immediately after this point. */
    investigator_process_start();
}


static const struct subcommand_group_s investigator_subcommands = {"INVESTIGATOR Options", &known_subcommands_investigator, 1};

const app_definition_t helpers_app_investigator[] __attribute__((section(".known_apps"))) = {
    {"I", mAPP, investigator_starter, investigator_terminate, waitForCommand, command_parser, &investigator_subcommands}};