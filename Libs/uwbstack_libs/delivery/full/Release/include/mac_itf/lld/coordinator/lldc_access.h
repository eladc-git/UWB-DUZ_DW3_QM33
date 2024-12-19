/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

/* Forward declaration. */
struct lldc_local;
struct work_struct;
struct lldc_ranging_report;
struct mcps802154_region;
struct mcps802154_region_demand;

/**
 * lldc_get_demand() - Get next region access.
 * @region: Region.
 * @next_timestamp_dtu: Next access opportunity.
 * @max_duration_dtu: Max duration of the next access opportunity.
 * @demand: Access information to set.
 *
 * Return: 1 if data is available, 0 if no demand (do not want access) or error.
 */
int lldc_get_demand(struct mcps802154_region *region, uint32_t next_timestamp_dtu,
		    int max_duration_dtu, struct mcps802154_region_demand *demand);

/**
 * lldc_get_access() - Get access for a given region at the given timestamp.
 * @region: Region.
 * @next_timestamp_dtu: Next access opportunity.
 * @next_in_region_dtu: Unused.
 * @region_duration_dtu: Unused.
 *
 * Return: The access.
 */
struct mcps802154_access *lldc_get_access(struct mcps802154_region *region,
					  uint32_t next_timestamp_dtu, int next_in_region_dtu,
					  int region_duration_dtu);

/**
 * lldc_get_add_blocks() - Get number of block to add.
 * @local: Local context.
 * @next_timestamp_dtu: Next access opportunity.
 *
 * Return: Number of block to add.
 */
int lldc_get_add_blocks(const struct lldc_local *local, uint32_t next_timestamp_dtu);

/**
 * lldc_update() - Update current access.
 * @local: Local context.
 * @next_timestamp_dtu: Next access opportunity.
 */
void lldc_update(struct lldc_local *local, uint32_t next_timestamp_dtu);

/**
 * lldc_subaccess_subblock() - Prepare a sub-block access.
 * @local: Local context.
 */
void lldc_subaccess_subblock(struct lldc_local *local);

/**
 * lldc_rx_frame_payload_event() - Process receveid frame payloads.
 * @work: Work struct.
 */
void lldc_rx_frame_payload_event(struct work_struct *work);

/**
 * lldc_prepare_ranging_block() - Prepare ranging information for the current
 * block.
 * @local: Local context.
 */
void lldc_prepare_ranging_block(struct lldc_local *local);

/**
 * lldc_allocate_ranging_report() - Allocate ranging report information.
 * @local: Local context.
 * @p: Pointer to the ranging report information.
 * @n_elem: Number of elements to allocate.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_allocate_ranging_report(struct lldc_local *local, struct lldc_ranging_report *p,
				       int n_elem);
