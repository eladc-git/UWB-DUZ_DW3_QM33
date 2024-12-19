/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

/*
 * This file is generated automatically, edit with care.
 */

#pragma once

#include "uwbmac/uwbmac.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * struct lld_mfs_stats - LLD MFS statistics.
 */
struct lld_mfs_stats {
	/**
	 * @rx_packets: Number of good data packets received.
	 */
	uint32_t rx_packets;
	/**
	 * @rx_bytes: Number of good received bytes, corresponding to
	 * rx_packets.
	 */
	uint32_t rx_bytes;
	/**
	 * @rx_errors: Number of bad data packets received.
	 */
	uint32_t rx_errors;
	/**
	 * @rx_ignored: Number of good data packets ignored because
	 * they are out of the reception window.
	 */
	uint32_t rx_ignored;
	/**
	 * @tx_packets: Number of data packets successfully transmitted
	 * (acknowledged successfully or no acknowledgement required).
	 */
	uint32_t tx_packets;
	/**
	 * @tx_bytes: Number of transmitted bytes, corresponding to
	 * tx_packet.
	 */
	uint32_t tx_bytes;
	/**
	 * @tx_errors: Number of data packets not transmitted due to problem
	 * with the packet.
	 */
	uint32_t tx_errors;
	/**
	 * @tx_dropped: Number of data packets dropped before successful
	 * transmission.
	 */
	uint32_t tx_dropped;
};

#ifdef __cplusplus
}
#endif
