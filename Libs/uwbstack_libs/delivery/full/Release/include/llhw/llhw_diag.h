/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "llhw.h"
#include "llhw_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Q8 offset. */
#define Q8_SHIFT 8
/* Threshold to determine between HPRF and BPRF */
#define PREAMBLE_CODE_PRF_THRESHOLD (25)

/**
 * llhw_read_diagnostics - Read diagnostics
 * @uwb: the UWB chip.
 * @info: the returned RX measurement informations.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_read_diagnostics(struct uwb_chip *uwb, struct mcps802154_rx_measurement_info *info);

/**
 * llhw_get_segments_to_measure - Read segments for which metrics and/or CIRs
 * can be measured on a given rx frame.
 * @uwb: the UWB chip.
 *
 * Return: the number of segments that can be analyzed.
 *
 * NOTE: The method to compute the segments to retrieve depends on the type of
 * measurement done:
 * - In case of PDoA: it is based on the number of consecutive pairs of
 *   pdoa_segments different from 0.
 *   For example with pdoa_segments configured like:
 *    - 0 seg: pdoa_segments = { PDOA_SEG_IP_S, 0 }, { 0, 0 }, { 0, 0 }
 *    - 2 segs: pdoa_segments = { PDOA_SEG_IP_S, PDOA_SEG_IP_M }, { 0, 0 },
 *                              { 0, 0 }
 *    - 2 segs: pdoa_segments = { PDOA_SEG_IP_S, PDOA_SEG_IP_M }, { 0, 0 },
 *                              { PDOA_SEG_STS0_S, PDOA_SEG_STS0_M }
 *    - 3 segs: pdoa_segments = { PDOA_SEG_IP_S, PDOA_SEG_IP_M },
 *                              { PDOA_SEG_STS0_S, PDOA_SEG_IP_M }, { 0, 0 }
 *    - 4 segs: pdoa_segments = { PDOA_SEG_IP_S, PDOA_SEG_IP_M },
 *                              { PDOA_SEG_STS0_S, PDOA_SEG_STS0_M }, { 0, 0 }
 *   Note: since pdoa_segments must be configured per pair, the algorithm
 *   assumes that if pdoa_segments[0][0] is set to 0, then is means there is no
 *   PDoA measurement. And thus the algorithm uses the ToA method (see below).
 * - In case of ToA: it is based on the number of receiver configured:
 *    - 1 seg: nb_receiver = 1, use:
 *         - PDOA_SEG_STS0_M for RFRAME
 *         - PDOA_SEG_IP_M for non RFRAME (SP0).
 *    - 2 segs: nb_receiver = 2, use:
 *         - PDOA_SEG_STS0_M and PDOA_SEG_STS0_S for RFRAME
 *         - PDOA_SEG_IP_M and PDOA_SEG_IP_S for non RFRAME (SP0).
 *
 */
uint8_t llhw_get_segments_to_measure(struct uwb_chip *uwb);

/**
 * llhw_get_dual_rx_rsl - Get RSLs of Master and Slave receivers.
 * @uwb: the UWB chip.
 * @rsl_dbm: The returned table containing the computed RSLs. First one is
 * Master RSL, second one is Slave RSL.
 */
void llhw_get_dual_rx_rsl(struct uwb_chip *uwb, int16_t rsl_dbm[UWB_DUAL_RX_AUTO_RSL_IDX_NB]);

#ifdef __cplusplus
}
#endif
