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
#include <math.h>
#include "utils.h"
#include "qirq.h"


// ----------------------------------------------------------------------------

#define NODE_MALLOC qmalloc
#define NODE_FREE   qfree

#define RX_CODE_THRESHOLD  8      // For 64 MHz PRF the RX code is 9.
#define ALPHA_PRF_16       113.8  // Constant A for PRF of 16 MHz. See User Manual for more information.
#define ALPHA_PRF_64       120.7  // Constant A for PRF of 64 MHz. See User Manual for more information.
#define LOG_CONSTANT_C0    63.2   // 10log10(2^21) = 63.2    // See User Manual for more information.
#define LOG_CONSTANT_D0_E0 51.175 // 10log10(2^17) = 51.175  // See User Manual for more information.


static uint8_t tx_msg[] = { 0xC5, 0, 'D', 'E', 'C', 'A', 'W', 'A', 'V', 'E' };
#define FRAME_LENGTH (sizeof(tx_msg) + FCS_LEN) // The real length that is going to be transmitted


void responder_rssi_cal(int *rssi, int *fsl)
{
    dwt_nlos_alldiag_t all_diag;
    uint8_t D;
    dwt_config_t *dwt_config = get_dwt_config();

    // All float variables used for recording different diagnostic results and probability.
    float ip_alpha, log_constant = 0;
    float ip_f1, ip_f2, ip_f3, ip_n, ip_cp, ip_rsl, ip_fsl;

    uint32_t dev_id = dwt_readdevid();

    if ((dev_id == (uint32_t)DWT_DW3000_DEV_ID) || (dev_id == (uint32_t)DWT_DW3000_PDOA_DEV_ID))
    {
        log_constant = LOG_CONSTANT_C0;
    }
    else
    {
        log_constant = LOG_CONSTANT_D0_E0;
    }

    // Select IPATOV to read Ipatov diagnostic registers from API function dwt_nlos_alldiag()
    all_diag.diag_type = IPATOV;
    dwt_nlos_alldiag(&all_diag);
    ip_alpha = (dwt_config->rxCode > RX_CODE_THRESHOLD) ? (-(ALPHA_PRF_64 + 1)) : -(ALPHA_PRF_16);
    // ip_alpha = (config->rxCode > RX_CODE_THRESHOLD) ? (-(ALPHA_PRF_64 + 1)) : -(ALPHA_PRF_16);
    ip_n = all_diag.accumCount; // The number of preamble symbols accumulated
    ip_f1 = all_diag.F1 / 4;    // The First Path Amplitude (point 1) magnitude value (it has 2 fractional bits),
    ip_f2 = all_diag.F2 / 4;    // The First Path Amplitude (point 2) magnitude value (it has 2 fractional bits),
    ip_f3 = all_diag.F3 / 4;    // The First Path Amplitude (point 3) magnitude value (it has 2 fractional bits),
    ip_cp = all_diag.cir_power;

    D = all_diag.D * 6;

    // For IPATOV
    ip_n *= ip_n;
    ip_f1 *= ip_f1;
    ip_f2 *= ip_f2;
    ip_f3 *= ip_f3;

    // For the CIR Ipatov.
    ip_rsl = 10 * log10((float)ip_cp / ip_n) + ip_alpha + log_constant + D;
    ip_fsl = 10 * log10(((ip_f1 + ip_f2 + ip_f3) / ip_n)) + ip_alpha + D;
    if ((ip_rsl < -120) || (ip_rsl > 0))
    {
        ip_rsl = 0;
    }
    if ((ip_fsl < -120) || (ip_fsl > 0))
    {
        ip_fsl = 0;
    }
    *rssi = (int)(ip_rsl * 100);
    *fsl = (int)(ip_fsl * 100);
}

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
    dwt_setrxtimeout(0);      /**< no any delays set by default : part of config of receiver on Tx sending */
    dwt_configureframefilter(DWT_FF_DISABLE, 0);
    dwt_writetxfctrl(FRAME_LENGTH, 0, 0); /* Zero offset in TX buffer, no ranging. */
    dwt_configure_rf_port(DWT_RF_PORT_MANUAL_1); // Configure PORT for RXTX
}


void responder_configure_uwb(dwt_cb_t cbRxOk, dwt_cb_t cbRxTo, dwt_cb_t cbRxErr)
{
    dwt_callbacks_s cbs = {0};
    dwt_setlnapamode(DWT_PA_ENABLE | DWT_LNA_ENABLE);   /* Configure TX/RX states to output on GPIOs */
    dwt_setsniffmode(1, 8, 128);                        /* Configure SNIFF mode. */

    cbs.cbRxOk = cbRxOk;
    cbs.cbRxTo = cbRxTo;
    cbs.cbRxErr = cbRxErr;
    dwt_setcallbacks(&cbs);

    dwt_setinterrupt(DWT_INT_TXFRS_BIT_MASK | DWT_INT_RXFCG_BIT_MASK | (DWT_INT_ARFE_BIT_MASK | DWT_INT_RXFSL_BIT_MASK | DWT_INT_RXSTO_BIT_MASK | DWT_INT_RXPHE_BIT_MASK | DWT_INT_RXFCE_BIT_MASK | DWT_INT_RXFTO_BIT_MASK), 0, 2);
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
        responder_rssi_cal(&p->rssi, &p->fsl);

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


void responder_blink(bool last)
{
      dwt_writetxdata(FRAME_LENGTH - FCS_LEN, tx_msg, 0); /* Zero offset in TX buffer. */
      dwt_starttx(last ? DWT_START_TX_IMMEDIATE | DWT_RESPONSE_EXPECTED : DWT_START_TX_IMMEDIATE);
      /* Poll DW IC until TX frame sent event set*/
      waitforsysstatus(NULL, NULL, DWT_INT_TXFRS_BIT_MASK, 0);
      /* Clear TX frame sent event. */
      dwt_writesysstatuslo(DWT_INT_TXFRS_BIT_MASK);
}

void start_responder_tx()
{
    // Start LED to indicate TX
    dwt_setleds(DWT_LEDS_ENABLE | DWT_LEDS_INIT_BLINK); 
    for (int i=0; i<RESPONDER_BLINK_COUNT-1; i++)
    {
        responder_blink(false);
        deca_sleep(RESPONDER_BLINK_INTERVAL_MS);
    }
    // In last transmission, we start the RX back: Will enable the receiver after TX has complete */
    responder_blink(true);
    // Stop LED
    dwt_setleds(DWT_LEDS_DISABLE);
}

void rx_responder_cb(const dwt_cb_data_t *rxd)
{

    // -------------------------- -----------------------
    // ----------  Parse RX -----------------------------
    // -------------------------- -----------------------
    parse_responder_rx(rxd);

    // --------------------------------------------------
    // ----------  Start TX ----------------------------- 
    // --------------------------------------------------
    start_responder_tx();

    // re-enable receiver again - no timeout
    /* ready to serve next raw reception */
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
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
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


void responder_process_start(void)
{
    diag_printf("Responder: Started\r\n"); 
    hal_uwb.enableIRQ();
}


void responder_process_terminate(void)
{
    // stop the RTC timer
    /* configure the RTC Wakeup timer with a high priority;
     * this timer is saving global Super Frame Timestamp,
     * so we want this timestamp as stable as we can.
     *
     * */
    
    diag_printf("Responder: Stopped\r\n"); 
    Rtc.disableIRQ();
    Rtc.setPriorityIRQ();

    if (pResponderInfo)
    {
        NODE_FREE(pResponderInfo);
        pResponderInfo = NULL;
    }
}

//-----------------------------------------------------------------------------
