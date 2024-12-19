/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_session.h"
#include "fbs/fira/fira_session.h"
#include "fixed_point.h"
#include "net/mcps802154.h"
#include "net/mcps802154_schedule.h"
#include "quwbs/fbs/defs.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct fbs_session_runtime;
struct fira_ranging_info;
struct fira_twr_ranging_info;
struct fira_slot;
struct fira_workspace;

static inline struct fira_session *access_to_fira(const struct mcps802154_access *access)
{
	return fbs_to_fira(access_to_session(access));
}

/**
 * fira_access_rx_sts_good() - Verifies STS FoM value against
 * FIRA_STS_FOM_THRESHOLD.
 * @info: Ranging info to be modified.
 *
 * Return: True if STS above threshold.
 */
bool fira_access_rx_sts_good(const struct mcps802154_rx_frame_info *info);

/**
 * fira_access_ranging_info_set_status() - Sets status to ranging info
 * structure.
 * @ranging_info: Ranging info to be modified.
 * @status: Status to be set.
 * @slot_index: In case of failure, the slot index where it has occured.
 */
void fira_access_ranging_info_set_status(struct fira_ranging_info *ranging_info,
					 enum quwbs_fbs_status status, uint8_t slot_index);

/**
 * fira_access_set_short_address() - Sets short address in access context.
 * @session: Session context.
 * @access: Access context.
 *
 * Return: Short address.
 */
uint16_t fira_access_set_short_address(const struct fira_session *session,
				       struct mcps802154_access *access);

/**
 * fira_access_get_channel() - Retrieve the channel to applied on the access.
 * @session: Session context.
 *
 * Return: The channel.
 */
const struct mcps802154_channel *fira_access_get_channel(struct fira_session *session);

/**
 * fira_access_ranging_status_from_error() - Convert an MCPS Rx error
 * code to the corresponding ranging status
 * @error: Type of the RX error.
 *
 * Return: corresponding ranging status.
 */
enum quwbs_fbs_status fira_access_ranging_status_from_error(enum mcps802154_rx_error_type error);

/**
 * fira_access_handle_rejected_first_rx() - Update access duration when error
 * occurs on first slot of the access.
 * code to the corresponding ranging status
 * @access: Access context.
 * @session: Session context.
 * @info: MCPS802154 information about received frame.
 */
void fira_access_handle_rejected_first_rx(struct mcps802154_access *access,
					  const struct fira_session *session,
					  const struct mcps802154_rx_frame_info *info);

/**
 * fira_access_set_ranging_status_from_error() - Set the ranging status
 * based on the RX error.
 * @ranging_info: Ranging info to be modified.
 * @error: Type of the RX error.
 * @slot_index: Slot index where the RX occured.
 *
 * Return: True if there is not error.
 */
bool fira_access_set_ranging_status_from_error(struct fira_ranging_info *ranging_info,
					       enum mcps802154_rx_error_type error,
					       uint8_t slot_index);
/**
 * fira_access_set_ranging_statuses_from_tx_return_reason() - Set all the ranging
 * statuses based on the reason of TX buffer return.
 * @ws: FiRa workspace context.
 * @slot: Slot where the TX occured.
 * @reason: Reason of TX buffer return.
 */
void fira_access_set_ranging_statuses_from_tx_return_reason(
	struct fira_workspace *ws, const struct fira_slot *slot,
	enum mcps802154_access_tx_return_reason reason);

/**
 * fira_alloc_slots() - Allocate FiRa slots.
 * @ws: FiRa workspace context.
 * @next_index: Next index to allocate in the array of elements.
 * @n_slots: Number of elements to allocate.
 *
 * Return: Pointer to the last allocated element on success or NULL.
 */
struct fira_slot *fira_alloc_slots(struct fira_workspace *ws, int next_index, int n_slots);

/**
 * fira_destroy_slots() - Destroy FiRa slots from array.
 * @ws: FiRa workspace context.
 */
void fira_destroy_slots(struct fira_workspace *ws);

/**
 * fira_alloc_ranging_info() - Allocate ranging info struct for a given
 * size (measurement type specific) and initialize the status field.
 * @ws: FiRa workspace context.
 * @next_index: Next index to allocate in the array of elements.
 * @size: Size of the memory to allocate.
 *
 * Return: Pointer to the allocated element on success or NULL.
 */
struct fira_ranging_info *fira_alloc_ranging_info(struct fira_workspace *ws, int next_index,
						  size_t size);

/**
 * fira_destroy_ranging_info() - Destroy all ranging info structs from the array.
 * @ws: FiRa workspace context.
 */
void fira_destroy_ranging_info(struct fira_workspace *ws);

/**
 * fira_access_map_fom_to_percent() - map FOM from uint16_t to percent.
 * @fom: FOM value.
 *
 * Return: FOM in percents.
 */
static inline uint8_t fira_access_map_fom_to_percent(uint16_t fom)
{
	return (fom * 100 + 255 / 2) / 255;
}

/**
 * fira_access_map_cfo_rctu_to_q26() - map CFO from RCTU to ppm Q26.
 * @info: Rx frame info containing offset and interval in RCTU.
 *
 * Return: CFO as Q26 (16 bits).
 */
static inline int16_t fira_access_map_cfo_rctu_to_q26(const struct mcps802154_rx_frame_info *info)
{
	return ((int64_t)info->ranging_offset_rctu * (1 << 26)) /
	       info->ranging_tracking_interval_rctu;
}

/**
 * fira_access_map_cfo_rctu_to_q6_10() - map CFO from RCTU to ppm Q6.10.
 * @info: Rx frame info containing offset and interval in RCTU.
 *
 * Return: CFO as Q6.10.
 */
static inline int16_t fira_access_map_cfo_rctu_to_q6_10(const struct mcps802154_rx_frame_info *info)
{
	return convert_q26_to_q10(fira_access_map_cfo_rctu_to_q26(info));
}

/**
 * fira_access_need_regular_report() - Check if the access needs a report.
 * @access: Access context.
 * @runtime: FBS runtime context.
 * @reason: Type of access done.
 *
 * Return: True when a report is needed.
 */
bool fira_access_need_regular_report(const struct mcps802154_access *access,
				     const struct fbs_session_runtime *runtime,
				     enum mcps802154_access_done_reason reason);

/**
 * fira_twr_alloc_ranging_info() - Allocate ranging info struct for TWR measurement
 * and initialize the status field.
 * @ws: FiRa workspace context.
 * @next_index: Next index to allocate in the array of elements.
 *
 * Return: True if allocated successfully, false otherwise.
 */
bool fira_twr_alloc_ranging_info(struct fira_workspace *ws, int next_index);

/**
 * fira_twr_destroy_psdus_report() - Destroy all stored PSDUs for this ranging round.
 * @ws: FiRa workspace context.
 */
void fira_twr_destroy_psdus_report(struct fira_workspace *ws);

/**
 * fira_access_twr_ranging_info_set_cfo() - Add the information
 * about the clock frequency offset to ranging info structure for TWR measurement.
 * @info: MCPS802154 information about received frame.
 * @ranging_info_twr: Ranging information for TWR measurement to be modified.
 */
void fira_access_twr_ranging_info_set_cfo(struct fira_twr_ranging_info *ranging_info_twr,
					  const struct mcps802154_rx_frame_info *info);
