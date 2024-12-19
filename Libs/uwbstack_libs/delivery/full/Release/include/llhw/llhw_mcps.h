/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "llhw.h"
#include "llhw_internal.h"
#include "llhw_mcps_task.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * llhw_mcps_init() - initialize and allocate a new low-level hardware device.
 * @uwb: the UWB device to initialize.
 *
 * Return: the allocated Low-level hardware without MCPS.
 */
enum qerr llhw_mcps_init(struct uwb_chip *uwb);

/**
 * llhw_mcps_deinit() - deinitialize and deallocate the low-level hardware
 * device.
 */
void llhw_mcps_deinit(void);

/**
 * llhw_mcps_rx_cb() - callback that handles RX good frame event.
 * @rxd: TX/RX call-back data.
 */
void llhw_mcps_rx_cb(const dwt_cb_data_t *rxd);

/**
 * llhw_mcps_rxerror_cb() - callback that handles RX error frame event.
 * @rxd: TX/RX call-back data.
 */
void llhw_mcps_rxerror_cb(const dwt_cb_data_t *rxd);

/**
 * llhw_mcps_rxtimeout_cb() - callback that handles RX timeout frame event.
 * @rxd: TX/RX call-back data.
 */
void llhw_mcps_rxtimeout_cb(const dwt_cb_data_t *rxd);

/**
 * llhw_mcps_txdone_cb() - callback that handles TX done frame event.
 * @txd: TX/RX call-back data.
 */
void llhw_mcps_txdone_cb(const dwt_cb_data_t *txd);

/**
 * llhw_mcps_timerexpired_cb() - callback that handles timer expiration event.
 * @data: TX/RX call-back data.
 */
void llhw_mcps_timerexpired_cb(const dwt_cb_data_t *data);

/**
 * llhw_mcps_pctt_loopback_rx_cb() - specific callback that handles RX error
 * frame event for Loopback test only.
 * @cb_data: TX/RX call-back data.
 */
void llhw_mcps_pctt_loopback_rx_cb(const dwt_cb_data_t *cb_data);

/**
 * llhw_mcps_pctt_loopback_rxerror_cb() - specific callback that handles RX
 * error or timeout frame event for Loopback test only.
 * @cb_data: TX/RX call-back data.
 */
void llhw_mcps_pctt_loopback_rxerror_cb(const dwt_cb_data_t *cb_data);

/**
 * llhw_mcps_pctt_loopback_txdone_cb() - specific callback that handles TX done
 * frame event for Loopback test only.
 * @cb_data: TX/RX call-back data.
 */
void llhw_mcps_pctt_loopback_txdone_cb(const dwt_cb_data_t *cb_data);

/**
 * llhw_mcps_call_wakeup_callback() - call wake-up callback.
 * @dss: deep-sleep informations.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_mcps_call_wakeup_callback(struct uwb_deep_sleep_state *dss);

/**
 * llhw_mcps_rxtimeout() - handles RX timeout frame event (triggered from timer
 * configured in software).
 * @uwb: the UWB device.
 */
void llhw_mcps_rxtimeout(struct uwb_chip *uwb);

#ifdef __cplusplus
}
#endif
