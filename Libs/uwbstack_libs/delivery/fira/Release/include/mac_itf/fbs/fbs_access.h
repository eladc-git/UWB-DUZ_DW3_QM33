/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154_schedule.h"

struct fbs_region;
struct fbs_session;
struct fbs_session_demand;

/**
 * fbs_access_get_access() - Region access start for all fbs sessions.
 * @local: FBS region context.
 * @fbs_session: FBS session context.
 * @fsd: Session demand to forward to the session.
 *
 * Return: Valid or NULL pointer to the access.
 */
struct mcps802154_access *fbs_access_get_access(struct fbs_region *local,
						struct fbs_session *fbs_session,
						const struct fbs_session_demand *fsd);

/**
 * fbs_access_done() - Region access done which must be used by sessions.
 * @access: The access which is over (same as the &fbs_region.access)
 * @reason: How fproc put an end to access, the reason.
 *
 * Note: The region propagates the access_done event to the right session.
 */
void fbs_access_done(struct mcps802154_access *access, enum mcps802154_access_done_reason reason);
