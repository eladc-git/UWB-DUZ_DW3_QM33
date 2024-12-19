/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <cbor/cbor.h>
#include <uwbmac/uwbmac_msg.h>

#ifdef __cplusplus
extern "C" {
#endif

enum qerr uwbmac_msg_serialize(struct uwbmac_msg *msg, struct cbor_state *cbor_state);

enum qerr uwbmac_msg_unserialize(struct uwbmac_msg *msg, struct cbor_state *cbor_state);

#ifdef __cplusplus
}
#endif
