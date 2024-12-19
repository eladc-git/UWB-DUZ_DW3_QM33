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
 * fira_hus_init() - Registers FiRa HUS protocol in the MAC.
 *
 * The protocol have to be register once before to be used.
 * Once registered, the protocol is ready to be instanciated.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr fira_hus_init(void);

/**
 * fira_hus_exit() - Unregisters the FiRa HUS protocol.
 *
 * All instanciations of this protocol can continue to run. Only new
 * instantiations of this protocol are no longer possible.
 */
enum qerr fira_hus_exit(void);

#ifdef __cplusplus
}
#endif
