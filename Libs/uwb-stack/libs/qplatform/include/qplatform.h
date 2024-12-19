/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-Qorvo-2
 */

#pragma once

#include <qerr.h>

/**
 * qplatform_init() - Initialize the platform.
 *
 * Initialize what is platform specific in the system.
 * It should be called prior to any other init of the UWB stack.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr qplatform_init(void);

/**
 * qplatform_deinit() - Denitialize the platform.
 *
 * Deinitialize what is platform specific in the system.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr qplatform_deinit(void);

/**
 * qplatform_uwb_interrupt_enable() - Enable interrupts for the UWB subsystem.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr qplatform_uwb_interrupt_enable(void);

/**
 * qplatform_uwb_interrupt_disable() - Disable interrupts for the UWB subsystem.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr qplatform_uwb_interrupt_disable(void);

/**
 * qplatform_uwb_spi_set_fast_rate_freq() - Configure fast rate frequency for SPI used for the UWB
 * communication, if applicable.
 */
void qplatform_uwb_spi_set_fast_rate_freq(void);

/**
 * qplatform_uwb_spi_set_slow_rate_freq() - Configure slow rate frequency for SPI used for the UWB
 * communication, if applicable.
 */
void qplatform_uwb_spi_set_slow_rate_freq(void);
