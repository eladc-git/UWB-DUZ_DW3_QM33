/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_scheduler.h"
#include "net/fira_region_params.h"
#include "net/mcps802154.h"
#include "net/mcps802154_schedule.h"
#include "qlist.h"
#include "qtils.h"
#include "tmp_fbs_region.h"

#include <stdbool.h>
#include <stdint.h>

#define FIRA_SLOT_DURATION_RSTU_DEFAULT 2400
#define FIRA_BLOCK_DURATION_MS_DEFAULT 200
#define FIRA_ROUND_DURATION_SLOTS_DEFAULT 25
#define FIRA_MAX_RR_RETRY_DEFAULT 0
#define FIRA_CAP_SIZE_MIN_DEFAULT 5
#define FIRA_PRIORITY_MAX 100
#define FIRA_PRIORITY_MIN 1
#define FIRA_PRIORITY_DEFAULT (FBS_SCHEDULER_PRIORITY_DEFAULT)
#define FIRA_IN_BAND_TERMINATION_ATTEMPT_COUNT_MAX 10
#define FIRA_BOOLEAN_MAX 1
#define FIRA_BLOCK_STRIDE_LEN_MAX 255

#define FIRA_FRAMES_MAX FIRA_TWR_FRAMES_MAX
/*
 * The controller sends up to 5 messages:
 * Control Message (CM), Ranging Initiation Message (RIM), Ranging
 * Final Message (RFM), Measurement Report Message (MRM) and Control Update
 * Message (CRUM).
 * Each controlee sends up to 2 messages:
 * Ranging Response Message (RRM) and Ranging Result Report Message (RRRM).
 */
#define FIRA_TWR_FRAMES_MAX (5 + 2 * FIRA_RESPONDERS_MAX)
/*
 * The DT-Anchor Initiator sends up to 2 messages:
 * Poll DL-TDoA Message (Poll DTM) and Final DL-TDoA Message (Final DTM).
 * Each DT-Anchor Responder sends 1 message:
 * Response DL-TDoA Message (Response DTM).
 */
#define FIRA_DL_TDOA_FRAMES_MAX (2 + 1 * FIRA_RESPONDERS_MAX)

/* Maximum value (FIRA_RESPONDERS_MAX, FIRA_OWR_AOA_MEASUREMENTS_MAX, FIRA_DL_TDOA_FRAMES_MAX). */
#define FIRA_MEASUREMENTS_MAX FIRA_OWR_AOA_MEASUREMENTS_MAX

#define FIRA_CONTROLEE_FRAMES_MAX (3 + 3 + 1)
/* FiRa Tx should arrive between 0 and 10 us, always add 2 us. */
#define FIRA_TX_MARGIN_US 2

#define NS_PER_SECOND 1000000000ull

/*
 * FIRA_SESSION_DATA_NTF_LOWER_/UPPER_BOUND_AOA min/max :
 * Azimuth in rad_2pi : -32768 / 32768 (equal to -180 / +180 degrees)
 * Elevation in rad_2pi : -16384 / 16384 (equal to -90 / 90 degrees)
 */
#define FIRA_SESSION_DATA_NTF_LOWER_BOUND_AOA_AZIMUTH_2PI_MIN -32768
#define FIRA_SESSION_DATA_NTF_LOWER_BOUND_AOA_AZIMUTH_2PI_MAX 32768
#define FIRA_SESSION_DATA_NTF_UPPER_BOUND_AOA_AZIMUTH_2PI_MIN -32768
#define FIRA_SESSION_DATA_NTF_UPPER_BOUND_AOA_AZIMUTH_2PI_MAX 32768
#define FIRA_SESSION_DATA_NTF_LOWER_BOUND_AOA_ELEVATION_2PI_MIN -16384
#define FIRA_SESSION_DATA_NTF_LOWER_BOUND_AOA_ELEVATION_2PI_MAX 16384
#define FIRA_SESSION_DATA_NTF_UPPER_BOUND_AOA_ELEVATION_2PI_MIN -16384
#define FIRA_SESSION_DATA_NTF_UPPER_BOUND_AOA_ELEVATION_2PI_MAX 16384
#define FIRA_SESSION_DATA_NTF_PROXIMITY_CM_MAX UINT16_MAX

#define FIRA_OWR_AOA_MIN_FRAMES_PER_RR_MIN 1
#define FIRA_OWR_AOA_MIN_FRAMES_PER_RR_MAX 15
#define FIRA_OWR_AOA_MIN_FRAMES_PER_RR_DEFAULT 4
#define FIRA_OWR_AOA_INTER_FRAME_INTERVAL_MS_DEFAULT 1

