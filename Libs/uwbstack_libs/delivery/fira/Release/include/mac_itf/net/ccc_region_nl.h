/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/fbs_region_nl.h"

#include <stdint.h>

/* TODO: Remove dependency to fira include and use a new common measurement
 * sequence structure in FiRa and CCC regions defined in another public
 * include.
 */
#include "net/fira_region_params.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * enum ccc_capability_attrs - CCC capabilities.
 *
 * @CCC_CAPABILITY_ATTR_UNSPEC: Internal use.
 * @CCC_CAPABILITY_ATTR_CHAPS_PER_SLOT:
 *	Chaps per slot supported.
 * @CCC_CAPABILITY_ATTR_SYNC_CODES:
 *	CCC sync codes supported.
 * @CCC_CAPABILITY_ATTR_HOPPING_CONFIG_MODES_AND_SEQUENCES:
 *	CCC hopping config mode supported.
 * @CCC_CAPABILITY_ATTR_CHANNELS:
 *	Bitmask for supported channels:
 *	- bit0: Channel 5 is supported.
 *	- bit1: Channel 9 is supported.
 * @CCC_CAPABILITY_ATTR_VERSIONS:
 *	CCC versions supported.
 * @CCC_CAPABILITY_ATTR_UWB_CONFIGS:
 *	CCC UWB configs supported.
 * @CCC_CAPABILITY_ATTR_PULSE_SHAPE_COMBOS:
 *	CCC pulse shape combos supported.
 * @CCC_CAPABILITY_ATTR_RAN_MULTIPLIER:
 *	RAN multiplier supported.
 * @__CCC_CAPABILITY_ATTR_AFTER_LAST: Internal use.
 * @CCC_CAPABILITY_ATTR_MAX: Internal use.
 */
enum ccc_capability_attrs {
	CCC_CAPABILITY_ATTR_UNSPEC,
	/* Main session capabilities. */
	CCC_CAPABILITY_ATTR_CHAPS_PER_SLOT = FBS_CAPABILITY_ATTR_VENDOR1_START,
	CCC_CAPABILITY_ATTR_SYNC_CODES,
	CCC_CAPABILITY_ATTR_HOPPING_CONFIG_MODES_AND_SEQUENCES,
	CCC_CAPABILITY_ATTR_CHANNELS,
	CCC_CAPABILITY_ATTR_VERSIONS,
	CCC_CAPABILITY_ATTR_UWB_CONFIGS,
	CCC_CAPABILITY_ATTR_PULSE_SHAPE_COMBOS,
	CCC_CAPABILITY_ATTR_RAN_MULTIPLIER,

	__CCC_CAPABILITY_ATTR_AFTER_LAST,
	CCC_CAPABILITY_ATTR_MAX = __CCC_CAPABILITY_ATTR_AFTER_LAST - 1
};

/**
 * enum ccc_call_attrs - CCC call attributes.
 *
 * @CCC_CALL_ATTR_SESSION_ID:
 *	Session identifier.
 * @CCC_CALL_ATTR_SESSION_PARAMS:
 *	Session parameters.
 * @CCC_CALL_ATTR_CONTROLEES:
 *	Controlees information.
 * @CCC_CALL_ATTR_RANGING_DATA:
 *	Ranging data NTF.
 * @CCC_CALL_ATTR_CAPABILITIES:
 *	Capabilities.
 * @CCC_CALL_ATTR_SESSION_STATE:
 *	Session state.
 * @CCC_CALL_ATTR_SESSION_COUNT:
 *	Sessions count.
 * @CCC_CALL_ATTR_SEQUENCE_NUMBER:
 *	Session notification counter.
 * @CCC_CALL_ATTR_RANGING_DIAGNOSTICS:
 *	Diagnostic information.
 * @CCC_CALL_ATTR_PSDUS_LOG:
 *	PSDUs logs.
 * @CCC_CALL_ATTR_SESSION_STATUS:
 *	Session Status NTF.
 * @CCC_CALL_ATTR_SESSION_HANDLE:
 *	Session handle returned in response to FBS_CALL_SESSION_INIT.
 * @CCC_CALL_ATTR_RANGING_COUNT:
 *	The number of successful ranging attempts in a session.
 * @CCC_CALL_ATTR_UNSPEC: Invalid command.
 * @__CCC_CALL_ATTR_AFTER_LAST: Internal use.
 * @CCC_CALL_ATTR_MAX: Internal use.
 */
