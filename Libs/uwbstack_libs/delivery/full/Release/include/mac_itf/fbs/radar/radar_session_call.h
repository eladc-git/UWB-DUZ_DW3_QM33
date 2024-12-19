/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/radar_region_nl.h"

struct radar_local;
struct nlattr;
struct genl_info;

/**
 * radar_config_set() - Set the session radar configuration.
 * @session: Radar session context.
 * @params: Netlink configuration to parse and maybe apply.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS on success or an error otherwise.
 */
enum qerr radar_config_set(struct radar_session *session, const struct nlattr *params,
			   const struct genl_info *info);

/**
 * radar_config_get() - Get the session radar configuration.
 * @session: Radar session context.
 *
 * Return: QERR_SUCCESS on success or an error otherwise.
 */
enum qerr radar_config_get(struct radar_session *session);
