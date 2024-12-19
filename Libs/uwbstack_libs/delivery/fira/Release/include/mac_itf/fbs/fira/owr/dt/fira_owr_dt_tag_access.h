/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154_schedule.h"
#include "qerr.h"

#include <stdbool.h>

struct fira_session;
struct fbs_session_demand;

/**
 * fira_owr_dt_tag_access_entering() - Entering in the OWR DT Tag access.
 * @session: FiRa session context.
 * @fsd: FiRa Session Demand result from the get_demand of the session.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_owr_dt_tag_access_entering(struct fira_session *session,
					  const struct fbs_session_demand *fsd);

void fira_owr_dt_tag_access_done(struct fira_session *session,
				 enum mcps802154_access_done_reason reason, bool report_allowed);
