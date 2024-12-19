/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qerr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * radar_init() - Registers Radar protocol in the MAC.
 *
 * The protocol have to be register once before to be used.
 * Once registered, the protocol is ready to be instanciated.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr radar_init(void);

/**
 * radar_exit() - Unregisters the Radar protocol.
 *
 * All instanciations of this protocol can continue to run. Only new
 * instantiations of this protocol are no longer possible.
 */
void radar_exit(void);

#ifdef __cplusplus
}
#endif
