/**
 * @file      HAL_button.c
 *
 * @brief     HAL functions for button
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include "boards.h"
#include "HAL_button.h"
#include "HAL_gpio.h"
#include "nrf_gpio.h"

bool isButtonPressed(void)
{
    return (nrf_gpio_pin_read(BUTTON_1) == GPIO_PIN_RESET);
}
