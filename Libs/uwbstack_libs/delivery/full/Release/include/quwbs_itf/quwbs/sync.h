/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "quwbs/err.h"

#ifdef __cplusplus
extern "C" {
#endif

struct quwbs;

/**
 * quwbs_sync_handle_events_timeout() - Handle any pending events with timeout.
 * @ctx: QUWBS context.
 * @timeout_ns: Maximum time to wait for event in nanosecond, infinite if 0.
 *
 * This function blocks waiting for events until at least one event can be handled or timeout
 * expires.
 *
 * If the platform supports it, it also returns when a signal is received.
 *
 * This can trigger calling of registered callbacks.
 *
 * Returns: QUWBS_ERR_SUCCESS or error.
 */
enum quwbs_err quwbs_sync_handle_events_timeout(struct quwbs *ctx, unsigned int timeout_ns);

/**
 * quwbs_sync_handle_events() - Handle any pending events, forever.
 * @ctx: QUWBS context.
 *
 * This function blocks waiting for events and handle them as they come. It only returns on error
 *
 * If the platform supports it, it also returns when a signal is received.
 *
 * This can trigger calling of registered callbacks.
 *
 * Returns: error.
 */
enum quwbs_err quwbs_sync_handle_events(struct quwbs *ctx);

#ifdef __cplusplus
}
#endif
