/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154_schedule.h"
#include "qtils.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct mcps802154_local;

/**
 * struct mcps802154_schedule_region - Region as defined in the schedule.
 */
struct mcps802154_schedule_region {
	/**
	 * @region: Pointer to the open region.
	 */
	struct mcps802154_region *region;
	/**
	 * @start_dtu: Region start from the start of the schedule.
	 */
	int start_dtu;
	/**
	 * @duration_dtu: Region duration or 0 for endless region.
	 */
	int duration_dtu;
	/**
	 * @once: Schedule the region once, ignoring the remaining region
	 * duration.
	 */
	bool once;
};

/**
 * struct mcps802154_schedule - Schedule.
 */
struct mcps802154_schedule {
	/**
	 * @start_timestamp_dtu: Date of the schedule start, might be too far in
	 * the past for endless schedule.
	 */
	uint32_t start_timestamp_dtu;
	/**
	 * @duration_dtu: Schedule duration or 0 for endless schedule.
	 */
	int duration_dtu;
	/**
	 * @regions: Table of regions.
	 */
	struct mcps802154_schedule_region *regions;
	/**
	 * @n_regions: Number of regions in the schedule.
	 */
	size_t n_regions;
	/**
	 * @current_index: Index of the current region.
	 */
	int current_index;
};

/**
 * struct mcps802154_schedule_update_local - Private part of a schedule
 * update context.
 */
struct mcps802154_schedule_update_local {
	/**
	 * @schedule_update: Public part.
	 */
	struct mcps802154_schedule_update schedule_update;
	/**
	 * @local: MCPS private data.
	 */
	struct mcps802154_local *local;
};

static inline struct mcps802154_schedule_update_local *
schedule_update_to_local(const struct mcps802154_schedule_update *schedule_update)
{
	return qparent_of(schedule_update, struct mcps802154_schedule_update_local,
			  schedule_update);
}

/**
 * mcps802154_schedule_clear() - Clear schedule and release regions.
 * @local: MCPS private data.
 */
void mcps802154_schedule_clear(struct mcps802154_local *local);

/**
 * mcps802154_schedule_update() - Initialize or update the schedule.
 * @local: MCPS private data.
 * @next_timestamp_dtu: Date of next access opportunity.
 *
 * Request the scheduler to update the schedule.
 *
 * Return: 1 or error.
 */
int mcps802154_schedule_update(struct mcps802154_local *local, uint32_t next_timestamp_dtu);