enum ccc_call_attrs {
	CCC_CALL_ATTR_UNSPEC,
	CCC_CALL_ATTR_SESSION_ID,
	CCC_CALL_ATTR_SESSION_PARAMS,
	CCC_CALL_ATTR_CONTROLEES,
	CCC_CALL_ATTR_RANGING_DATA,
	CCC_CALL_ATTR_CAPABILITIES,
	CCC_CALL_ATTR_SESSION_STATE,
	CCC_CALL_ATTR_SESSION_COUNT,
	CCC_CALL_ATTR_SEQUENCE_NUMBER,
	CCC_CALL_ATTR_RANGING_DIAGNOSTICS,
	CCC_CALL_ATTR_PSDUS_LOG = FBS_CALL_ATTR_PSDUS_LOG,
	CCC_CALL_ATTR_SESSION_STATUS = FBS_CALL_ATTR_SESSION_STATUS,
	CCC_CALL_ATTR_SESSION_HANDLE = FBS_CALL_ATTR_SESSION_HANDLE,
	CCC_CALL_ATTR_RANGING_COUNT = FBS_CALL_ATTR_RANGING_COUNT,

	__CCC_CALL_ATTR_AFTER_LAST,
	CCC_CALL_ATTR_MAX = __CCC_CALL_ATTR_AFTER_LAST - 1
};

/* bit B7 No Hopping. */
#define HOP_CONF_MSK_NONE 0x80
/* bit B6 Continuous Hopping. */
#define HOP_CONF_MSK_CONT 0x40
/* bit B5 Adaptive Hopping. */
#define HOP_CONF_MSK_ADPT 0x20
/* bit B4 default hopping sequence and support thereof is mandator:y and defined
 * in Appendix A. */
#define HOP_CONF_MSK_DFLT 0x10
/* bit B3 optional AES-based hopping sequence. */
#define HOP_CONF_MSK_AES 0x08
/* bit B2 Rest of the bits are reserved for future hopping sequences. */
#define HOP_CONF_MSK_RES2 0x04
#define HOP_CONF_MSK_RES1 0x02
#define HOP_CONF_MSK_RES0 0x01

/**
 * enum ccc_device_type - Type of a device.
 * @CCC_DEVICE_TYPE_RESPONDER: The device is a responder.
 * @CCC_DEVICE_TYPE_INITIATOR: The device is a initiator.
 */
enum ccc_device_type {
	CCC_DEVICE_TYPE_RESPONDER,
	CCC_DEVICE_TYPE_INITIATOR,
};

/*
 * @brief Default Session configuration parameters when using Shell command :
 * 'SESS_DBG' (which accepts no parameter)
 * */
#define DEFAULT_CCC_CHANNEL_NUMBER 9
#define DEFAULT_CCC_NUMBER_RESPONDERS_NODES 5
#define DEFAULT_CCC_SLOT_DURATION 1200
#define DEFAULT_CCC_RANGING_INTERVAL 480
/*
 * STS Index 0 can't exceed 0x80000000 but here it's a magic number
 * which permits to determine if we have to generate randomly a value
 * or if a value has been set by the user (debug mode).
 */
#define DEFAULT_CCC_STS_INDEX0 0xffffffff
#define DEFAULT_CCC_SESSION_INFO_NTF_CONFIG 0
#define DEFAULT_CCC_SYNC_CODE_INDEX 9
/* Slot per round => 8 Rounds if Block = 96ms (see CCC Table F-1) */
#define DEFAULT_CCC_NUMBER_SLOT_PER_ROUND 12
#define DEFAULT_CCC_HOP_CONFIG_BITMASK HOP_CONF_MSK_NONE
/* (HOP_CONF_MSK_CONT|HOP_CONF_MSK_DFLT) */
#define DEFAULT_CCC_HOP_MODE_KEY 0xbc72574c
#define DEFAULT_CCC_UWB_TIME0 0
#define DEFAULT_CCC_SELECTED_PROTOCOL_VERSION 0x100
#define DEFAULT_CCC_SELECTED_UWB_CONFIG_ID 1
#define DEFAULT_CCC_SELECTED_PULSE_SHAPE_COMBO 0x11
#define DEFAULT_CCC_URSK_TTL 0
#define DEFAULT_CCC_MAC_PAYLOAD_ENCRYPTION 1
#define DEFAULT_CCC_REPORT_PSDUS 0

/* 1ms slot duration (1 chap = (1/3) ms). */
#define DEFAULT_CCC_NUMBER_CHAPS_PER_SLOT 3
/* 40 Rounds per Block = 5 x 96ms when Nran=5 as defined here. */
#define DEFAULT_CCC_RAN_MULTIPLIER 5
#define DEFAULT_CCC_SELECTED_UWB_CHANNEL 5
#define DEFAULT_CCC_SELECTED_UWB_CONFIG_ID 1

