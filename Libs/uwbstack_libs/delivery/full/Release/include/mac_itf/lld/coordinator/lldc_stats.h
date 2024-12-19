/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "lld/lld_mfs.h"

/**
 * struct lldc_stats - LLD coordinator per device statistics.
 */
struct lldc_stats {
	/**
	 * @mfs: MFS packet statistics.
	 */
	struct lld_mfs_stats mfs[LLD_MFS_ID_NB];
	/**
	 * @rx_ul_frames: Number of good UL frames received.
	 */
	uint32_t rx_ul_frames;
	/**
	 * @rx_ul_bytes: Number of good UL frames bytes received, FCS not
	 * included, corresponding to rx_ul_frames.
	 */
	uint32_t rx_ul_bytes;
	/**
	 * @rx_ul_errors: Number of UL frames received with error, or not
	 * received.
	 */
	uint32_t rx_ul_errors;
	/**
	 * @rx_ul_aux_frames: Number of good UL frames received in an auxiliary
	 * sub-block.
	 */
	uint32_t rx_ul_aux_frames;
	/**
	 * @rx_ul_aux_bytes: Number of good UL frames bytes received in an
	 * auxiliary sub-block, FCS not included, corresponding to
	 * rx_ul_aux_frames.
	 */
	uint32_t rx_ul_aux_bytes;
	/**
	 * @rx_ul_aux_errors: Number of UL frames received with error in an
	 * auxiliary sub-block, or not received.
	 */
	uint32_t rx_ul_aux_errors;
	/**
	 * @blocks: Number of blocks in which the device participated (tracking
	 * or associated).
	 */
	uint32_t blocks;
	/**
	 * @lqi: Link quality indicator (LQI, 0-100) of all connected devices.
	 */
	uint8_t lqi;
};

/**
 * struct lldc_coord_stats - LLD global coordinator statistics.
 */
struct lldc_coord_stats {
	/**
	 * @tx_frame_non_assocs: Number of non associative frames successfully
	 * transmitted.
	 */
	uint32_t tx_frame_non_assocs;
	/**
	 * @tx_frame_non_assocs_errors: Number of non associative frames not
	 * transmitted.
	 */
	uint32_t tx_frame_non_assocs_errors;
	/**
	 * @tx_frame_beacon: Number of beacon frames successfully transmitted.
	 */
	uint32_t tx_frame_beacon;
	/**
	 * @tx_frame_beacon_errors: Number of beacon frames not transmitted.
	 */
	uint32_t tx_frame_beacon_errors;
	/**
	 * @tx_dl_frames: Number of DL frames successfully transmitted.
	 */
	uint32_t tx_dl_frames;
	/**
	 * @tx_dl_errors: Number of DL frames not transmitted.
	 */
	uint32_t tx_dl_errors;
	/**
	 * @tx_dl_bytes: Number of DL frames bytes transmitted
	 * successfully, corresponding to tx_dl_frames.
	 */
	uint32_t tx_dl_bytes;
	/**
	 * @stats: See &struct lldc_stats.
	 */
	struct lldc_stats stats;
};
