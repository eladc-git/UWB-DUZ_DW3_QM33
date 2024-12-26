#ifndef DUZ_APP_UTILS_C
#define DUZ_APP_UTILS_C

#include "duz_app_config.h"
#include "driver_app_config.h"
#include <deca_device_api.h>
#include <deca_types.h>
#include "deca_dbg.h"
#include <stdlib.h>
#include <math.h>

#if DEBUG_PRINT
#define debug_print diag_printf
#else
#define debug_print do_nothing
#endif

#define NODE_MALLOC qmalloc
#define NODE_FREE   qfree

#define RX_CODE_THRESHOLD  8      // For 64 MHz PRF the RX code is 9.
#define ALPHA_PRF_16       113.8  // Constant A for PRF of 16 MHz. See User Manual for more information.
#define ALPHA_PRF_64       120.7  // Constant A for PRF of 64 MHz. See User Manual for more information.
#define LOG_CONSTANT_C0    63.2   // 10log10(2^21) = 63.2    // See User Manual for more information.
#define LOG_CONSTANT_D0_E0 51.175 // 10log10(2^17) = 51.175  // See User Manual for more information.
#define M_PI		       3.14159265358979323846

float pdoa2degree(int16_t, int16_t);
void rssi_cal(int *, int *);
void do_nothing(char *s, ...);

#endif // DUZ_APP_UTILS_C