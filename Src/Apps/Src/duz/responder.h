#ifndef __responder__H__
#define __responder__H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uwb_frames.h"
#include "appConfig.h"
#include "HAL_uwb.h"
#include "duz_app_config.h"
#include "reporter.h"



//-----------------------------------------------------------------------------
/*
 * Rx Events circular buffer : used to transfer RxPckt from ISR to APP.
 * As per design, the amount of RxPckt in the buffer at any given time shall not be more than 1.
 * */
#define EVENT_BUF_L_SIZE     16


//-----------------------------------------------------------------------------


/* RxPckt is the structure is for the current reception */
struct rx_responder_pckt_s
{
    int16_t rxDataLen;
    uint8_t data[EVENT_BUF_L_SIZE];
    uint8_t timeStamp[TS_40B_SIZE]; /* Full TimeStamp */
    uint32_t status;
    uint32_t id;
    uint32_t seq_count;
    int rssi;
    int fsl;
    int16_t clock_offset;
};

typedef struct rx_responder_pckt_s rx_responder_pckt_t;


/* This structure holds responder's application parameters */
struct responder_info_s
{
    /* circular Buffer of received Rx packets :
     * uses in transferring of the data from ISR to APP level.
     * */
    struct
    {
        rx_responder_pckt_t buf[EVENT_BUF_L_SIZE];
        uint16_t           head;
        uint16_t           tail;
    } rxPcktBuf;

    dwt_deviceentcnts_t event_counts;
    uint32_t event_counts_sfd_detect; /* Counts the number of SFD detections (RXFR has to be set also). */
};

typedef struct responder_info_s responder_info_t;

//-----------------------------------------------------------------------------
// HW-specific function implementation
//
void responder_rssi_cal(int *rsl100, int *fsl100);
void responder_readrxtimestamp(uint8_t *timestamp);
int  responder_readstsquality(int16_t *rxStsQualityIndex);
void responder_configure_uwb(dwt_cb_t cbRxOk, dwt_cb_t cbRxTo, dwt_cb_t cbRxErr);
void responder_deinit(void);


//-----------------------------------------------------------------------------
// exported functions prototypes
//
extern responder_info_t *getResponderInfoPtr(void);

//-----------------------------------------------------------------------------
// exported functions prototypes
//

/* responder (responder) */
void parse_responder_rx(const dwt_cb_data_t *);
void responder_blink();
void start_responder_tx();

error_e responder_process_init(void);
void responder_process_terminate(void);
void responder_set_mode(int mode);
int  responder_get_mode(void);

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif

#endif /* __responder__H__ */
