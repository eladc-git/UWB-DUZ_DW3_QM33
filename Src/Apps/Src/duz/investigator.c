#include <stdlib.h>

#include "investigator.h"
#include "deca_device_api.h"
#include "deca_dbg.h"
#include "circular_buffers.h"
#include "HAL_error.h"
#include "HAL_rtc.h"
#include "HAL_uwb.h"
#include "task_investigator.h"
#include "driver_app_config.h"
#include <stdint.h>
#include "utils.h"
#include "qirq.h"
#include "duz_app_config.h"


bool investigator_calib_mode = 0;
uint32_t investigator_seq_counter = 0;
int16_t calib_val = PDOA1_CALIB_VAL;


static uint8_t tx_msg[] = 
{ 
    0xAA, 0xAA, // Investigator indicator
    0xFF&(INVESTIGATOR_ID>>24), 0xFF&(INVESTIGATOR_ID>>24), 0xFF&(INVESTIGATOR_ID>>24), 0xFF&(INVESTIGATOR_ID), // ID
    0x00, 0x00, 0x00, 0x00, // blink counter
 };

#define FRAME_LENGTH (sizeof(tx_msg) + FCS_LEN) // The real length that is going to be transmitted


void investigator_readrxtimestamp(uint8_t *timestamp)
{
    dwt_readrxtimestamp(timestamp, DWT_COMPAT_NONE);
}

int investigator_readstsquality(int16_t *rxStsQualityIndex)
{
    return dwt_readstsquality(rxStsQualityIndex, 0);
}


//-----------------------------------------------------------------------------
// The psinvestigatorInfo structure holds all investigator's process parameters
static investigator_info_t *pInvestigatorInfo = NULL;
investigator_info_t *getInvestigatorInfoPtr(void)
{
    return (pInvestigatorInfo);
}


static void rxtx_investigator_configure(dwt_config_t *pdwCfg, uint16_t frameFiltery)
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


void investigator_configure_uwb(dwt_cb_t cbRxOk, dwt_cb_t cbRxTo, dwt_cb_t cbRxErr)
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

void parse_investigator_rx(const dwt_cb_data_t *rxd)
{
    investigator_info_t *pinvestigatorInfo = getInvestigatorInfoPtr();

    if (!pinvestigatorInfo)
    {
        return;
    }

    const int size = sizeof(pinvestigatorInfo->rxPcktBuf.buf) / sizeof(pinvestigatorInfo->rxPcktBuf.buf[0]);
    int head = pinvestigatorInfo->rxPcktBuf.head;
    int tail = pinvestigatorInfo->rxPcktBuf.tail;
    if (CIRC_SPACE(head, tail, size) > 0)
    {
        rx_investigator_pckt_t *p = &pinvestigatorInfo->rxPcktBuf.buf[head];

        investigator_readrxtimestamp(p->timeStamp); // Raw Rx TimeStamp (STS or IPATOV based on STS config)

        p->clock_offset = dwt_readclockoffset(); // Reading Clock offset for any Rx packets
        p->status = rxd->status;

        pinvestigatorInfo->event_counts_sfd_detect++;

        p->rxDataLen = MIN(rxd->datalength, sizeof(p->data));
        dwt_readrxdata((uint8_t *)&p->data, p->rxDataLen, 0); // Raw message
        rssi_cal(&p->rssi, &p->fsl); // Calc rssi
        p->id = *(uint32_t*)(&p->data[2]); // ID
        p->seq_count = *(uint32_t*)(&p->data[6]); // Sequence count
        p->pdoa1 = dwt_readpdoa(); // Pdoa
        p->azimut = pdoa2degree(p->pdoa1, calib_val);

        if (investigator_task_started()) // RTOS : investigatorTask can be not started yet
        {
            head = (head + 1) & (size - 1);
            pinvestigatorInfo->rxPcktBuf.head = head; // ISR level : do not need to protect
        }

    }

    investigator_task_notify();

    sts_config_t *sts_config = get_sts_config();
    if (sts_config->stsInteropMode) // value 0 = dynamic STS, 1 = fixed STS)
    {
        // re-load the initial cp_iv value to keep STS the same for each frame
        dwt_configurestsloadiv();
    }

    /* Take a snapshot of event counters. */
    dwt_readeventcounters(&pinvestigatorInfo->event_counts);

    /* ready to serve next raw reception */
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
}