/**
 * struct fira_diagnostic - Diagnostic result.
 */
struct fira_diagnostic {
	/**
	 * @is_tx: True if the frame is scheduled for TX, false if for RX.
	 */
	bool is_tx;
	/**
	 * @status: additional frame status, bitfield.
	 * See &enum fira_ranging_diagnostics_frame_reports_status_flags.
	 */
	uint32_t status;
	/**
	 * @aoas: Angle of arrival, ordered by increasing measurement type.
	 */
	struct mcps802154_rx_aoa_measurements aoas[MCPS802154_RX_AOA_MEASUREMENTS_MAX];
	/**
	 * @cirs: CIR for different parts of the frame.
	 *
	 * Set by low-level driver, must be kept valid until next received
	 * frame.
	 */
	struct mcps802154_rx_segment_cir *cirs;
	/**
	 * @rx_seg_metrics: RX Metrics computed for each segment of the frame.
	 * This field is needed to retrieve the former CIR parameters that have
	 * been move within this structure.
	 */
	struct mcps802154_rx_segment_metrics *rx_seg_metrics;
	/**
	 * @n_aoas: Number of angle of arrival.
	 */
	uint8_t n_aoas;
	/**
	 * @n_segment_meas: Number of available segments.
	 */
	uint8_t n_segment_meas;
	/**
	 * @emitter_short_addr: The MAC short address of the frame emitter.
	 */
	uint16_t emitter_short_addr;
	/**
	 * @cfo_q26: Clock Frequency Offset, if requested.
	 */
	int16_t cfo_q26;
};

/**
 * struct fira_slot - Information on an active slot.
 */
struct fira_slot {
	/**
	 * @index: Index of this slot, add it to the block STS index to get the
	 * slot STS index. Note: there can be holes for a responder as only
	 * relevant slots are recorded.
	 */
	int16_t index;
	/**
	 * @ranging_index: Index of the ranging in the ranging information
	 * table, -1 if none.
	 */
	int8_t ranging_index;
	/**
	 * @ant_set_id: Antenna set ID used for Tx/RX and to report.
	 */
	int8_t ant_set_id;
	/**
	 * @initiator_tx: True if Tx is performed by the initiator.
	 */
	bool initiator_tx;
	/**
	 * @rframe_no_data: True if receiving an empty rframe is accepted.
	 */
	bool rframe_no_data;
	/**
	 * @message_id: Identifier of the message exchanged in this slot.
	 */
	enum fira_message_id message_id;
	/**
	 * @owr_message_type: Type of the One Way Ranging (OWR) Message.
	 */
	enum fira_owr_message_type owr_message_type;
	union {
		/**
		 * @controlee: Basic information about the controlee assigned
		 * to this slot.
		 */
		struct fira_controlee *controlee;
		/**
		 * @dt_anchor_responder: Basic information about the Responder
		 * assigned to this slot.
		 */
		struct fira_owr_dt_responder *dt_anchor_responder;
	};
};

/**
 * struct fira_local_aoa_info - Ranging AoA information.
 */
struct fira_local_aoa_info {
	/**
	 * @pdoa_2pi: Phase Difference of Arrival.
	 */
	int16_t pdoa_2pi;
	/**
	 * @aoa_2pi: Angle of Arrival.
	 */
	int16_t aoa_2pi;
	/**
	 * @aoa_fom_100: Figure of merit of the AoA.
	 */
	uint8_t aoa_fom_100;
	/**
	 * @rx_ant_set: Antenna set index.
	 */
	uint8_t rx_ant_set;
	/**
	 * @present: true if AoA information is present.
	 */
	bool present;
};

/**
 * enum fira_session_info_ntf_status - Device (controller or controlee)
 * status, used for session_info_ntf.
 * @FIRA_SESSION_INFO_NTF_NONE: Undetermined, no ranging data for this
 * device yet, or N/A (not applicable).
 * @FIRA_SESSION_INFO_NTF_IN: Last ranging data for this device
 * were inside given boudaries.
 * @FIRA_SESSION_INFO_NTF_OUT: Last ranging data for this device
 * were outside given boudaries.
 * @FIRA_SESSION_INFO_NTF_ERROR: Last ranging round(s) for this device
 * failed (timeout, error, ...). No info about a previous state or N/A.
 */
enum fira_session_info_ntf_status {
	FIRA_SESSION_INFO_NTF_NONE,
	FIRA_SESSION_INFO_NTF_IN,
	FIRA_SESSION_INFO_NTF_OUT,
	FIRA_SESSION_INFO_NTF_ERROR,
};

