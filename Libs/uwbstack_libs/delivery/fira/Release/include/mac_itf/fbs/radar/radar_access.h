/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_session.h"
#include "net/mcps802154_schedule.h"
#include "radar_session.h"

#include <stdbool.h>
struct fbs_session_demand;

static inline struct radar_session *access_to_radar(struct mcps802154_access *access)
{
	return fbs_to_radar(access_to_session(access));
}

/**
 * radar_access_get() - Start the radar access granted.
 * @session: Radar session context.
 * @fsd: FBS session demand to apply.
 *
 * Return: Access pointer to use.
 */
struct mcps802154_access *radar_access_get(struct radar_session *session,
					   const struct fbs_session_demand *fsd);

/**
 * radar_access_done() - End of the access.
 * @fbs_session: FBS session context.
 * @reason: Global feedback about how was the access.
 * @report_allowed: True when the session can send ranging report.
 */
void radar_access_done(struct fbs_session *fbs_session, enum mcps802154_access_done_reason reason,
		       bool report_allowed);

/**
 * radar_access_cleanup() - Disables the PLL the pre-buffer in the LLHW.
 * @session: Radar session context.
 */
void radar_access_cleanup(struct radar_session *session);
