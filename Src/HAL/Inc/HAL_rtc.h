/**
 * @file      HAL_rtc.h
 *
 * @brief     Interface for HAL_rtc
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdint.h>

struct hal_rtc_s
{
    void (*init)(void);
    void (*deinit)(void);
    void (*enableIRQ)(void);
    void (*disableIRQ)(void);
    void (*setPriorityIRQ)(void);
    uint32_t (*getTimestamp)(void);
    void (*getTime)(uint32_t *p_sec, uint32_t *p_subsec, uint32_t *p_rtc_subsec_hz);
    void (*getTimeNonBlocking)(uint32_t *p_sec, uint32_t *p_subsec, uint32_t *p_rtc_subsec_hz);
    uint32_t (*getTimeElapsed)(uint32_t start, uint32_t stop);
    void (*reload)(uint32_t time);
    void (*configureWakeup_ms)(uint32_t period_ms);
    void (*configureWakeup_ns)(uint32_t period_ns);
    float (*getWakeupResolution_ns)(void);
    void (*setCallback)(void (*cb)(void));
    void (*WakeupStop)(void);
};

extern const struct hal_rtc_s Rtc;
