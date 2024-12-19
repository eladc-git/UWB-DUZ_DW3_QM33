/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <linux/ktime.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

ktime_t ktime_get_boottime(void);

#ifdef __cplusplus
}
#endif
