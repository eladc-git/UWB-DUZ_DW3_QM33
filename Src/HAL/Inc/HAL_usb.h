/**
 * @file      HAL_usb.h
 *
 * @brief     Interface for HAL_usb
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdbool.h>

struct hal_usb_s
{
    void (*init)(void (*rx_callback)(uint8_t *ptr, size_t len));
    void (*deinit)(void);
    bool (*transmit)(uint8_t *tx_buffer, int size);
    void (*receive)(void);
    void (*update)(void);
    bool (*isTxBufferEmpty)(void);
};

extern const struct hal_usb_s Usb;
