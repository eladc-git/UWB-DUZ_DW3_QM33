/**
 * @file      HAL_button.h
 *
 * @brief     Interface for HAL_button
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdbool.h>

/**
 * @brief Check if the button is pressed.
 * Check parameters and send/receive through T1 protocol.
 *
 * @return true is the button is pressed
 */
bool isButtonPressed(void);

/** @brief Configure button input. */
void configure_button(void);
