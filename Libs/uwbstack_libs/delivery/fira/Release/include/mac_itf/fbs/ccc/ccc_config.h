/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qerr.h"

struct ccc_session;
struct nlattr;
struct genl_info;

/**
 * ccc_config_init() - Initializes default configuration.
 * @session: CCC session context.
 */
void ccc_config_init(struct ccc_session *session);

/**
 * ccc_session_set_app_config() - Set ccc session parameters.
 * @session: CCC session context.
 * @params: Nested attribute containing session parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr ccc_session_set_app_config(struct ccc_session *session, const struct nlattr *params,
				     const struct genl_info *info);

/**
 * ccc_session_get_app_config() - Get CCC session configuration.
 * @session: CCC session context.
 *
 * Return: QERR_SUCCESS on success or error otherwise.
 */
enum qerr ccc_session_get_app_config(struct ccc_session *session);
