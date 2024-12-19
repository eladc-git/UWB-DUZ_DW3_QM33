/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

struct fira_session;
struct fbs_session_demand;

/**
 * fira_hus_access_entering() - Entering in the HUS access.
 * @session: FiRa session context.
 * @fsd: FiRa Session Demand result from the get_demand of the session.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_hus_access_entering(struct fira_session *session,
				   const struct fbs_session_demand *fsd);
