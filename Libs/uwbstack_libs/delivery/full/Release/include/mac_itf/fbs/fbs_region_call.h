/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qerr.h"

struct fbs_region;

/**
 * fbs_region_call_get_capabilities() - Handle the GET_CAPS_INFO request.
 * @local: FBS region context.
 *
 * Return: QERR_SUCCESS when the request is handled, or an error otherwise.
 */
enum qerr fbs_region_call_get_capabilities(struct fbs_region *local);

/**
 * fbs_region_call_get_session_count() - Handle the SESSION_GET_COUNT request.
 * @local: FBS region context.
 *
 * Return: QERR_SUCCESS when the request is handled, or an error otherwise.
 */
enum qerr fbs_region_call_get_session_count(struct fbs_region *local);
