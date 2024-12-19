/**
 * @file      HAL_gpio.h
 *
 * @brief     Interface for HAL_gpio
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

/** @brief  GPIO Bit SET and Bit RESET enumeration. */
typedef enum
{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState;

struct hal_gpio_s
{
    void (*init)(int, char *, int, int);
    void (*on)(int);
    void (*off)(int);
    void (*toggle)(int);
};

extern const struct hal_gpio_s Gpio;

/* HW Debug pins */
void hw_debug_D0(void);
void hw_debug_D1(void);

#define LP_DEBUG_D0() hw_debug_D0()
#define LP_DEBUG_D1() hw_debug_D1()