/* Unused value. */
#define DEFAULT_CCC_DEVICE_TYPE 1
#define DEFAULT_CCC_STS_CONFIG 3
#define DEFAULT_CCC_RANGING_ROUND_CONTROL 0
#define DEFAULT_CCC_DEVICE_ROLE 1
#define DEFAULT_CCC_MULTI_NODE_MODE 0
#define DEFAULT_CCC_KEY_ROTATION 0

/* Starting time reference (resolution 1 microsecond) for ranging.
 * Offset value relative to initial LE Connection Complete event. */
#define DEFAULT_CCC_PULSE_SHAPE_COMBO 0x11
#define CCC_SLOT_RSTU_DEFAULT ((DEFAULT_CCC_NUMBER_CHAPS_PER_SLOT)*400)
#define CCC_RANGING_INTERVAL_MS_DEFAULT ((DEFAULT_CCC_RAN_MULTIPLIER)*96)

#define CCC_INIT_SESSION 0
#define CCC_START_SESSION 1
#define CCC_STOP_SESSION 2
#define CCC_SUSPEND_SESSION 3
#define CCC_RECOVER_SESSION 4

/**
 * enum ccc_session_param_attrs - ccc session parameters attributes.
 *
 * @CCC_SESSION_PARAM_ATTR_DEVICE_TYPE:
 *	Controlee (0) or controller (1)
 * @CCC_SESSION_PARAM_ATTR_STS_CONFIG:
 *	Static STS (0, default), dynamic STS (1) or dynamic STS for controlee
 *	individual keys (2)
 * @CCC_SESSION_PARAM_ATTR_SESSION_INFO_NTF_CONFIG:
 *
 * @CCC_SESSION_PARAM_ATTR_NEAR_PROXIMITY_CONFIG:
 *
 * @CCC_SESSION_PARAM_ATTR_FAR_PROXIMITY_CONFIG:
 *
 * @CCC_SESSION_PARAM_ATTR_CCC_UWB_TIME0:
 *	Initiation time in unit of 1200 RSTU (same as ms), default 0.
 * @CCC_SESSION_PARAM_ATTR_UWB_SESSION_ID:
 *
 * @CCC_SESSION_PARAM_ATTR_STS_INDEX0:
 *
 * @CCC_SESSION_PARAM_ATTR_RANGING_ROUND_CONTROL:
 *
 * @CCC_SESSION_PARAM_ATTR_HOP_MODE_KEY:
 *
 * @CCC_SESSION_PARAM_ATTR_SELECTED_PROTOCOL_VERSION:
 *
 * @CCC_SESSION_PARAM_ATTR_SELECTED_UWB_CONFIG_ID:
 *
 * @CCC_SESSION_PARAM_ATTR_CHANNEL_NUMBER:
 *	Override channel for this session: 5, 6, 8, 9, 10, 12, 13 or 14
 * @CCC_SESSION_PARAM_ATTR_HOP_CONFIG_BITMASK:
 *
 * @CCC_SESSION_PARAM_ATTR_SYNC_CODE_INDEX:
 *
 * @CCC_SESSION_PARAM_ATTR_RANGING_INTERVAL:
 *
 * @CCC_SESSION_PARAM_ATTR_SLOT_DURATION:
 *
 * @CCC_SESSION_PARAM_ATTR_NUMBER_RESPONDERS_NODES:
 *
 * @CCC_SESSION_PARAM_ATTR_NUMBER_SLOT_PER_ROUND:
 *
 * @CCC_SESSION_PARAM_ATTR_SELECTED_PULSE_SHAPE_COMBO:
 *
 * @CCC_SESSION_PARAM_ATTR_URSK_TTL:
 *
 * @CCC_SESSION_PARAM_ATTR_DEVICE_ROLE:
 *	Responder (0) or initiator (1)
 * @CCC_SESSION_PARAM_ATTR_MULTI_NODE_MODE:
 *
 * @CCC_SESSION_PARAM_ATTR_KEY_ROTATION:
 *
 * @CCC_SESSION_PARAM_ATTR_REPORT_PSDUS:
 *	Report psdu data on each round, disabled (0, default) or enabled (1)
 * @CCC_SESSION_PARAM_ATTR_TX_ANTENNA_SELECTION:
 *	Antenna set to use during TX phases
 * @CCC_SESSION_PARAM_ATTR_RX_ANTENNA_SELECTION:
 *	Antenna set to use during RX phases
 *
 * @CCC_SESSION_PARAM_ATTR_MEASUREMENT_SEQUENCE:
 *	Sequence of measurement steps. Configures antenna flexibility.
 *
 * @CCC_SESSION_PARAM_ATTR_CCC_STS_INDEX:
 *	Get the current sts index.
 *
 * @CCC_SESSION_PARAM_ATTR_MAC_PAYLOAD_ENCRYPTION:
 *	Activate the encryption of the SP0 payload for CCC.
 * @CCC_SESSION_PARAM_ATTR_MAC_MODE:
 *	Set the number of ranging rounds per block and the offset between rounds.
 * @CCC_SESSION_PARAM_ATTR_URSK:
 *	Set URSK through UCI.
 *
 * @CCC_SESSION_PARAM_ATTR_UNSPEC: Invalid command.
 * @__CCC_SESSION_PARAM_ATTR_AFTER_LAST: Internal use.
 * @CCC_SESSION_PARAM_ATTR_MAX: Internal use.
 */
