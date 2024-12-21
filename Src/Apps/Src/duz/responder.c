#include <stdlib.h>
#include "duz_app_config.h"
#include "responder.h"
#include "deca_device_api.h"
#include "deca_dbg.h"
#include "circular_buffers.h"
#include "HAL_error.h"
#include "HAL_rtc.h"
#include "HAL_uwb.h"
#include "task_responder.h"
#include "driver_app_config.h"
#include <stdint.h>
#include "utils.h"
#include "qirq.h"


bool responder_calib_mode = 0;
uint32_t responder_seq_counter = 0;

static uint8_t tx_msg[] = 
{ 
    0xBB, 0xBB, // Responder indicator
    0xFF&(RESPONDER_ID>>24), 0xFF&(RESPONDER_ID>>24), 0xFF&(RESPONDER_ID>>24), 0xFF&(RESPONDER_ID), // ID
    0x00, 0x00, 0x00, 0x00,  // blink counter
 };

#define FRAME_LENGTH (sizeof(tx_msg) + FCS_LEN) // The real length that is going to be transmitted


void responder_readrxtimestamp(uint8_t *timestamp)
{
    dwt_readrxtimestamp(timestamp, DWT_COMPAT_NONE);
}

int responder_readstsquality(int16_t *rxStsQualityIndex)
{
    return dwt_readstsquality(rxStsQualityIndex, 0);
}


//-----------------------------------------------------------------------------
// The psresponderInfo structure holds all responder's process parameters
static responder_info_t *pResponderInfo = NULL;
responder_info_t *getResponderInfoPtr(void)
{
    return (pResponderInfo);
}

static void rxtx_responder_configure(dwt_config_t *pdwCfg, uint16_t frameFilter)
{

    if (dwt_configure(pdwCfg)) /**< Configure the Physical Channel parameters (PLEN, PRF, etc) */
    {
        error_handler(1, _ERR_INIT);
    }
    dwt_setrxaftertxdelay(0); /**< no any delays set by default : part of config of receiver on Tx sending */
    dwt_configureframefilter(DWT_FF_DISABLE, 0);
    dwt_writetxfctrl(FRAME_LENGTH, 0, 0); /* Zero offset in TX buffer, no ranging. */
    dwt_configure_rf_port(DWT_RF_PORT_MANUAL_1); // Configure PORT for RXTX

     /* Configure the TX spectrum parameters (power, PG delay and PG count) */
    dwt_txconfig_t dwt_txconfig = 
    {
        0x27,       /* PG delay. */
        0xffffffff, /* TX power. */
        0x0         /*PG count*/
    };
    dwt_configuretxrf(&dwt_txconfig);
}


void responder_configure_uwb(dwt_cb_t cbRxOk, dwt_cb_t cbRxTo, dwt_cb_t cbRxErr)
{
    dwt_callbacks_s cbs = {0};
    dwt_setlnapamode(DWT_PA_ENABLE | DWT_LNA_ENABLE);   /* Configure TX/RX states to output on GPIOs */

    cbs.cbRxOk = cbRxOk;
    cbs.cbRxTo = cbRxTo;
    cbs.cbRxErr = cbRxErr;
    dwt_setcallbacks(&cbs);

    dwt_setinterrupt(DWT_INT_TXFRS_BIT_MASK | DWT_INT_RXFCG_BIT_MASK | (DWT_INT_ARFE_BIT_MASK | DWT_INT_RXFSL_BIT_MASK |
                     DWT_INT_RXSTO_BIT_MASK | DWT_INT_RXPHE_BIT_MASK | DWT_INT_RXFCE_BIT_MASK | DWT_INT_RXFTO_BIT_MASK), 0, 2);   
    dwt_configciadiag(DW_CIA_DIAG_LOG_ALL);
    dwt_configeventcounters(1);

    dwt_app_config_t *dwt_app_config = get_app_dwt_config();
    dwt_setxtaltrim(dwt_app_config->xtal_trim);
}


//-----------------------------------------------------------------------------

void parse_responder_rx(const dwt_cb_data_t *rxd)
{
    responder_info_t *pResponderInfo = getResponderInfoPtr();

    if (!pResponderInfo)
    {
        return;
    }

    const int size = sizeof(pResponderInfo->rxPcktBuf.buf) / sizeof(pResponderInfo->rxPcktBuf.buf[0]);
    int head = pResponderInfo->rxPcktBuf.head;
    int tail = pResponderInfo->rxPcktBuf.tail;
    if (CIRC_SPACE(head, tail, size) > 0)
    {
        rx_responder_pckt_t *p = &pResponderInfo->rxPcktBuf.buf[head];

        responder_readrxtimestamp(p->timeStamp); // Raw Rx TimeStamp (STS or IPATOV based on STS config)

        p->clock_offset = dwt_readclockoffset(); // Reading Clock offset for any Rx packets
        p->status = rxd->status;

        pResponderInfo->event_counts_sfd_detect++;

        p->rxDataLen = MIN(rxd->datalength, sizeof(p->data));
        dwt_readrxdata((uint8_t *)&p->data, p->rxDataLen, 0); // Raw message
        rssi_cal(&p->rssi, &p->fsl);
        p->id = *(uint32_t*)(&p->data[2]); // ID
        p->seq_count = *(uint32_t*)(&p->data[6]); // Sequence count

        if (responder_task_started()) // RTOS : responderTask can be not started yet
        {
            head = (head + 1) & (size - 1);
            pResponderInfo->rxPcktBuf.head = head; // ISR level : do not need to protect
        }
    }

    responder_task_notify();

    sts_config_t *sts_config = get_sts_config();
    if (sts_config->stsInteropMode) // value 0 = dynamic STS, 1 = fixed STS)
    {
        // re-load the initial cp_iv value to keep STS the same for each frame
        dwt_configurestsloadiv();
    }

    dwt_readeventcounters(&pResponderInfo->event_counts); // take a snapshot of event counters
}


