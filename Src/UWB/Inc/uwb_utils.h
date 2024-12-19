/**
 * @file      uwb_utils.h
 *
 * @brief     Common functionalities for UWB device.
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

bool uwb_device_is_aoa(uint32_t device_id);
