/**
 * @file      usb_uart_rx.h
 *
 * @brief     Interface for usb_uart_rx
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "app.h"

#ifdef __cplusplus
extern "C" {
#endif

usb_data_e usb_uart_rx(void);
void usb_rx_callback_circular_buf(uint8_t *ptr, size_t len);

#ifdef __cplusplus
}
#endif
