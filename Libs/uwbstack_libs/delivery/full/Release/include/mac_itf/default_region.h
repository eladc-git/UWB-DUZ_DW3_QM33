/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "linux/skbuff.h"
#include "net/mcps802154_schedule.h"
#include "qatomic.h"
#include "qerr.h"
#include "qtils.h"

/**
 * struct default_local - Local context.
 */
struct default_local {
	/**
	 * @region: Region instance returned to MCPS.
	 */
	struct mcps802154_region region;
	/**
	 * @llhw: Low-level device pointer.
	 */
	struct mcps802154_llhw *llhw;
	/**
	 * @access: Access returned to MCPS.
	 */
	struct mcps802154_access access;
	/**
	 * @queue: Queue of frames to be transmitted.
	 */
	struct sk_buff_head queue;
	/**
	 * @n_queued: Number of queued frames. This also includes frame being
	 * transmitted which is no longer in &queue.
	 */
	qatomic_int n_queued;
	/**
	 * @retries: Number of retries done on the current tx frame.
	 */
	int retries;
};

static inline struct default_local *region_to_local(struct mcps802154_region *region)
{
	return qparent_of(region, struct default_local, region);
}

static inline struct default_local *access_to_local(struct mcps802154_access *access)
{
	return qparent_of(access, struct default_local, access);
}

enum qerr mcps802154_default_region_init(void);
void mcps802154_default_region_exit(void);
