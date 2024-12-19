/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * enum radar_preamble_duration - Duration of preamble in symbols.
 * @RADAR_PREAMBLE_DURATION_32: 32 symbols duration.
 * @RADAR_PREAMBLE_DURATION_64: 64 symbols duration.
 * @RADAR_PREAMBLE_DURATION_128: 128 symbols duration.
 * @RADAR_PREAMBLE_DURATION_256: 256 symbols duration.
 * @RADAR_PREAMBLE_DURATION_512: 512 symbols duration.
 * @RADAR_PREAMBLE_DURATION_1024: 1024 symbols duration.
 * @RADAR_PREAMBLE_DURATION_2048: 2048 symbols duration.
 * @RADAR_PREAMBLE_DURATION_4096: 4096 symbols duration.
 */
enum radar_preamble_duration {
	RADAR_PREAMBLE_DURATION_32,
	RADAR_PREAMBLE_DURATION_64,
	RADAR_PREAMBLE_DURATION_128,
	RADAR_PREAMBLE_DURATION_256,
	RADAR_PREAMBLE_DURATION_512,
	RADAR_PREAMBLE_DURATION_1024,
	RADAR_PREAMBLE_DURATION_2048,
	RADAR_PREAMBLE_DURATION_4096,
};

/**
 * enum radar_channel_number - Channel number.
 *
 * @RADAR_CHANNEL_NUMBER_5: Use channel number 5.
 * @RADAR_CHANNEL_NUMBER_9: Use channel number 9.
 */
enum radar_channel_number {
	RADAR_CHANNEL_NUMBER_5 = 5,
	RADAR_CHANNEL_NUMBER_9 = 9,
};

/**
 * enum radar_data_type - radar data type.
 *
 * @RADAR_DATA_TYPE_SWEEP_SAMPLES: Sweep samples.
 */
enum radar_data_type {
	RADAR_DATA_TYPE_SWEEP_SAMPLES = 0x00,
};

/**
 * enum radar_rframe_config - Rframe configuration used to transmit/receive
 * frames.
 *
 * @RADAR_RFRAME_CONFIG_SP0: Use SP0 mode.
 * @RADAR_RFRAME_CONFIG_SP1: Use SP1 mode.
 * @RADAR_RFRAME_CONFIG_SP3: Use SP3 mode.
 */
enum radar_rframe_config {
	RADAR_RFRAME_CONFIG_SP0 = 0,
	RADAR_RFRAME_CONFIG_SP1 = 1,
	RADAR_RFRAME_CONFIG_SP3 = 3,
};

/**
 * enum radar_tx_profile - Indexes for different TX profiles
 * profiles numbers are used directly as an indexes for the lut_tx_profile so
 * they need to be continues, non-decreasing and starting from 0
 *
 * @RADAR_TX_PROFILE_HIGH: TX profile for High radar range
 * @RADAR_TX_PROFILE_LOW: TX profile for Low radar range
 * @__RADAR_TX_PROFILE_AFTER_LAST: Biggest defined profile index
 */
enum radar_tx_profile {
	RADAR_TX_PROFILE_HIGH,
	RADAR_TX_PROFILE_LOW,

	__RADAR_TX_PROFILE_AFTER_LAST,
};

/**
 * enum radar_bytes_per_sample - Bytes per sample configuration.
 *
 * @RADAR_BYTES_PER_SAMPLE_4: Use 4 Bytes.
 * @RADAR_BYTES_PER_SAMPLE_6: Use 6 Bytes.
 * @RADAR_BYTES_PER_SAMPLE_8: Use 8 Bytes.
 */
enum radar_bytes_per_sample {
	RADAR_BYTES_PER_SAMPLE_4 = 4,
	RADAR_BYTES_PER_SAMPLE_6 = 6,
	RADAR_BYTES_PER_SAMPLE_8 = 8,
};

enum radar_ranging_status {
	RADAR_RANGING_SUCCESS,
	RADAR_RANGING_FAILED,
};

/**
 * enum radar_support_capability - supported radar features as capability.
 *
 * @RADAR_CAPABILITY_SWEEP_SAMPLES_SUPPORTED: Radar produces sweep samples.
 */
enum radar_support_capability {
	RADAR_CAPABILITY_SWEEP_SAMPLES_SUPPORTED = (1 << 0),
};

#ifdef __cplusplus
}
#endif