void investigator_blink()
{
      dwt_writetxdata(FRAME_LENGTH - FCS_LEN, tx_msg, 0); /* Zero offset in TX buffer. */
      dwt_starttx(DWT_START_TX_IMMEDIATE);
}


void start_investigator_tx()
{
    // Start LED to indicate TX
    dwt_setleds(DWT_LEDS_ENABLE | DWT_LEDS_INIT_BLINK); 
    for (int i=0; i<INVESTIGATOR_BLINK_COUNT; i++)
    {
        // First blink then wait
        investigator_blink();
        deca_sleep(INVESTIGATOR_BLINK_INTERVAL_MS);
    }
    // Stop LED
    dwt_setleds(DWT_LEDS_DISABLE);
    // Increment counter
    *(uint32_t*)&tx_msg[6] = investigator_seq_counter++; 
}

void rx_investigator_cb(const dwt_cb_data_t *rxd)
{
    // -------------------------- -----------------------
    // ----------  Parse RX -----------------------------
    // -------------------------- -----------------------
    parse_investigator_rx(rxd);
}


void investigator_timeout_cb(const dwt_cb_data_t *rxd)
{
    investigator_info_t *pinvestigatorInfo = getInvestigatorInfoPtr();
    sts_config_t *sts_config = get_sts_config();
    if (sts_config->stsInteropMode) // value 0 = dynamic STS, 1 = fixed STS)
    {
        // re-load the initial cp_iv value to keep STS the same for each frame
        dwt_configurestsloadiv();
    }
    dwt_readeventcounters(&pinvestigatorInfo->event_counts);
    /* ready to serve next raw reception */
    dwt_rxenable(DWT_START_RX_IMMEDIATE);
}

void investigator_error_cb(const dwt_cb_data_t *rxd)
{
    investigator_timeout_cb(rxd);
}


//-----------------------------------------------------------------------------

error_e investigator_process_init()
{
    hal_uwb.init();
    hal_uwb.irq_init();
    hal_uwb.disable_irq_and_reset(1);
    assert(hal_uwb.probe() == DWT_SUCCESS);
    
    if (!pInvestigatorInfo)
    {
        pInvestigatorInfo = NODE_MALLOC(sizeof(investigator_info_t));
    }

    investigator_info_t *pInvestigatorInfo = getInvestigatorInfoPtr();

    if (!pInvestigatorInfo)
    {
        return (_ERR_Cannot_Alloc_NodeMemory);
    }

    /* switch off receiver's rxTimeOut, RxAfterTxDelay, delayedRxTime,
     * autoRxEnable, dblBufferMode and autoACK,
     * clear all initial counters, etc.
     * */

    memset(pInvestigatorInfo, 0, sizeof(investigator_info_t));
    memset(&pInvestigatorInfo->event_counts, 0, sizeof(pInvestigatorInfo->event_counts));
    pInvestigatorInfo->event_counts_sfd_detect = 0;

    /* dwt_xx calls in app level Must be in protected mode (DW3000 IRQ disabled) */
    hal_uwb.disableIRQ();
    unsigned int lock = qirq_lock();

    if (dwt_initialise(0) != DWT_SUCCESS) /**< set callbacks to NULL inside dwt_initialise*/
    {
        return (_ERR_INIT);
    }

    /* Configure SPI to fast rate */
    if (hal_uwb.uwbs != NULL)
    {
        hal_uwb.uwbs->spi->fast_rate(hal_uwb.uwbs->spi->handler);
    }

    // ----------------------------------- ----------------
    // ---------  Configuration of UWB PHY ----------------
    // ----------------------------------- ----------------
    rxtx_investigator_configure(get_dwt_config(), DWT_FF_DISABLE);
    investigator_configure_uwb(rx_investigator_cb, investigator_timeout_cb, investigator_error_cb);
    // ----------------------------------- ----------------

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


void investigator_process_terminate(void)
{
    // stop the RTC timer
    /* configure the RTC Wakeup timer with a high priority;
     * this timer is saving global Super Frame Timestamp,
     * so we want this timestamp as stable as we can.
     *
     * */
    
    Rtc.disableIRQ();
    Rtc.setPriorityIRQ();

    if (pInvestigatorInfo)
    {
        NODE_FREE(pInvestigatorInfo);
        pInvestigatorInfo = NULL;
    }
}

//-----------------------------------------------------------------------------
