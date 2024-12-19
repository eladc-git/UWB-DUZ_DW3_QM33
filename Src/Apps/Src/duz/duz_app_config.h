#ifndef DUZ_APP_CONFIG_H
#define DUZ_APP_CONFIG_H

#include "deca_device_api.h"

#define RESPONDER_MODE               1
#define INIT_TIME_MS                 1000
#define INVEST_TIME_MS               1000
#define DEBUG_PRINT                  1


// ------------------------ INVESTIGATOR ----------------------------
#define INVESTIGATOR_ID                         0x0001
#define INVESTIGATOR_BLINK_COUNT                2      // Number of blinks to be trasmitt by responder
#define INVESTIGATOR_BLINK_INTERVAL_MS          100    // Time [ms] between each blink

// ------------------------ RESPONDER -------------------------------
#define RESPONDER_ID                            0x0001
#define RESPONDER_BLINK_COUNT                   2      // Number of blinks to be trasmitt by responder
#define RESPONDER_BLINK_INTERVAL_MS             100    // Time [ms] between each blink


#endif // DUZ_APP_CONFIG_H
