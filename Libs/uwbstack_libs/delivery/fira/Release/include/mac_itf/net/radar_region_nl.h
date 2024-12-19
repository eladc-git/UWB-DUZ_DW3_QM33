/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/fbs_region_nl.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RADAR_CIR_WIN_MAX_LEN 128

/**
 * enum radar_capability_attrs - radar capability parameters.
 *
 * @RADAR_CAPABILITY_ATTR_RADAR_SUPPORT:
 *	Supported Radar fearures.
 * @__RADAR_CAPABILITY_ATTR_AFTER_LAST: Internal use.
 * @RADAR_CAPABILITY_ATTR_MAX: Internal use.
 */
enum radar_capability_attrs {
	RADAR_CAPABILITY_ATTR_RADAR_SUPPORT = 0xb0,

	__RADAR_CAPABILITY_ATTR_AFTER_LAST,
	RADAR_CAPABILITY_ATTR_MAX = __RADAR_CAPABILITY_ATTR_AFTER_LAST - 1
};

/**
 * enum radar_call_attrs - radar call attributes.
 *
 * @RADAR_CALL_ATTR_STATUS: Session status.
 * @RADAR_CALL_ATTR_RADAR_DATA: Radar data.
 * @__RADAR_CALL_ATTR_AFTER_LAST: Internal use.
 * @RADAR_CALL_ATTR_MAX: Internal use.
 */
enum radar_call_attrs {
	/* All FBS attributs are before Radar attributs. */
	RADAR_CALL_ATTR_STATUS = __FBS_CALL_ATTR_AFTER_LAST,
	RADAR_CALL_ATTR_RADAR_DATA,

	__RADAR_CALL_ATTR_AFTER_LAST,
	RADAR_CALL_ATTR_MAX = __RADAR_CALL_ATTR_AFTER_LAST - 1
};

/**
 * enum radar_session_param_attrs - radar session parameters attributes.
 *
 * @RADAR_SESSION_PARAM_ATTR_CHANNEL_NUMBER:
 *	Channel number
 * @RADAR_SESSION_PARAM_ATTR_RFRAME_CONFIG:
 *	Rframe config.
 * @RADAR_SESSION_PARAM_ATTR_PREAMBLE_CODE_INDEX:
 *	Preamble code for this session, BPRF (9-24), HPRF (25-32)
 * @RADAR_SESSION_PARAM_ATTR_PREAMBLE_DURATION:
 *	Code indicating the preamble duration in symbols.
 *	0x00 = 64 symbols
 *	0x01 = 1024 symbols
 *	0x02 = 2048 symbols
 * @RADAR_SESSION_PARAM_ATTR_SESSION_PRIORITY:
 *	Priority of the session.
 * @RADAR_SESSION_PARAM_ATTR_BURST_PERIOD:
 *	Duration between two consecutive radar bursts in millisecond.
 * @RADAR_SESSION_PARAM_ATTR_SWEEP_PERIOD:
 *	Duration between two consecutive radar sweeps in RSTU.
 * @RADAR_SESSION_PARAM_ATTR_SWEEPS_PER_BURST:
 *	Number of radar sweeps within a burst.
 * @RADAR_SESSION_PARAM_ATTR_SAMPLES_PER_SWEEP:
 *	Number of samples per sweep.
 * @RADAR_SESSION_PARAM_ATTR_SWEEP_OFFSET:
 *	Number of samples offset before First Path.
 * @RADAR_SESSION_PARAM_ATTR_BYTES_PER_SAMPLE:
 *	Number of bytes per sample.
 * @RADAR_SESSION_PARAM_ATTR_RADAR_DATA_TYPE:
 *	Type of radar data to be reported in RADAR_DATA_MESSAGE.
 * @RADAR_SESSION_PARAM_ATTR_ANTENNA_GROUP_NB:
 *	Number of antenna pair used.
 * @RADAR_SESSION_PARAM_ATTR_TX_PROFILE_IDX:
 *	index of TX profile.
 * @RADAR_SESSION_PARAM_ATTR_NUMBER_OF_BURSTS:
 *	Number of bursts to be done in one radar session.
 *	0x00 = infinity
 * @RADAR_SESSION_PARAM_ATTR_UNSPEC: Invalid command.
 * @__RADAR_SESSION_PARAM_ATTR_AFTER_LAST: Internal use.
 * @RADAR_SESSION_PARAM_ATTR_MAX: Internal use.
 */
enum radar_session_param_attrs {
	RADAR_SESSION_PARAM_ATTR_UNSPEC,
	/* Main session parameters. */
	RADAR_SESSION_PARAM_ATTR_CHANNEL_NUMBER,
	RADAR_SESSION_PARAM_ATTR_RFRAME_CONFIG,
	RADAR_SESSION_PARAM_ATTR_PREAMBLE_CODE_INDEX,
	RADAR_SESSION_PARAM_ATTR_PREAMBLE_DURATION,
	RADAR_SESSION_PARAM_ATTR_SESSION_PRIORITY,
	RADAR_SESSION_PARAM_ATTR_BURST_PERIOD,
	RADAR_SESSION_PARAM_ATTR_SWEEP_PERIOD,
	RADAR_SESSION_PARAM_ATTR_SWEEPS_PER_BURST,
	RADAR_SESSION_PARAM_ATTR_SAMPLES_PER_SWEEP,
	RADAR_SESSION_PARAM_ATTR_SWEEP_OFFSET,
	RADAR_SESSION_PARAM_ATTR_BYTES_PER_SAMPLE,
	RADAR_SESSION_PARAM_ATTR_RADAR_DATA_TYPE,
	RADAR_SESSION_PARAM_ATTR_ANTENNA_GROUP_NB,
	RADAR_SESSION_PARAM_ATTR_TX_PROFILE_IDX,
	RADAR_SESSION_PARAM_ATTR_NUMBER_OF_BURSTS,

	__RADAR_SESSION_PARAM_ATTR_AFTER_LAST,
	RADAR_SESSION_PARAM_ATTR_MAX = __RADAR_SESSION_PARAM_ATTR_AFTER_LAST - 1
};

/**
 * enum radar_data_attrs - radar data attributes.
 *
 * @RADAR_DATA_ATTR_NUM_TAPS:
 *	Number of TAPS.
 * @RADAR_DATA_ATTR_NUM_CIRS:
 *	Number of CIRs per package.
 * @RADAR_DATA_ATTR_SWEEP_OFFSET:
 *	Used value of taps offset in relation to First Path.
 * @RADAR_DATA_ATTR_BYTES_PER_SAMPLE:
 *	Number of bytes per sample.
 * @RADAR_DATA_ATTR_BUF_FRAMES:
 *	Container aggregating NUM_CIRS of TAPS/frames
 * @RADAR_DATA_ATTR_UNSPEC: Invalid command.
 * @__RADAR_DATA_ATTR_AFTER_LAST: Internal use.
 * @RADAR_DATA_ATTR_MAX: Internal use.
 */
enum radar_data_attrs {
	RADAR_DATA_ATTR_UNSPEC,
	RADAR_DATA_ATTR_NUM_TAPS,
	RADAR_DATA_ATTR_NUM_CIRS,
	RADAR_DATA_ATTR_SWEEP_OFFSET,
	RADAR_DATA_ATTR_BYTES_PER_SAMPLE,
	RADAR_DATA_ATTR_BUF_FRAMES,

	__RADAR_DATA_ATTR_AFTER_LAST,
	RADAR_DATA_ATTR_MAX = __RADAR_DATA_ATTR_AFTER_LAST - 1
};

#ifdef __cplusplus
}
#endif
