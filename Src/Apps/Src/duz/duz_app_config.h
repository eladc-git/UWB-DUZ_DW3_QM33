#ifndef DUZ_APP_CONFIG_H
#define DUZ_APP_CONFIG_H

#include "deca_device_api.h"


#define DEBUG_PRINT                             1


// ------------------------ INVESTIGATOR ----------------------------
#define INVESTIGATOR_ID                         0x11111111
#define INVESTIGATOR_BLINK_COUNT                200         // Number of blinks to be trasmitt by investigator
#define INVESTIGATOR_BLINK_INTERVAL_MS          1           // Time [ms] between each blink
#define INVESTIGATOR_RECEIVER_TIME_MS           400         // Time [ms] for receive to wait for AOA
#define INVESTIGATOR_TX_TIME_MS                 (INVESTIGATOR_BLINK_COUNT-1)*INVESTIGATOR_BLINK_INTERVAL_MS
#define INVESTIGATOR_PUSH_BUTTON_PIN_NUM        30          // Push button for trigger initiation

// ------------------------ RESPONDER -------------------------------
#define RESPONDER_ID                            0x22222222
#define RESPONDER_RECEIVER_ON_MS                5           // Time [ms] for receiver to be ON for getting initiations
#define RESPONDER_RECEIVER_OFF_MS               100         // Time [ms] for receiver to be OFF
#define RESPONDER_BLINK_COUNT                   3           // Number of blinks to be trasmitt by responder
#define RESPONDER_BLINK_INTERVAL_MS             100         // Time [ms] between each blink

#endif // DUZ_APP_CONFIG_H
