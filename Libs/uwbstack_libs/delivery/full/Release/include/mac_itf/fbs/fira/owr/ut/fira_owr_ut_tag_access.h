/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154_schedule.h"
#include "qerr.h"

#include <stdbool.h>

struct fbs_session_demand;
struct fira_session;

/**
 * fira_owr_ut_tag_access_entering() - Entering in the UT-Tag access.
 * @session: FiRa session context.
 * @fsd: FiRa Session Demand result from the get_demand of the session.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_owr_ut_tag_access_entering(struct fira_session *session,
					  const struct fbs_session_demand *fsd);

/**
 * fira_owr_ut_tag_access_done() - Access done callback.
 * @session: FiRa session context.
 * @reason: Reason of access done event.
 * @report_allowed: True when report are allowed to be send.
 */
void fira_owr_ut_tag_access_done(struct fira_session *session,
				 enum mcps802154_access_done_reason reason, bool report_allowed);
