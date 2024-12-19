/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

/* File to include by other trace file, to avoid to redefine common items. */

#pragma once

#include "linux/tracepoint.h"
#include "net/mcps802154.h"

/* clang-format off */

#define mcps802154_rx_error_name(name)                                       \
	{ MCPS802154_RX_ERROR_##name, #name }
#define MCPS802154_RX_ERROR_NAMES                                            \
	mcps802154_rx_error_name(NONE),                                      \
	mcps802154_rx_error_name(TIMEOUT),                                   \
	mcps802154_rx_error_name(BAD_CKSUM),                                 \
	mcps802154_rx_error_name(UNCORRECTABLE),                             \
	mcps802154_rx_error_name(FILTERED),                                  \
	mcps802154_rx_error_name(SFD_TIMEOUT),                               \
	mcps802154_rx_error_name(PHR_DECODE),                                \
	mcps802154_rx_error_name(OTHER)
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_NONE);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_TIMEOUT);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_BAD_CKSUM);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_UNCORRECTABLE);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_FILTERED);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_SFD_TIMEOUT);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_PHR_DECODE);
TRACE_DEFINE_ENUM(MCPS802154_RX_ERROR_OTHER);

#define mcps802154_tx_reason_name(name)                                      \
	{ MCPS802154_ACCESS_TX_RETURN_REASON_##name, #name }
#define MCPS802154_TX_REASON_NAMES                                           \
	mcps802154_tx_reason_name(CONSUMED),                                 \
	mcps802154_tx_reason_name(FAILURE),                                  \
	mcps802154_tx_reason_name(CANCEL)
TRACE_DEFINE_ENUM(MCPS802154_ACCESS_TX_RETURN_REASON_CONSUMED);
TRACE_DEFINE_ENUM(MCPS802154_ACCESS_TX_RETURN_REASON_FAILURE);
TRACE_DEFINE_ENUM(MCPS802154_ACCESS_TX_RETURN_REASON_CANCEL);

#define mcps802154_access_done_reason_name(name)                             \
	{ MCPS802154_ACCESS_DONE_REASON_##name, #name }
#define MCPS802154_ACCESS_DONE_REASON_NAMES                                  \
	mcps802154_access_done_reason_name(SUCCESS),                         \
	mcps802154_access_done_reason_name(ERROR),                           \
	mcps802154_access_done_reason_name(CANCEL)
TRACE_DEFINE_ENUM(MCPS802154_ACCESS_DONE_REASON_SUCCESS);
TRACE_DEFINE_ENUM(MCPS802154_ACCESS_DONE_REASON_ERROR);
TRACE_DEFINE_ENUM(MCPS802154_ACCESS_DONE_REASON_CANCEL);

#define print_boolean(x) ((x) ? "true" : "false")

/* clang-format on */
