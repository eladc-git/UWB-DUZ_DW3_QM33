/**
 * @file      controlTask.h
 *
 * @brief     Interface for control task
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "circular_buffers.h"

/* Communication RX buffer size from usb_uart_rx parser to application for RX from USB/USART. */
#define COM_RX_BUF_SIZE UART_RX_BUF_SIZE
extern uint16_t local_buff_length;
extern uint8_t local_buff[COM_RX_BUF_SIZE];

void ControlTaskInit(void);
void NotifyControlTask(void);
void NotifyControlTaskStopApp(void);

#ifdef __cplusplus
}
#endif