void responder_blink()
{
      dwt_writetxdata(FRAME_LENGTH - FCS_LEN, tx_msg, 0); /* Zero offset in TX buffer. */
      dwt_starttx(DWT_START_TX_IMMEDIATE);
}

void start_responder_tx()
{
    // Start LED to indicate TX
    dwt_setleds(DWT_LEDS_ENABLE | DWT_LEDS_INIT_BLINK); 
    for (int i=0; i<RESPONDER_BLINK_COUNT; i++)
    {
        // First wait then blink
        deca_sleep(RESPONDER_BLINK_INTERVAL_MS);
        responder_blink();
        // Increment counter
        *(uint32_t*)&tx_msg[6] = responder_seq_counter++;
    }
    // Stop LED
    dwt_setleds(DWT_LEDS_DISABLE);
}

void rx_responder_cb(const dwt_cb_data_t *rxd)
{
    // ----------  Parse RX ------------/
    parse_responder_rx(rxd);
}


void responder_timeout_cb(const dwt_cb_data_t *rxd)
{
    responder_info_t *presponderInfo = getResponderInfoPtr();
    sts_config_t *sts_config = get_sts_config();
    if (sts_config->stsInteropMode) // value 0 = dynamic STS, 1 = fixed STS)
    {
        // re-load the initial cp_iv value to keep STS the same for each frame
        dwt_configurestsloadiv();
    }
    dwt_readeventcounters(&presponderInfo->event_counts);
}

void responder_error_cb(const dwt_cb_data_t *rxd)
{
    responder_timeout_cb(rxd);
}


//-----------------------------------------------------------------------------

error_e responder_process_init()
{
    hal_uwb.init();
    hal_uwb.irq_init();
    hal_uwb.disable_irq_and_reset(1);
    assert(hal_uwb.probe() == DWT_SUCCESS);

    if (!pResponderInfo)
    {
        pResponderInfo = NODE_MALLOC(sizeof(responder_info_t));
    }

    responder_info_t *pResponderInfo = getResponderInfoPtr();

    if (!pResponderInfo)
    {
        return (_ERR_Cannot_Alloc_NodeMemory);
    }

    /* Switch off receiver's rxTimeOut, RxAfterTxDelay, delayedRxTime,
     * autoRxEnable, dblBufferMode and autoACK,
     * clear all initial counters, etc. */
    memset(pResponderInfo, 0, sizeof(responder_info_t));
    memset(&pResponderInfo->event_counts, 0, sizeof(pResponderInfo->event_counts));
    pResponderInfo->event_counts_sfd_detect = 0;

    /* dwt_xx calls in app level Must be in protected mode (DW3000 IRQ disabled). */
    hal_uwb.disableIRQ();
    unsigned int lock = qirq_lock();

    /* Set callbacks to NULL inside dwt_initialise. */
    if (dwt_initialise(0) != DWT_SUCCESS)
    {
        return (_ERR_INIT);
    }

    /* Configure SPI to fast rate */
    if (hal_uwb.uwbs != NULL)
    {
        hal_uwb.uwbs->spi->fast_rate(hal_uwb.uwbs->spi->handler);
    }

    /* Configure DW IC's UWB mode, no frame filtering for responder. */
    rxtx_responder_configure(get_dwt_config(), DWT_FF_DISABLE);
    responder_configure_uwb(rx_responder_cb, responder_timeout_cb, responder_error_cb);

    /* End configuration of DW IC */
    {
        /* Configure the RTC Wakeup timer with a high priority;
         * this timer is saving global Super Frame Timestamp,
         * so we want this timestamp as stable as we can. */
        Rtc.disableIRQ();
        Rtc.setPriorityIRQ();
    }

    qirq_unlock(lock);

    return (_NO_ERR);
}


void responder_process_terminate(void)
{
    // stop the RTC timer
    /* configure the RTC Wakeup timer with a high priority;
     * this timer is saving global Super Frame Timestamp,
     * so we want this timestamp as stable as we can.
     *
     * */
    
    Rtc.disableIRQ();
    Rtc.setPriorityIRQ();

    if (pResponderInfo)
    {
        NODE_FREE(pResponderInfo);
        pResponderInfo = NULL;
    }
}

//-----------------------------------------------------------------------------
