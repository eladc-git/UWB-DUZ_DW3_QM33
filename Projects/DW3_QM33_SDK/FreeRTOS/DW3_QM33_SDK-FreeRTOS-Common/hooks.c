/**
 * @file      hooks.c
 *
 * @brief     FreeRTOS hooks
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "FreeRTOS.h"
#include "HAL_error.h"

/* To Test Low power mode - Set configUSE_IDLE_HOOK as '1' in FreeRTOSConfig.h. */
__attribute__((weak)) void vApplicationIdleHook(void)
{
    __WFI();
}

__attribute__((weak)) void vApplicationMallocFailedHook(void)
{
    error_handler(1, _ERR_Malloc_Failed);
}

__attribute__((weak)) void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
}
