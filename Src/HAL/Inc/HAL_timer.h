/**
 * @file      HAL_timer.h
 *
 * @brief     Interface for HAL_timer
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

struct hal_fs_timer_s
{
    /* Timer instance. */
    void *timer;

    /* Timer external ISR. */
    void (*timerIsr)(void *dwchip);

    /**
     * @brief Initialization/uninit of the Fast Sleep Timer
     * @param cb The "timerIsr"
     */
    void (*init)(void *self, void *dwchip, void (*cb)(void *));

    /** @brief De-Initialization/uninit of the Fast Sleep Timer. */
    void (*deinit)(void *self);

    /**
     * @brief  start the timer
     * @param int_en True/False to generate an interrupt on counter compare event.
     * @param time_us Time to wait in microseconds.
     * @param corr_tick The number of timer's ticks it needs to start the counting from.
     */
    void (*start)(void *self, bool int_en, uint32_t next_cc_us, int corr_tick);

    /**
     * @brief stop the timer
     */
    void (*stop)(void *self);

    /**
     * @brief Returns the fast sleep timer current tick value.
     * This will be the number of ticks since its last reset (since last CC event)
     */
    uint32_t const (*get_tick)(void *self);

    /* Timer tick frequency. */
    int freq;
};

typedef struct hal_fs_timer_s hal_fs_timer_t;

/* Public fast sleep timer. */
extern hal_fs_timer_t hal_fs_timer;

struct hal_timer_s
{
    uint32_t (*init)(void);
    void (*start)(volatile uint32_t *p_timestamp);
    bool (*check)(uint32_t timestamp, uint32_t time);
};

extern const struct hal_timer_s Timer;
