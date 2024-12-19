/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/fbs_region_nl.h"

struct fbs_region;
struct nlattr;
struct genl_info;

/**
 * fbs_session_call() - Handle all commands related to a session.
 * @local: FBS region context.
 * @call_id: Request type identifier.
 * @attrs: Netlink attributes array related to the "call".
 * @info: Netlink extra info with the "cookie"/"socket id" inside.
 *
 * Return: Zero or positive value on success, or negative value on error.
 */
int fbs_session_call(struct fbs_region *local, enum fbs_call call_id, const struct nlattr *attrs,
		     const struct genl_info *info);
