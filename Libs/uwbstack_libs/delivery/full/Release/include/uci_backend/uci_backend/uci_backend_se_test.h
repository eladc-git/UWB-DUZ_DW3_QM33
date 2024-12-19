/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <uci/uci.h>

#if !defined(CONFIG_SE)
/**
 * enum uci_application_se_parameters - Secure element parameters.
 *
 * @UCI_APPLICATION_SE_URSK:
 *     Set ranging session key.
 * @UCI_APPLICATION_SE_SESSION_ID:
 *     Set session id.
 * @UCI_APPLICATION_SE_ERROR:
 *     Set status code.
 */
enum uci_application_se_parameters {
	UCI_APPLICATION_SE_URSK = 0xA0,
	UCI_APPLICATION_SE_SESSION_ID = 0xA2,
	UCI_APPLICATION_SE_ERROR = 0xA3,
};

/**
 * uci_backend_se_test_init() - Attach this backend to the uci context to bridge
 * uci communication to the key_manager for testing purpose.
 * @uci: UCI context.
 *
 * Return: QERR_SUCCESS or error.
 */
void uci_backend_se_test_init(struct uci *uci);
#else
static inline void uci_backend_se_test_init(struct uci *uci)
{
}
#endif
