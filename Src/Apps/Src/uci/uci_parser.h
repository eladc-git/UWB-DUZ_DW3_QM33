/**
 * @file      uci_parser.h
 *
 * @brief     Interface for UCI parser
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdint.h>

#include "app.h"

usb_data_e cir_to_ser_buf(uint8_t *pbuf, uint16_t len, uint16_t *read_offset, uint16_t cyclic_size);
