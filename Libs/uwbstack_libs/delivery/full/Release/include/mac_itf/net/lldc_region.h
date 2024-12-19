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
 * lldc_region_init() - Initialize LLDC region.
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_region_init(void);

/**
 * lldc_region_exit() - Leave LLDC region.
 */
void lldc_region_exit(void);

#ifdef __cplusplus
}
#endif
