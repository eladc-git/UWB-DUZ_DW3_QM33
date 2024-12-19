/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <uwbmac/uwbmac_msg.h>

/**
 * uwbmac_parser_get_nla() - Get current nla.
 * @msg: message in its current reading state
 * @rem: in/out parameter indicating remaining size to parse in message.
 *
 * Parameter `rem` should be updated by function only when nla retrieval may
 * affect it, i.e. in case of fragmented buffer.
 *
 * Return: QERR_SUCCESS on success, or error.
 */
struct nlattr *uwbmac_parser_get_nla(struct uwbmac_msg *msg, int *rem);

/**
 * uwbmac_parser_get_next_data() - Get next nla.
 * @msg: original message where nla is.
 * @nla: current nla.
 * @rem: remaining data.
 *
 * Return: next nla address or NULL if error.
 */
void *uwbmac_parser_get_next_data(struct uwbmac_msg *msg, const void *nla, int *rem);
