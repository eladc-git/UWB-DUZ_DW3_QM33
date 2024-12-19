/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "deca_regs.h"
#include "qsoc_reg_access.h"
#include "uwb_device_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DWT_SFD_IEEE_4Z DWT_SFD_IEEE_4Z_8

#define RX_A DWT_RX_A_ON
#define RX_B (DWT_RX_B_ON | DWT_RX_ADC_COMPARATOR_SWAP)
#define RX_AB (DWT_RX_A_ON | DWT_RX_B_ON)
#define RX_BA (RX_AB | DWT_RX_ADC_COMPARATOR_SWAP)
#define RX_AB_DIV (RX_AB | DWT_RX_DSPR_DVSTY_ON | DWT_RX_PHASOR_DVSTY_ON)
#define RX_BA_DIV (RX_BA | DWT_RX_DSPR_DVSTY_ON | DWT_RX_PHASOR_DVSTY_ON)

/* Chip capabilities. */
#define UWB_LLHW_PRF_MODE_SUPPORTED (MCPS802154_LLHW_BPRF | MCPS802154_LLHW_HPRF)
#define UWB_LLHW_DATA_RATE_SUPPORTED                                       \
	(MCPS802154_LLHW_DATA_RATE_850K | MCPS802154_LLHW_DATA_RATE_6M81 | \
	 MCPS802154_LLHW_DATA_RATE_7M80 | MCPS802154_LLHW_DATA_RATE_27M2 | \
	 MCPS802154_LLHW_DATA_RATE_31M2 | MCPS802154_LLHW_DATA_RATE_CUSTOM)
#define UWB_LLHW_PRF_SUPPORTED                                                       \
	(MCPS802154_LLHW_PRF_16 | MCPS802154_LLHW_PRF_64 | MCPS802154_LLHW_PRF_125 | \
	 MCPS802154_LLHW_PRF_250)
#define UWB_LLHW_PHR_DATA_RATE_SUPPORTED \
	(MCPS802154_LLHW_PHR_DATA_RATE_850K | MCPS802154_LLHW_PHR_DATA_RATE_6M81)
#define UWB_LLHW_PSR_SUPPORTED                                                          \
	(MCPS802154_LLHW_PSR_32 | MCPS802154_LLHW_PSR_64 | MCPS802154_LLHW_PSR_128 |    \
	 MCPS802154_LLHW_PSR_256 | MCPS802154_LLHW_PSR_512 | MCPS802154_LLHW_PSR_1024 | \
	 MCPS802154_LLHW_PSR_2048 | MCPS802154_LLHW_PSR_4096)
#define UWB_LLHW_SFD_SUPPORTED                                                          \
	(MCPS802154_LLHW_SFD_4A | MCPS802154_LLHW_SFD_4Z_8 | MCPS802154_LLHW_SFD_4Z_4 | \
	 MCPS802154_LLHW_SFD_4Z_16 | MCPS802154_LLHW_SFD_4Z_32)
#define UWB_LLHW_STS_SEG_SUPPORTED                                       \
	(MCPS802154_LLHW_STS_SEGMENT_1 | MCPS802154_LLHW_STS_SEGMENT_2 | \
	 MCPS802154_LLHW_STS_SEGMENT_3 | MCPS802154_LLHW_STS_SEGMENT_4)
#define UWB_LLHW_AOA_SUPPORTED                                                                   \
	(MCPS802154_LLHW_AOA_AZIMUTH | MCPS802154_LLHW_AOA_ELEVATION | MCPS802154_LLHW_AOA_FOM | \
	 MCPS802154_LLHW_AOA_AZIMUTH_FULL)

/* Non standard chip capabilities. */
#define UWB_LLHW_SUPPORT_DATA_RATE_54M
#define UWB_LLHW_SUPPORT_DATA_RATE_108M

/* Specific supported features. */
#define UWB_LLHW_SUPPORT_RADAR
#define UWB_LLHW_SUPPORT_LOOPBACK
#define UWB_LLHW_SUPPORT_L1_CONFIG
#define UWB_LLHW_SUPPORT_PDOA_TABLE /* Chip supports multiple PDOA segments. \
				     */
#define UWB_LLHW_SUPPORT_ZERO_PSDU

/* Support two receivers. */
#define UWB_LLHW_SUPPORT_DUAL_RX

/* Chip supports HPRF frames. */
#define UWB_LLHW_SUPPORT_HPRF

#ifdef CONFIG_LLHW_UWB_WAKEUP_LATENCY
#define UWB_WAKEUP_LATENCY_US (CONFIG_LLHW_UWB_WAKEUP_LATENCY + CONFIG_LLHW_MCU_WAKEUP_LATENCY)
#else
/* UWB wake-up latency.
 * When CONFIG_PM is activated, MCU enters S3a during opportunistic deep-sleep,
 * so we need more time to wake-up. */
#ifdef CONFIG_PM
/* Current latency is estimated to 5000 us.
 * Add more margin to absorb a potential 32Khz ppm error,
 * range from 10 to 100ppm. */
#define UWB_WAKEUP_LATENCY_US 5500
#else
#define UWB_WAKEUP_LATENCY_US 2000
#endif /* CONFIG_PM. */
#endif /* CONFIG_LLHW_UWB_WAKEUP_LATENCY. */

#ifdef __cplusplus
}
#endif
