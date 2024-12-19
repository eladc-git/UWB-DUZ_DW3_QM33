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
 * fira_owr_aoa_advertiser_access_entering() - Fill the basic parameters needed
 * to start the access for the Advertiser.
 * @session: FiRa session context.
 * @fsd: FiRa Session Demand result from the get_demand of the session.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_owr_aoa_advertiser_access_entering(struct fira_session *session,
						  const struct fbs_session_demand *fsd);

/**
 * fira_owr_aoa_advertiser_access_done() - Access done callback.
 * @session: FiRa session context.
 * @reason: Reason of the access done event.
 * @report_allowed: True when report are allowed to be send.
 */
void fira_owr_aoa_advertiser_access_done(struct fira_session *session,
					 enum mcps802154_access_done_reason reason,
					 bool report_allowed);