enum ccc_session_param_attrs {
	CCC_SESSION_PARAM_ATTR_UNSPEC = 0x9f,
	/* Main session parameters */
	CCC_SESSION_PARAM_ATTR_DEVICE_TYPE,
	CCC_SESSION_PARAM_ATTR_STS_CONFIG,
	CCC_SESSION_PARAM_ATTR_SESSION_INFO_NTF_CONFIG,
	CCC_SESSION_PARAM_ATTR_NEAR_PROXIMITY_CONFIG,
	CCC_SESSION_PARAM_ATTR_FAR_PROXIMITY_CONFIG,
	CCC_SESSION_PARAM_ATTR_CCC_UWB_TIME0,
	CCC_SESSION_PARAM_ATTR_UWB_SESSION_ID,
	CCC_SESSION_PARAM_ATTR_STS_INDEX0,
	CCC_SESSION_PARAM_ATTR_RANGING_ROUND_CONTROL,
	CCC_SESSION_PARAM_ATTR_HOP_MODE_KEY,
	CCC_SESSION_PARAM_ATTR_SELECTED_PROTOCOL_VERSION,
	CCC_SESSION_PARAM_ATTR_SELECTED_UWB_CONFIG_ID,
	CCC_SESSION_PARAM_ATTR_CHANNEL_NUMBER,
	CCC_SESSION_PARAM_ATTR_HOP_CONFIG_BITMASK,
	CCC_SESSION_PARAM_ATTR_SYNC_CODE_INDEX,
	CCC_SESSION_PARAM_ATTR_RANGING_INTERVAL,
	CCC_SESSION_PARAM_ATTR_SLOT_DURATION,
	CCC_SESSION_PARAM_ATTR_NUMBER_RESPONDERS_NODES,
	CCC_SESSION_PARAM_ATTR_NUMBER_SLOT_PER_ROUND,
	CCC_SESSION_PARAM_ATTR_SELECTED_PULSE_SHAPE_COMBO,
	CCC_SESSION_PARAM_ATTR_URSK_TTL,
	CCC_SESSION_PARAM_ATTR_DEVICE_ROLE,
	CCC_SESSION_PARAM_ATTR_MULTI_NODE_MODE,
	CCC_SESSION_PARAM_ATTR_KEY_ROTATION,
	CCC_SESSION_PARAM_ATTR_REPORT_PSDUS,
	CCC_SESSION_PARAM_ATTR_TX_ANTENNA_SELECTION,
	CCC_SESSION_PARAM_ATTR_RX_ANTENNA_SELECTION,
	CCC_SESSION_PARAM_ATTR_MEASUREMENT_SEQUENCE,
	CCC_SESSION_PARAM_ATTR_CCC_STS_INDEX,
	CCC_SESSION_PARAM_ATTR_MAC_PAYLOAD_ENCRYPTION,
	CCC_SESSION_PARAM_ATTR_MAC_MODE,
	CCC_SESSION_PARAM_ATTR_URSK,

	__CCC_SESSION_PARAM_ATTR_AFTER_LAST,
	CCC_SESSION_PARAM_ATTR_MAX = __CCC_SESSION_PARAM_ATTR_AFTER_LAST - 1
};

/**
 * enum ccc_ranging_data_attrs - ccc ranging data attributes.
 *
 * @CCC_RANGING_DATA_ATTR_STS_INDEX:
 *
 * @CCC_RANGING_DATA_ATTR_BLOCK_INDEX:
 *	Current block index.
 * @CCC_RANGING_DATA_ATTR_ROUND_INDEX:
 *	Current round index.
 * @CCC_RANGING_DATA_ATTR_MEASUREMENTS:
 *	Measurements, see ccc_ranging_data_measurements.
 *
 * @CCC_RANGING_DATA_ATTR_UNSPEC: Invalid command.
 * @__CCC_RANGING_DATA_ATTR_AFTER_LAST: Internal use.
 * @CCC_RANGING_DATA_ATTR_MAX: Internal use.
 */
