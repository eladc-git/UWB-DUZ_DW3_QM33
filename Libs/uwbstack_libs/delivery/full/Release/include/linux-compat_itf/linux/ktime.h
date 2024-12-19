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

typedef int64_t ktime_t;

/* Convert ktime_t to nanoseconds */
static inline int64_t ktime_to_ns(const ktime_t kt)
{
	return kt;
}

/* Convert ktime_t to milliseconds */
static inline int64_t ktime_to_ms(const ktime_t kt)
{
	return kt / 1000000;
}

#ifdef __cplusplus
}
#endif
