/**
 * @file      circular_buffers.h
 *
 * @brief     Interface for circular buffers
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdint.h>

#ifdef TINY_BUILD
/* Read buffer for UART reception, shall be 1<<X. */
#define UART_RX_BUF_SIZE 0x200
#else
/* Read buffer for UART reception, shall be 1<<X. */
#define UART_RX_BUF_SIZE 0x800
#endif

#ifndef CIRC_CNT
/* Return count in buffer.  */
#define CIRC_CNT(head, tail, size) (((head) - (tail)) & ((size)-1))
#endif

#ifndef CIRC_SPACE
/* Return space available, 0..size-1. */
#define CIRC_SPACE(head, tail, size) CIRC_CNT((tail), ((head) + 1), (size))
#endif

#ifdef UART_RX_BUF_SIZE
struct data_circ_buf_s
{
    uint16_t head;
    uint16_t tail;
    uint8_t buf[UART_RX_BUF_SIZE];
};

typedef struct data_circ_buf_s data_circ_buf_t;
#endif