/**
 * struct fira_ranging_info - The subset of the ranging information
 * which is common for all FiRa session modes.
 */
struct fira_ranging_info {
	/**
	 * @short_addr: Short address of the peer device.
	 */
	uint16_t short_addr;
	/**
	 * @status: FiRa Status Code.
	 */
	enum quwbs_fbs_status status;
	/**
	 * @slot_index: In case of a failure, the slot index when the error occured.
	 */
	uint8_t slot_index;
	/**
	 * @local_aoa: Information about undefined AoA axis measured during the reception.
	 */
	struct fira_local_aoa_info local_aoa;
	/**
	 * @local_aoa_azimuth: Information about AoA Azimuth measured during the reception.
	 */
	struct fira_local_aoa_info local_aoa_azimuth;
	/**
	 * @local_aoa_elevation: Information about AoA Elevation measured during the reception.
	 */
	struct fira_local_aoa_info local_aoa_elevation;
	/**
	 * @real_azimuth: True if the real AoA Azimuth has been computed, otherwise it's just AoA
	 * measured on the X axis.
	 */
	bool real_azimuth;
	/**
	 * @sts_quality: Sts quality indicator.
	 */
	uint8_t sts_quality;
	/**
	 * @rx_ctx: Pointer to the current Rx context (for Responders).
	 */
	void *rx_ctx;
};

/**
 * struct fira_twr_ranging_info - Ranging information structure which is used
 * to fill TWR Measurement Result (applicable for all the roles in TWR and for DT-Anchors).
 */
struct fira_twr_ranging_info {
	/**
	 * @common: The subset common for all the session modes. Shall be the first field
	 * of this struct.
	 */
	struct fira_ranging_info common;
	/**
	 * @tof_present: True if tof_rctu value is present, false otherwise.
	 */
	bool tof_present;
	/**
	 * @remote_aoa_azimuth_present: True if remote_aoa_azimuth_2pi values is present, false
	 * otherwise.
	 */
	bool remote_aoa_azimuth_present;
	/**
	 * @remote_aoa_elevation_present: True if remote_aoa_elevation_pi value is present, false
	 * otherwise.
	 */
	bool remote_aoa_elevation_present;
	/**
	 * @remote_aoa_fom_present: True if the FoM metric is present for AoA values measured
	 * on the peer's side.
	 */
	bool remote_aoa_fom_present;
	/**
	 * @clock_offset_present: True if clock_offset_q26 values is present, false otherwise.
	 */
	bool clock_offset_present;
	/**
	 * @remote_aoa_azimuth_fom: FoM metric for the AoA Azimuth measured on the peer's side.
	 */
	uint8_t remote_aoa_azimuth_fom;
	/**
	 * @remote_aoa_elevation_fom: FoM metric for the AoA Elevation measured on the peer's side.
	 */
	uint8_t remote_aoa_elevation_fom;
	/**
	 * @remote_aoa_azimuth_2pi: AoA Azimuth measured on the peer's side.
	 */
	int16_t remote_aoa_azimuth_2pi;
	/**
	 * @remote_aoa_elevation_pi: AoA Elevation measured on the peer's side.
	 */
	int16_t remote_aoa_elevation_pi;
	/**
	 * @clock_offset_q26: Clock Frequency Offset measured in reference to the peer's device
	 * (encoded as signed Q26).
	 */
	int16_t clock_offset_q26;
	/**
	 * @tof_rctu: Computed Time of Flight in RCTU.
	 */
	int tof_rctu;
	/**
	 * @n_rx_rssis: Number of the measured RSSI values.
	 */
	int n_rx_rssis;
	/**
	 * @rx_rssis_q1: RSSI values measured for inidividual frames received.
	 */
	uint8_t rx_rssis_q1[FIRA_MESSAGE_ID_MAX + 1];
	/**
	 * @timestamps_rctu: Saved timestamps for the infivitual RFRAMEs.
	 */
	uint64_t timestamps_rctu[FIRA_MESSAGE_ID_RFRAME_MAX + 1];
};

static inline struct fira_twr_ranging_info *
fira_get_ranging_info_twr(const struct fira_ranging_info *ranging_info)
{
	return qparent_of(ranging_info, struct fira_twr_ranging_info, common);
}

/**
 * struct fira_step - All informations needed for the FiRa step.
 */
