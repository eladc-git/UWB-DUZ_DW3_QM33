/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define time_after(a, b) ((int64_t)((b) - (a)) < 0)

#define time_is_before_jiffies(a) time_after(jiffies, a)

extern uint64_t jiffies;

#ifdef __cplusplus
}
#endif
