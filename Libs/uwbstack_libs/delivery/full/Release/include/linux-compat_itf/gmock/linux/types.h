/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
#include_next <linux/types.h>
#else
typedef uint16_t __le16;
typedef uint32_t __le32;
typedef uint64_t __le64;
#endif

#ifdef __cplusplus
}
#endif
