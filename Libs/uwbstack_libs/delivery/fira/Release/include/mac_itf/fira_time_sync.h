/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/fira_region_params.h"
#include "qerr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The Time Sync objects are associated with local session, which means that
 * for each local session a new fira_time_sync context is created.
 */
struct fira_time_sync;

/**
 * enum fira_time_sync_location_coord - Coordinate configuration
 * @FIRA_TIME_SYNC_LOCATION_COORD_WGS84: WGS-84 coordinate system is 12 bytes
 *	size value with 33-bits latitude, 33-bits longitude, 30-bits altitude
 *	all in WGS-84 coordinate system.
 * @FIRA_TIME_SYNC_LOCATION_COORD_RELATIVE: Relative coordinate system is 10
 *	bytes size value with 28-bits X-coordinate, 28-bits Y-coordinate, 24-bits
 *	Z-coordinate all in the relative coordinate system in the unit of
 *	millimeters.
 * @FIRA_TIME_SYNC_LOCATION_COORD_NONE: no location available.
 */
enum fira_time_sync_location_coord {
	FIRA_TIME_SYNC_LOCATION_COORD_WGS84 = 0,
	FIRA_TIME_SYNC_LOCATION_COORD_RELATIVE = 1,
	FIRA_TIME_SYNC_LOCATION_COORD_NONE = 0xff
};

/**
 * fira_time_sync_new() - Allocate and initialize the time synchronisation
 * context.
 *
 * Return: Address of the new time synchronisation context.
 */
struct fira_time_sync *fira_time_sync_new(void);

/**
 * fira_time_sync_free() - Delete the time synchronisation context.
 * @time_sync: Time synchronization context.
 */
void fira_time_sync_free(struct fira_time_sync *time_sync);

/**
 * fira_time_sync_set_poll_timestamps() - Set the timestamps of the Poll DTM.
 * @ctx : Time synchronisation context involved.
 * @tx_timestamp_poll_common_base_rctu : Tx Timestamp of the Poll DTM in the
 * common time base in rctu unit.
 * @rx_timestamp_poll_local_base_rctu : Rx Timestamp of the Poll DTM in the
 * local time base in rctu unit.
 */
void fira_time_sync_set_poll_timestamps(struct fira_time_sync *ctx,
					uint64_t tx_timestamp_poll_common_base_rctu,
					uint64_t rx_timestamp_poll_local_base_rctu);

/**
 * fira_time_sync_set_cfo() - Set one cfo sample.
 * @ctx : Time synchronisation context involved.
 * @cfo_q26 : Clock frequency offset with respect to the initiator DT-Anchor as
 * Q26 (16 bits).
 */
void fira_time_sync_set_cfo(struct fira_time_sync *ctx, int16_t cfo_q26);

/**
 * fira_time_sync_get_time_sync_location_coord_type() - Translate location type
 * from MAC value to interface value.
 * @presence: validity flag of the type.
 * @type : input coordinate system.
 *
 * Return: the translated value.
 */
static inline enum fira_time_sync_location_coord
fira_time_sync_get_time_sync_location_coord_type(bool presence,
						 enum fira_dt_location_coord_system_type type)
{
	if (!presence)
		return FIRA_TIME_SYNC_LOCATION_COORD_NONE;
	if (type == FIRA_DT_LOCATION_COORD_WGS84)
		return FIRA_TIME_SYNC_LOCATION_COORD_WGS84;
	if (type == FIRA_DT_LOCATION_COORD_RELATIVE)
		return FIRA_TIME_SYNC_LOCATION_COORD_RELATIVE;
	return FIRA_TIME_SYNC_LOCATION_COORD_NONE;
}

/**
 * fira_time_sync_set_local_location() - Set the coordinates of the local
 * Anchor.
 * @ctx : Time synchronisation context involved.
 * @anchor_local_short_addr : Anchor local short address.
 * @coord_type : Used coordinate system.
 * @local_location_coord : coord value as per coord_type.
 */
void fira_time_sync_set_local_location(struct fira_time_sync *ctx, uint16_t anchor_local_short_addr,
				       enum fira_time_sync_location_coord coord_type,
				       const uint8_t *local_location_coord);

/**
 * fira_time_sync_set_remote_location() - Set the coordinates of the Anchor to
 * synchronize with.
 * @ctx : Time synchronisation context involved.
 * @anchor_remote_short_addr : Short address of the Anchor to synchronize with.
 * @coord_type : Used coordinate system.
 * @remote_location_coord : coord value as per coord_type.
 */
void fira_time_sync_set_remote_location(struct fira_time_sync *ctx,
					uint16_t anchor_remote_short_addr,
					enum fira_time_sync_location_coord coord_type,
					const uint8_t *remote_location_coord);

/**
 * fira_time_sync_set_tof() - Set the Time of Flight directly received from
 * the DT-Anchor Initiator.
 * @ctx : Time synchronisation context involved.
 * @tof_rctu : Time of Flight with the DT-Anchor Initiator in rctu unit.
 */
void fira_time_sync_set_tof(struct fira_time_sync *ctx, uint16_t tof_rctu);

/**
 * fira_time_sync_convert_local_to_common_time() - Convert a local time into
 * a common time.
 * @ctx : Time synchronisation context involved.
 * @local_time_rctu : The time in local time base in rctu unit for which the
 * conversion is requested.
 * @common_time_rctu : Pointer on the conversion in the common time base in
 * rctu unit.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_time_sync_convert_local_to_common_time(struct fira_time_sync *ctx,
						      uint64_t local_time_rctu,
						      uint64_t *common_time_rctu);

#ifdef __cplusplus
}
#endif
