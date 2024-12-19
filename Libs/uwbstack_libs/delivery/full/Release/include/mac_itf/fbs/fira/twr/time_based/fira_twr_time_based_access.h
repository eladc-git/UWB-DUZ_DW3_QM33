/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154_schedule.h"
#include "qerr.h"

#include <stdbool.h>
#include <stdint.h>

struct fira_ranging_info;
struct fira_twr_ranging_info;
struct fira_session;
struct fbs_session_demand;
struct fira_session_params;
struct fira_slot;
struct mcps802154_rx_frame_info;
struct mcps802154_rx_measurement_info;
struct mcps802154_tx_frame_info;
struct sk_buff;

/**
 * fira_twr_time_based_access_entering() - Entering in the TWR access.
 * @session: FiRa session context.
 * @fsd: FiRa Session Demand result from the get_demand of the session.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_twr_time_based_access_entering(struct fira_session *session,
					      const struct fbs_session_demand *fsd);

/**
 * fira_twr_time_based_access_entering_controlee() - Entering in the TWR access,
 * controlee side.
 * @session: FiRa session context.
 * @fsd: FiRa Session Demand result from the get_demand of the session.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_twr_time_based_access_entering_controlee(struct fira_session *session,
							const struct fbs_session_demand *fsd);

/**
 * fira_twr_access_rx_frame_rssi_pdoa() - Common TWR function to compute RSSI /
 * PDOA.
 * @session: FiRa session context.
 * @params: Current session parameters.
 * @info: Information on a received frame.
 * @ranging_info_twr: Ranging information for TWR Measurement.
 * @meas_info: Measurements on a received frame.
 */
void fira_twr_access_rx_frame_rssi_pdoa(struct fira_session *session,
					const struct fira_session_params *params,
					const struct mcps802154_rx_frame_info *info,
					struct fira_twr_ranging_info *ranging_info_twr,
					struct mcps802154_rx_measurement_info *meas_info);

/**
 * fira_rx_frame_control() - Parse control part of a frame.
 * @session: FiRa session context.
 * @slot: The slot with the frame to parse.
 * @skb: Frame buffer.
 * @info: Frame config information.
 * @ranging_info: Ranging info to be modified.
 */
void fira_rx_frame_control(struct fira_session *session, const struct fira_slot *slot,
			   struct sk_buff *skb, const struct mcps802154_rx_frame_info *info,
			   struct fira_ranging_info *ranging_info);

/**
 * fira_twr_rx_frame_ranging() - Parse ranging part of a frame.
 * @session: FiRa session context.
 * @slot: The slot with the frame to parse.
 * @skb: Frame buffer.
 * @info: Frame config information.
 * @ranging_info_twr: Ranging information for TWR Measurement to be modified.
 * @meas_info: Measurements on a received frame.
 */
void fira_twr_rx_frame_ranging(struct fira_session *session, struct fira_slot *slot,
			       struct sk_buff *skb, const struct mcps802154_rx_frame_info *info,
			       struct fira_twr_ranging_info *ranging_info_twr,
			       struct mcps802154_rx_measurement_info *meas_info);

/**
 * fira_rx_frame_measurement_report() - Parse Measurement part of a frame.
 * @session: FiRa session context.
 * @slot: The slot with the frame to parse.
 * @skb: Frame buffer.
 * @info: Frame config information.
 * @ranging_info: Ranging info to be modified.
 */
void fira_rx_frame_measurement_report(struct fira_session *session, const struct fira_slot *slot,
				      struct sk_buff *skb,
				      const struct mcps802154_rx_frame_info *info,
				      struct fira_ranging_info *ranging_info);

/**
 * fira_twr_access_setup_frame() - Fill an access frame from a FiRa slot.
 * @session: FiRa session context.
 * @slot: Corresponding slot.
 * @timestamp_dtu: Frame transmission or reception date.
 * @rx_timeout_dtu: Timeout value for RX in device time unit (if zero, use a
 * default timeout value).
 *
 * Return: information about the frame needed by MCPS802154 or NULL in case of
 * error.
 */
const struct mcps802154_access_frame *fira_twr_access_setup_frame(struct fira_session *session,
								  const struct fira_slot *slot,
								  uint32_t timestamp_dtu,
								  int rx_timeout_dtu);

/**
 * fira_twr_access_select_antenna_set_ids() - Sets antenna ids.
 * @session: FiRa session context.
 */
void fira_twr_access_select_antenna_set_ids(struct fira_session *session);

/**
 * fira_twr_access_done() - Process access done.
 * @session: FiRa session context.
 * @reason: Reason for the end of access.
 * @report_allowed: True when report are allowed to be send.
 */
void fira_twr_access_done(struct fira_session *session, enum mcps802154_access_done_reason reason,
			  bool report_allowed);

/**
 * fira_twr_time_based_access_tx_get_frame() - Return a frame to send.
 * @access: Access context.
 *
 * Return: Buffer or NULL on failure.
 */
struct sk_buff *fira_twr_time_based_access_tx_get_frame(struct mcps802154_access *access);

/**
 * fira_twr_time_based_access_tx_return() - Manage tx buffer after tx (free...).
 * @access: Access context.
 * @skb: Frame buffer.
 * @info: Additional info for tx frame.
 * @reason: Reason for returning buffer
 */
void fira_twr_time_based_access_tx_return(struct mcps802154_access *access, struct sk_buff *skb,
					  const struct mcps802154_tx_frame_info *info,
					  enum mcps802154_access_tx_return_reason reason);

/**
 * fira_rx_frame_result_report() - Parse result report.
 * @session: FiRa session context.
 * @slot: The slot with the frame to parse.
 * @skb: Frame buffer.
 * @info: Frame config information.
 * @ranging_info: Ranging info to be modified.
 */
void fira_rx_frame_result_report(struct fira_session *session, const struct fira_slot *slot,
				 struct sk_buff *skb, const struct mcps802154_rx_frame_info *info,
				 struct fira_ranging_info *ranging_info);

/**
 * fira_controlee_resync() - Synchronize a Controlee thanks to the phyStsIndex.
 * @session: FiRa session context.
 * @phy_sts_index: phyStsIndex used for synchronization.
 * @timestamp_dtu: Timestamp of the CM used for synchronization.
 */
void fira_controlee_resync(struct fira_session *session, uint32_t phy_sts_index,
			   uint32_t timestamp_dtu);
