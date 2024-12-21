#include "utils.h"


uint32_t get_dwt_time_ms()
{
    return dwt_readsystimestamphi32()/125000;
}

float pdoa2degree(uint32_t pdoa)
{
   return ((float)pdoa / (1 << 11)) * 180 / M_PI; 
}

void rssi_cal(int *rssi, int *fsl)
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


