#ifndef __investigator__H__
#define __investigator__H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uwb_frames.h"
#include "appConfig.h"
#include "HAL_uwb.h"
#include "duz_app_config.h"
#include "reporter.h"

#define investigator_DATA 2


//-----------------------------------------------------------------------------
/*
 * Rx Events circular buffer : used to transfer RxPckt from ISR to APP.
 * As per design, the amount of RxPckt in the buffer at any given time shall not be more than 1.
 * */
#define EVENT_BUF_L_SIZE     16


//-----------------------------------------------------------------------------

/* RxPckt is the structure is for the current reception */
struct rx_investigator_pckt_s
{
    int16_t rxDataLen;
    uint8_t data[EVENT_BUF_L_SIZE];
    uint8_t timeStamp[TS_40B_SIZE]; /* Full TimeStamp */
    uint32_t status;
    int rssi;
    int fsl;
    int16_t clock_offset;
};

typedef struct rx_investigator_pckt_s rx_investigator_pckt_t;


/* This structure holds investigator's application parameters */
struct investigator_info_s
{
    /* circular Buffer of received Rx packets :
     * uses in transferring of the data from ISR to APP level.
     * */
    struct
    {
        rx_investigator_pckt_t buf[EVENT_BUF_L_SIZE];
        uint16_t           head;
        uint16_t           tail;
    } rxPcktBuf;

    dwt_deviceentcnts_t event_counts;
    uint32_t event_counts_sfd_detect; /* Counts the number of SFD detections (RXFR has to be set also). */
};

typedef struct investigator_info_s investigator_info_t;

//-----------------------------------------------------------------------------
// HW-specific function implementation
//
void investigator_rssi_cal(int *rsl100, int *fsl100);
void investigator_readrxtimestamp(uint8_t *timestamp);
int  investigator_readstsquality(int16_t *rxStsQualityIndex);
void investigator_configure_uwb(dwt_cb_t cbRxOk, dwt_cb_t cbRxTo, dwt_cb_t cbRxErr);
void investigator_deinit(void);


//-----------------------------------------------------------------------------
// exported functions prototypes
//
extern investigator_info_t *getInvestigatorInfoPtr(void);

//-----------------------------------------------------------------------------
// exported functions prototypes
//

/* investigator (investigator) */
void parse_investigator_rx(const dwt_cb_data_t *);
void investigator_blink();
void start_investigator_tx();

error_e investigator_process_init(void);
void investigator_process_terminate(void);
void investigator_set_mode(int mode);
int  investigator_get_mode(void);

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif

#endif /* __investigator__H__ */
