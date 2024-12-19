/**
 * @file      HAL_sleep.h
 *
 * @brief     Header for sleep functionalities
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#ifndef HAL_SLEEP_H
#define HAL_SLEEP_H

#include <stdint.h>

void deca_sleep(unsigned int time_ms);
void deca_usleep(unsigned long time_us);
void usleep(unsigned long time_us);

#endif