enum ccc_ranging_data_attrs {
	CCC_RANGING_DATA_ATTR_UNSPEC,
	CCC_RANGING_DATA_ATTR_STS_INDEX,
	CCC_RANGING_DATA_ATTR_BLOCK_INDEX,
	CCC_RANGING_DATA_ATTR_ROUND_INDEX,
	CCC_RANGING_DATA_ATTR_MEASUREMENTS,

	__CCC_RANGING_DATA_ATTR_AFTER_LAST,
	CCC_RANGING_DATA_ATTR_MAX = __CCC_RANGING_DATA_ATTR_AFTER_LAST - 1
};

/*
 * enum ccc_ranging_data_measurements_attrs - ccc ranging data measurements
 * attributes.
 */
enum ccc_ranging_data_measurements_attrs {
	CCC_RANGING_DATA_MEASUREMENTS_ATTR_UNSPEC,
	CCC_RANGING_DATA_MEASUREMENTS_ATTR_SLOT_INDEX,
	CCC_RANGING_DATA_MEASUREMENTS_ATTR_STATUS,
	CCC_RANGING_DATA_MEASUREMENTS_ATTR_TIMESTAMP,

	__CCC_RANGING_DATA_MEASUREMENTS_ATTR_AFTER_LAST,
	CCC_RANGING_DATA_MEASUREMENTS_ATTR_MAX = __CCC_RANGING_DATA_MEASUREMENTS_ATTR_AFTER_LAST - 1
};

/*
 * @brief : this function is called by scanf() parsing results from INITC
 * command to set the 256 bits session key - URSK
 * @param uint32_t uwb_sessionid : UWB session Id for which the URSK is
 * configured
 * @param uint8_t *p_ursk : point to input key buffer of 32 bytes
 * */
void ccc_set_ursk(uint32_t uwb_sessionid, uint8_t *p_ursk);

/**
 * enum ccc_session_param_meas_sched_step_attrs - CCC measurement schedule
 * step attributes.
 *
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_MEASUREMENT_TYPE:
 *	The type of measurement to perform during the step.
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_N_MEASUREMENTS:
 *	The number of times this type of measurement shall be performed
 *	during the step.
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_RX_ANT_SET_NONRANGING:
 *	The antenna set to use to receive the non-rfames during the step.
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_RX_ANT_SETS_RANGING:
 *	The antenna set to use to receive the rframes frame during the step.
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_TX_ANT_SET_NONRANGING:
 *	The antenna set to use to transmit the non-rframes during the step.
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_TX_ANT_SET_RANGING:
 *	The antenna set to use to transmit the rframes during the step.
 *
 *
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_UNSPEC: Invalid command.
 * @__CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_AFTER_LAST: Internal use.
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_MAX: Internal use.
 */
enum ccc_session_param_meas_sched_step_attrs {
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_UNSPEC,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_MEASUREMENT_TYPE,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_N_MEASUREMENTS,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_RX_ANT_SET_NONRANGING,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_RX_ANT_SETS_RANGING,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_TX_ANT_SET_NONRANGING,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_TX_ANT_SET_RANGING,

	__CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_AFTER_LAST,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_MAX =
		__CCC_SESSION_PARAM_MEAS_SCHED_STEP_ATTR_AFTER_LAST - 1
};

/**
 * enum ccc_session_params_meas_sched_step_sets_attrs - Attributes of the
 * CCC RX antenna sets to use during a step.
 *
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_0:
 *	Antenna set used to receive all rframes for range, azimuth and elevation
 *	steps or initial rframe for azimuth_elevation step.
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_1:
 *	Antenna set used to receive final rframes for azimuth_elevation step.
 *
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_UNSPEC:
 *	Invalid command.
 * @CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_MAX:
 *	Internal use.
 * @__CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_AFTER_LAST:
 *	Internal use.
 */
enum ccc_session_params_meas_sched_step_sets_attrs {
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_UNSPEC,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_0,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_1,

	__CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_AFTER_LAST,
	CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_MAX =
		__CCC_SESSION_PARAM_MEAS_SCHED_STEP_RX_ANT_SETS_RANGING_ATTR_AFTER_LAST - 1
};

/*
 * The maximum number of steps a measurement sequence can contain.
 */
#define CCC_MEASUREMENT_SEQUENCE_STEP_MAX 1

#ifdef __cplusplus
}
#endif
