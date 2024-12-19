/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "lld/lld_mfs.h"

/**
 * struct lldd_stats - Device stats.
 */
struct lldd_stats {
	/**
	 * @mfs: MFS packet statistics.
	 */
	struct lld_mfs_stats mfs[LLD_MFS_ID_NB];
	/**
	 * @rx_dl_frames: Number of good DL frames received.
	 */
	uint32_t rx_dl_frames;
	/**
	 * @rx_dl_bytes: Number of good DL frames bytes received, FCS not
	 * included, corresponding to rx_dl_frames.
	 */
	uint32_t rx_dl_bytes;
	/**
	 * @rx_dl_errors: Number of DL frames received with error, or not
	 * received.
	 */
	uint32_t rx_dl_errors;
	/**
	 * @rx_dl_aux_frames: Number of good DL frames received in an auxiliary
	 * sub-block.
	 */
	uint32_t rx_dl_aux_frames;
	/**
	 * @rx_dl_aux_bytes: Number of good DL frames bytes received in an
	 * auxiliary sub-block, FCS not included, corresponding to
	 * rx_dl_aux_frames.
	 */
	uint32_t rx_dl_aux_bytes;
	/**
	 * @rx_dl_aux_errors: Number of DL frames received with error in an
	 * auxiliary sub-block, or not received.
	 */
	uint32_t rx_dl_aux_errors;
	/**
	 * @tx_ul_frames: Number of UL frames transmitted successfully
	 * (acknowledged successfully or no acknowledgement required).
	 */
	uint32_t tx_ul_frames;
	/**
	 * @tx_ul_bytes: Number of UL frames bytes transmitted successfully,
	 * corresponding to tx_ul_frames.
	 */
	uint32_t tx_ul_bytes;
	/**
	 * @tx_ul_errors: Number of UL frames transmitted with error.
	 */
	uint32_t tx_ul_errors;
	/**
	 * @rx_beacon_frames: Number of beacon frames received.
	 */
	uint32_t rx_beacon_frames;
	/**
	 * @rx_beacon_errors: Number of beacon frames received with errors.
	 */
	uint32_t rx_beacon_errors;
	/**
	 * @rx_unrelateds: Number of unrelated frames received.
	 */
	uint32_t rx_unrelateds;
	/**
	 * @rx_frame_non_assocs: Number of non associative frames received.
	 */
	uint32_t rx_frame_non_assocs;
	/**
	 * @rx_frame_non_assocs_errors: Number of non associative frames errors.
	 */
	uint32_t rx_frame_non_assocs_errors;
	/**
	 * @blocks: Number of blocks in which the device participated (tracking
	 * or associated).
	 */
	uint32_t blocks;
	/**
	 * @lqi: Link quality indicator (LQI, 0-100).
	 */
	uint8_t lqi;
};
