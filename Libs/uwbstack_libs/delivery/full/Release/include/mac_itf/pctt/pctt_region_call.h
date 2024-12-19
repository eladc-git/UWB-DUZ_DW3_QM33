/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

struct genl_info;
struct nlattr;
struct pctt_local;

/**
 * pctt_call_session_get_state() - Return current state on netlink reply.
 * @local: PCTT region context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_call_session_get_state(struct pctt_local *local);

/**
 * pctt_call_session_get_params() - Return session parameters on netlink reply.
 * @local: PCTT region context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_call_session_get_params(struct pctt_local *local);

/**
 * pctt_call_session_set_params() - Set session parameters on netlink request.
 * @local: PCTT region context.
 * @params: Nested attribute containing procedure parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_call_session_set_params(struct pctt_local *local, const struct nlattr *params,
				       const struct genl_info *info);

/**
 * pctt_call_session_cmd() - PCTT specific region call
 * @local: PCTT region context.
 * @params: Nested attribute containing procedure parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_call_session_cmd(struct pctt_local *local, const struct nlattr *params,
				const struct genl_info *info);