struct fira_step {
	/** @next_index: Next slot, frame or step index (same value). */
	int16_t next_index;
	/** @n_slots: Number of slots. */
	int16_t n_slots;
	/** @slot0_timestamp_dtu: Timestamp of the first slot. */
	uint32_t slot0_timestamp_dtu;
	/** @slot0_rx_timeout_dtu: Rx timeout of the first slot. */
	int slot0_rx_timeout_dtu;
};

/**
 * struct fira_deferred - Deferred context.
 */
struct fira_deferred {
	/**
	 * @n_slots_remaining: Number of slots remaining.
	 */
	uint16_t n_slots_remaining;
	/**
	 * @slot_idx: Slot index of the PSDU.
	 */
	uint8_t slot_idx;
	/**
	 * @psdu: PSDU to store, can be null if no job to do.
	 */
	struct sk_buff *psdu;
};

/**
 * struct fira_workspace - Local context.
 */
struct fira_workspace {
	/**
	 * @step: Step (or slot) in the current access.
	 */
	struct fira_step step;
	/**
	 * @src_short_addr: Source address for the current session.
	 */
	uint16_t src_short_addr;
	/**
	 * @dst_short_addr: Destination address for the current session. When
	 * initiator, this is the broadcast address or the address of the only
	 * responder. When responder, this is the address of the initiator.
	 */
	uint16_t dst_short_addr;
	/**
	 * @slots: Descriptions of each active slots for the current session.
	 * When initiator, this is filled when the access is requested. When
	 * responder, the first slot is filled when the access is requested
	 * and the other slots are filled when the Control Message (TWR) or
	 * Poll DTM (DL-TDoA) is received.
	 */
	struct fira_slot *slots[FIRA_FRAMES_MAX];
	/**
	 * @diagnostics: Diagnostic collected for each slot.
	 */
	struct fira_diagnostic *diagnostics[FIRA_FRAMES_MAX];
	/**
	 * @ranging_info: Information on ranging for the current session. Elements in this array are
	 * defined depending on the device role:
	 * - for every Responder when the role is Initiator (both in TWR and for DT-Anchor)
	 * - only one element for Responder (both in TWR and for DT-Anchor)
	 * - only one element for Observer when the message aggregation is disabled
	 * - for every aggregated message for Observer when the message aggregation is enabled
	 * - for every message received for DT-Tag
	 */
	struct fira_ranging_info *ranging_info[FIRA_MEASUREMENTS_MAX];
	/**
	 * @n_ranging_info: Number of element in the ranging information table.
	 */
	uint8_t n_ranging_info;
	/**
	 * @n_ranging_valid: Number of valid ranging in the current ranging
	 * information table.
	 */
	uint8_t n_ranging_valid;
	/**
	 * @deferred: Deferred context.
	 */
	struct fira_deferred deferred;
	/**
	 * @psdus_report: Report containing all the PSDUs of the ranging round.
	 */
	struct sk_buff *psdus_report;
	/**
	 * @stopped_controlees: Short addresses of the stopped controlees for
	 * which an element must be added to the Device Management List of
	 * the control message.
	 */
	uint16_t stopped_controlees[FIRA_RESPONDERS_MAX];
	/**
	 * @n_stopped_controlees: Number of elements in the stopped controlees.
	 */
	uint8_t n_stopped_controlees;
	/**
	 * @n_notify_stopped_controlees: Number of elements in the stopped
	 * controlees that need to be notified.
	 */
	uint8_t n_notify_stopped_controlees;
};

/**
 * struct fira_data_payload_rcv_buffer - Element in the receiver buffer list.
 * See &app_data_payload_rcv.
 */
struct fira_data_payload_rcv_buffer {
	/**
	 * @entry: Entry as a new receiver buffer.
	 */
	struct qlist_head entry;
	/**
	 * @src_address: Short address of the sender device associated with
	 * this buffer.
	 */
	mcps802154_device_address src_address;
	/**
	 * @payload_segments: List of segments contained by this receiver
	 * buffer. See &struct fira_data_payload_segment.
	 */
	struct qlist_head payload_segments;
	/**
	 * @n_payload_segments: number of segments.
	 */
	uint8_t n_payload_segments;
};

/**
 * struct fira_data_payload_segment - Element of the payload segment list.
 * See &struct fira_data_payload_rcv_buffer.
 */
struct fira_data_payload_segment {
	/**
	 * @entry: Entry as a new segment in the payload.
	 */
	struct qlist_head entry;
	/**
	 * @length: Length of the segment.
	 */
	uint16_t length;
	/**
	 * @app_data: Pointer to the application data contained by the segment.
	 */
	uint8_t *app_data;
};
