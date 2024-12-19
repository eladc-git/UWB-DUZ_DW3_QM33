/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154_schedule.h"
#include "qerr.h"

#include <stdint.h>

/* Forward declaration. */
struct lldd_local;
struct work_struct;
struct sk_buff;

/**
 * lldd_fsm_common_stop() - Common stop.
 * @local: Local context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldd_fsm_common_stop(struct lldd_local *local);

/**
 * lldd_fsm_common_xmit_skb() - Add skb in tx queue.
 * @local: Local context.
 * @skb: skb to add in the queue.
 *
 * Return: The access.
 */
int lldd_fsm_common_xmit_skb(struct lldd_local *local, struct sk_buff *skb);

/**
 * lldd_fsm_common_get_demand() - Update demand with next block to access.
 * @local: Local context.
 * @next_timestamp_dtu: Next access opportunity.
 * @max_duration_dtu: Max duration of the next access opportunity.
 * @demand: Demand to set with next block timestamp and max duration.
 *
 * Return: Always 1.
 */
int lldd_fsm_common_get_demand(struct lldd_local *local, uint32_t next_timestamp_dtu,
			       int max_duration_dtu, struct mcps802154_region_demand *demand);

/**
 * lldd_fsm_common_get_access() - Prepare a common access.
 * @local: Local context.
 * @next_timestamp_dtu: Next access opportunity.
 *
 * Return: The access.
 */
struct mcps802154_access *lldd_fsm_common_get_access(struct lldd_local *local,
						     uint32_t next_timestamp_dtu);

/**
 * lldd_get_access_scanning() - Prepare a scanning access to find the beacon.
 * @local: Local context.
 * @next_timestamp_dtu: Next access opportunity.
 *
 * Return: The access.
 */
struct mcps802154_access *lldd_get_access_scanning(struct lldd_local *local,
						   uint32_t next_timestamp_dtu);

/**
 * lldd_get_demand() - Get next region access.
 * @region: Region.
 * @next_timestamp_dtu: Next access opportunity.
 * @max_duration_dtu: Max duration of the next access opportunity.
 * @demand: Access information to set.
 *
 * Return: 1 if data is available, 0 if no demand (do not want access) or error.
 */
int lldd_get_demand(struct mcps802154_region *region, uint32_t next_timestamp_dtu,
		    int max_duration_dtu, struct mcps802154_region_demand *demand);

/**
 * lldd_get_access() - Get access for a given region at the given timestamp.
 * @region: Region.
 * @next_timestamp_dtu: Next access opportunity.
 * @next_in_region_dtu: Unused.
 * @region_duration_dtu: Unused.
 *
 * Return: The access.
 */
struct mcps802154_access *lldd_get_access(struct mcps802154_region *region,
					  uint32_t next_timestamp_dtu, int next_in_region_dtu,
					  int region_duration_dtu);

/**
 * lldd_fsm_common_access_done() - Common access done.
 * @local: Local context.
 * @reason: Reason of the end of access.
 */
void lldd_fsm_common_access_done(struct lldd_local *local,
				 enum mcps802154_access_done_reason reason);

/**
 * lldd_rx_frame_payload_event() - Process receveid frame payloads.
 * @work: Work struct.
 */
void lldd_rx_frame_payload_event(struct work_struct *work);
