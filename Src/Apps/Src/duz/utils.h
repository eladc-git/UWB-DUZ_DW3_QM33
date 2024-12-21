#ifndef DUZ_APP_UTILS_C
#define DUZ_APP_UTILS_C

#include "driver_app_config.h"
#include <deca_device_api.h>
#include <deca_types.h>
#include <stdlib.h>
#include <math.h>

#define NODE_MALLOC qmalloc
#define NODE_FREE   qfree

#define RX_CODE_THRESHOLD  8      // For 64 MHz PRF the RX code is 9.
#define ALPHA_PRF_16       113.8  // Constant A for PRF of 16 MHz. See User Manual for more information.
#define ALPHA_PRF_64       120.7  // Constant A for PRF of 64 MHz. See User Manual for more information.
#define LOG_CONSTANT_C0    63.2   // 10log10(2^21) = 63.2    // See User Manual for more information.
#define LOG_CONSTANT_D0_E0 51.175 // 10log10(2^17) = 51.175  // See User Manual for more information.
#define M_PI		       3.14159265358979323846

uint32_t get_dwt_time_ms();
float pdoa2degree(uint32_t);
void rssi_cal(int *, int *);



#endif // DUZ_APP_UTILS_C