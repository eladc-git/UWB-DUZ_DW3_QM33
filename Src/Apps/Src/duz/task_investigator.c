
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
#include "utils.h"
#include "qmalloc.h"

static task_signal_t investigatorTask;
extern const struct command_s known_subcommands_investigator;
extern bool investigator_calib_mode;

#define INVESTIGATOR_TASK_STACK_SIZE_BYTES       2048
#define MAX_PRINT_FAST_INVESTIGATOR              6

error_e print_investigator_info(uint32_t id, uint32_t seq_count, uint8_t *ts, int16_t cfo, int rssi, int fsl, int pdoa1, int pdoa2, float azimut, float elevation)
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
        sprintf(&str[strlen(str)], "rssi:%d.%02ddBm, fsl:%d.%02ddBm, ", rssi / 100, (rssi * -1) % 100, fsl / 100, (fsl * -1) % 100);
        sprintf(&str[strlen(str)], "pdoa1:%d, pdoa2:%d, ", pdoa1, pdoa2);
        sprintf(&str[strlen(str)], "azimut:%.1f, elevation:%.1f", azimut, elevation);
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
 
    if (investigator_calib_mode)
    {
        // Calibration Mode
        diag_printf("Investigator: Calibration (Only RX)\r\n"); 
    }
    else
    {
        // Trigger push button 
        nrf_gpio_pin_pull_t pull_config = NRF_GPIO_PIN_PULLUP;
        nrf_gpio_cfg_input(INVESTIGATOR_PUSH_BUTTON_PIN_NUM, pull_config);
    }

    while (investigatorTask.Exit == 0)
    {
         
        if (!investigator_calib_mode)
        {
            diag_printf("Investigator: Waiting for trigger\r\n");
            /* Wait for trigger from push button*/
            uint32_t pushbutton = nrf_gpio_pin_read(INVESTIGATOR_PUSH_BUTTON_PIN_NUM);
            while(pushbutton) // 0 is pushed
            {
                qtime_msleep_yield(20);
                pushbutton = nrf_gpio_pin_read(INVESTIGATOR_PUSH_BUTTON_PIN_NUM);
            }

            /* Start transmitting blinks for initiation and then go to reception for INVESTIGATOR_RECEIVER_TIME_MS [ms] */
            diag_printf("Investigator: Start initiation for %dms\r\n", (INVESTIGATOR_BLINK_COUNT-1)*INVESTIGATOR_BLINK_INTERVAL_MS); 
            start_investigator_tx();    
            
            /* Start reception on the Responder for RESPONDER_RECEIVER_ON_MS [ms]. */
            diag_printf("Investigator: Start reception for %dms\r\n", INVESTIGATOR_RECEIVER_TIME_MS); 
        }

        lock = qirq_lock();
        dwt_rxenable(DWT_START_RX_IMMEDIATE);
        qirq_unlock(lock);

        int64_t start_time_ms = qtime_get_uptime_us()/1000;
        int64_t current_time_ms = start_time_ms;
        int64_t stop_time_ms = start_time_ms + INVESTIGATOR_RECEIVER_TIME_MS;

        while(current_time_ms < stop_time_ms)
        {
            /* ISR is delivering RxPckt via circ_buf & Signal */
            if (qsignal_wait(investigatorTask.signal, &signal_value, stop_time_ms-start_time_ms) != QERR_SUCCESS)
            {
                break;
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
                print_investigator_info(pRx_investigator_Pckt->id,
                                        pRx_investigator_Pckt->seq_count,
                                        pRx_investigator_Pckt->timeStamp,
                                        pRx_investigator_Pckt->clock_offset,
                                        pRx_investigator_Pckt->rssi,
                                        pRx_investigator_Pckt->fsl,
                                        pRx_investigator_Pckt->pdoa1,
                                        pRx_investigator_Pckt->pdoa2,
                                        pRx_investigator_Pckt->azimut,
                                        pRx_investigator_Pckt->elevation);
    #endif
                lock = qirq_lock();
                tail = (tail + 1) & (size - 1);
                pInvestigatorInfo->rxPcktBuf.tail = tail;
                qirq_unlock(lock);
                NotifyFlushTask();
            }
            current_time_ms = qtime_get_uptime_us()/1000;
        }
        dwt_forcetrxoff(); // Stop RXTX   
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
        if (qsignal_raise(investigatorTask.signal, 2) == 0x80000000)
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
    diag_printf("investigator: Stopped\r\n"); 

    /* Need to switch off UWB chip's RX and IRQ before killing tasks. */
    hal_uwb.stop_all_uwb();

    terminate_task(&investigatorTask);
    investigator_process_terminate();

    hal_uwb.sleep_enter();
}

void investigator_starter(void const *argument)
{
    error_e tmp;

    diag_printf("investigator: Started\r\n");

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
    /* IRQ is enabled from MASTER chip. */; 
    hal_uwb.enableIRQ();
}


static const struct subcommand_group_s investigator_subcommands = {"INVESTIGATOR Options", &known_subcommands_investigator, 1};

const app_definition_t helpers_app_investigator[] __attribute__((section(".known_apps"))) = {
    {"I", mAPP, investigator_starter, investigator_terminate, waitForCommand, command_parser, &investigator_subcommands}};