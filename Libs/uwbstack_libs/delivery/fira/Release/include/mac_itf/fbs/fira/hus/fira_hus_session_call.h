/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fira/fira_region.h"
#include "net/netlink.h"

/**
 * fira_hus_session_call_set_parameters() - Set FiRa HUS session parameters.
 * @session: FiRa session context.
 * @params: Netlink attribute containing the parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
int fira_hus_session_call_set_parameters(struct fira_session *session, const struct nlattr *params,
					 const struct genl_info *info);

/**
 * fira_hus_session_call_get_parameters() - Get FiRa HUS session parameters.
 * @session: FiRa session context.
 *
 * Return: QERR_SUCCESS or error.
 */
int fira_hus_session_call_get_parameters(const struct fira_session *session);

/**
 * fira_hus_session_call_set_hus_controller_config() - Set HUS controller configuration.
 * @session: FiRa session context.
 * @call_attrs: Netlink attribute containing the parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
int fira_hus_session_call_set_hus_controller_config(struct fira_session *session,
						    struct nlattr **call_attrs,
						    const struct genl_info *info);

/**
 * fira_hus_session_call_set_hus_controlee_config() - Set HUS controlee configuration.
 * @session: FiRa session context.
 * @call_attrs: Netlink attribute containing the parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
int fira_hus_session_call_set_hus_controlee_config(struct fira_session *session,
						   struct nlattr **call_attrs,
						   const struct genl_info *info);

/**
 * fira_hus_session_call_start() - Start HUS session.
 * @session: FiRa session context.
 *
 * Return: QERR_SUCCESS or error.
 */
int fira_hus_session_call_start(struct fira_session *session);
