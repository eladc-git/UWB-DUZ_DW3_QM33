/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qerr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * lldd_region_init() - Initialize LLDD region.
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldd_region_init(void);

/**
 * lldd_region_exit() - Leave LLDD region.
 */
void lldd_region_exit(void);

#ifdef __cplusplus
}
#endif
