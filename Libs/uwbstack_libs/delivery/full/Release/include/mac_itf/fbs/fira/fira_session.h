/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_region.h"
#include "fbs/fbs_session.h"
#include "fira/fira_frame_defines.h"
#include "fira/fira_region.h"
#include "net/fbs_region_params.h"
#include "net/fira_region_params.h"
#include "net/mcps802154.h"
#include "net/mcps_crypto.h"
#include "qerr.h"
#include "qlist.h"
#include "qtils.h"
#include "quwbs/fbs/defs.h"

#include <stdbool.h>
#include <stdint.h>

struct nlattr;
struct sk_buff;

struct fira_contention_based_data {
	/**
	 * @rml_size : Size of the RML.
	 */
	uint8_t rml_size;
	/**
	 * @n_controlees_cap : Total number of controlees seen in RML and CAP.
	 */
	uint8_t n_controlees_cap;
	/**
	 * @rml_idx : Next index to be inserted in RML.
	 */
	uint8_t rml_idx;
	/**
	 * @n_cap_slots: Number of slots currently available in Contention
	 * Access Period.
	 */
	uint8_t n_cap_slots;
	/**
	 * @slot_idx: Slot index for contention-based, this is only incremented
	 * if a device answer, otherwise we recycle slots.
	 */
	uint8_t slot_idx;
	/**
	 * @step: Internal step (eSS-TWR for now).
	 */
	uint8_t step;
	/**
	 * @nb_step: Numbers of steps (eSS-TWR for now).
	 */
	uint8_t nb_step;
	/**
	 * @round_success: True if the contention-based round is successful.
	 */
	bool round_success;
	/**
	 * @rml: Responder Management List (RML).
	 */
	mcps802154_device_address rml[];
};

struct fira_session_params {
	/* Main parameters. */
	enum quwbs_fbs_device_type device_type;
	enum quwbs_fbs_device_role device_role;
	enum fira_ranging_round_usage ranging_round_usage;
	enum fira_multi_node_mode multi_node_mode;
	enum fbs_mac_address_mode mac_address_mode;
	mcps802154_device_address addr;
	mcps802154_device_address initiator_addr;
	union {
		mcps802154_device_address controller_addr;
		mcps802154_device_address advertiser_addr;
	};
	/* Timings parameters. */
	uint64_t time0_ns;
	int slot_duration_dtu;
	int block_duration_dtu;
	int round_duration_slots;
	/* Only used on type controller or role DT-Anchor. */
	struct fira_session_time_base time_base;
	/* Behaviour parameters. */
	uint32_t block_stride_len;
	uint32_t max_number_of_measurements;
	uint32_t max_rr_retry;
	bool round_hopping;
	bool cap_size_max_default;
	uint8_t cap_size_max;
	uint8_t cap_size_min;
	uint8_t priority;
	bool ranging_round_control_default;
	uint8_t ranging_round_control;
	enum fira_schedule_mode schedule_mode;
	/* Behaviour of OWR for AoA */
	uint8_t min_frames_per_rr;
	uint16_t mtu_size;
	uint8_t inter_frame_interval_ms;
	enum fira_owr_aoa_measurement_ntf_period owr_aoa_measurement_ntf_period;
	/* Radio. */
	int channel_number;
	int preamble_code_index;
	enum fira_rframe_config rframe_config;
	enum fira_preamble_duration preamble_duration;
	enum fira_sfd_id sfd_id;
	enum fira_sts_segments number_of_sts_segments;
	enum fira_psdu_data_rate psdu_data_rate;
	enum fira_mac_fcs_type mac_fcs_type;
	enum fira_prf_mode prf_mode;
	enum fira_phr_data_rate phr_data_rate;
	/* STS and crypto. */
	enum fbs_sts_mode sts_config;
	uint8_t vendor_id[FIRA_VENDOR_ID_SIZE];
	uint8_t static_sts_iv[FIRA_STATIC_STS_IV_SIZE];
	uint8_t session_key_len;
	uint8_t *session_key;
	uint32_t sub_session_id;
	uint8_t sub_session_key_len;
	uint8_t *sub_session_key;
	bool key_rotation;
	uint8_t key_rotation_rate;
	bool mac_payload_encryption;
	/* Report parameters. */
	bool report_tof;
	bool report_aoa_azimuth;
	bool report_aoa_elevation;
	bool report_aoa_fom;
	bool report_rssi;
	struct fira_measurement_sequence *meas_seq;
	bool report_diagnostics;
	bool report_psdus;
	enum fira_ranging_diagnostics_frame_report_flags diagnostic_report_flags;
	/* Misc */
	enum fira_sts_length sts_length;
	enum fira_session_info_ntf_config session_info_ntf_config;
	uint32_t near_proximity_config_cm;
	uint32_t far_proximity_config_cm;
	int32_t lower_aoa_bound_config_azimuth_2pi;
	int32_t upper_aoa_bound_config_azimuth_2pi;
	int16_t lower_aoa_bound_config_elevation_2pi;
	int16_t upper_aoa_bound_config_elevation_2pi;
	/* Data transfer. */
	uint8_t link_layer_mode;
	uint8_t data_repetition_count;
	uint8_t data_transfer_status_ntf_config;
	uint8_t in_band_termination_attempt_count;
	/* Parameters specific for DL-TDoA. */
	bool dt_anchor_is_reference;
	bool dl_tdoa_responder_tof;
	enum fira_dl_tdoa_ranging_method dt_anchor_ranging_method;
	enum fira_owr_dtm_timestamp_type dt_anchor_tx_timestamp_type;
	enum fira_owr_dtm_timestamp_len dt_anchor_tx_timestamp_length;
	bool dt_anchor_hop_count_present;
	bool dt_anchor_cfo_present;
	bool dt_anchor_location_present;
	enum fira_dt_location_coord_system_type dt_anchor_location_type;
	uint8_t dt_anchor_location[FIRA_OWR_DTM_LOCATION_SIZE_MAX];
	bool dt_anchor_active_ranging_rounds_info_present;
	uint8_t dt_tag_block_skipping;
	/* Parameters specific for UL-TDoA. */
	uint32_t ut_tx_interval_dtu;
	uint32_t ut_random_window_dtu;
	uint8_t application_data_endpoint;
	enum fira_owr_utm_timestamp_len ut_tx_timestamp_len;
	enum fira_owr_utm_device_id_len ut_device_id_len;
	uint64_t ut_device_id;
	uint8_t ut_report_config_interval;
	uint8_t ut_report_config_count;
	enum fira_owr_ut_report_config_event ut_report_config_event;
};

/**
 * struct fira_report_info - Report information.
 */
struct fira_report_info {
	/**
	 * @ranging_data: Base address of ranging data per peer, or null
	 * pointer.
	 */
	const struct fira_ranging_info *const *ranging_data;
	/**
	 * @diagnostics: Array of diagnostic collected per slots.
	 */
	const struct fira_diagnostic *const *diagnostics;
	/**
	 * @slots: Array of information slots.
	 */
	const struct fira_slot *const *slots;
	/**
	 * @ranging_specific_report_info: Ranging-specific context report
	 * information.
	 */
	void *ranging_specific_report_info;
	/**
	 * @n_ranging_data: Number of entry in ranging_data above.
	 */
	uint8_t n_ranging_data;
	/**
	 * @n_diagnostics: Number of diagnostics.
	 */
	uint8_t n_diagnostics;
	/**
	 * @n_slots: Number of slots.
	 */
	uint8_t n_slots;
};

/**
 * struct fira_measurement - Measurement which influence diagnostics.
 */
struct fira_measurement {
	/**
	 * @sequence: Measurement sequence parameters.
	 * It's provided by set_parameter.
	 * It's don't change during an access.
	 * It's can be reset on access_done.
	 */
	struct fira_measurement_sequence *sequence;
	/**
	 * @index: Index of the step in sequence array.
	 */
	int index;
	/**
	 * @n_achieved: Number of measurements done inside a step.
	 */
	int n_achieved;
	/**
	 * @n_total_achieved: Total number of measurements done.
	 */
	int n_total_achieved;
};

/**
 * struct fira_session - Session information.
 */
struct fira_session {
	/**
	 * @base: FiRa Based Session context.
	 */
	struct fbs_session base;
	/**
	 * @app: Session application handlers.
	 */
	const struct fira_session_app *app;
	/**
	 * @sequence_number: Session notification counter.
	 */
	uint32_t sequence_number;
	/**
	 * @params: Session parameters, mostly read only while the session is
	 * active.
	 */
	struct fira_session_params params;
	/**
	 * @hrp_uwb_params: HRP UWB parameters, read only while the session is
	 * active.
	 */
	struct mcps802154_hrp_uwb_params hrp_uwb_params;
	/**
	 * @block_stride_len: Stride length indicates how many ranging blocks
	 * will be skipped.
	 * The value is updated at the beginning of an access.
	 */
	int block_stride_len;
	/**
	 * @round_index: Round index used on the last access.
	 */
	int round_index;
	/**
	 * @next_round_index: Next round index a announced in measurement
	 * report message.
	 */
	int next_round_index;
	/**
	 * @n_ranging_round_retry: Number of ranging round failed.
	 * Counter reset on ranging round success.
	 */
	int n_ranging_round_retry;
	/**
	 * @round_hopping_sequence: Round hopping sequence generation context.
	 */
	struct fira_round_hopping_sequence round_hopping_sequence;
	/**
	 * @responder: Group of persistent variables used when the device
	 * is a responder.
	 */
	struct {
		/**
		 * @block_index_sync: Last block index received.
		 */
		int block_index_sync;
		/**
		 * @hopping_mode: True when hopping is enabled on last
		 * measurement frame.
		 */
		bool hopping_mode;
		/**
		 * @next_round_index_valid: True when the next round index
		 * is present in measurement report frame.
		 */
		bool next_round_index_valid;
		/**
		 * @initiator_session_info_ntf_status: session_info_ntf status of
		 * the initiator.
		 */
		enum fira_session_info_ntf_status initiator_session_info_ntf_status;
		/**
		 * @crypto: Crypto related variables in the sub-session.
		 * Only valid if sts_config if either
		 * FBS_STS_MODE_DYNAMIC_INDIVIDUAL_KEY or
		 * FBS_STS_MODE_PROVISIONED_INDIVIDUAL_KEY.
		 */
		struct fira_crypto *crypto;
		/**
		 * @se_key_req: pending dynamic key request.
		 */
		struct fbs_se_key_request *se_key_req;
		/**
		 * @sub_session_id_valid: True when the sub_session_id parameter value is valid.
		 */
		bool sub_session_id_valid;
	} responder;
	/**
	 * @controller: Group of persistent variables used when the device
	 * is a controller.
	 */
	struct {
		/**
		 * @next_block_index: Next block index built on get access with
		 * next round index.
		 * It's only to avoid to rebuild the next round index on next
		 * access, when this last occur in time as block index will
		 * match. Used only in case of round hopping.
		 */
		uint32_t next_block_index;
	} controller;
	/**
	 * @dt_anchor: Group of persistent variables used when the device
	 * is a DT-Anchor.
	 */
	struct fira_session_owr_dt_anchor *dt_anchor;
	/**
	 * @dt_tag: Group of persistent variables used when the device
	 * is a DT-Tag.
	 */
	struct fira_session_owr_dt_tag *dt_tag;
	/**
	 * @ut_tag: Group of persistent variables used when the device
	 * is an UT-Tag.
	 */
	struct fira_session_owr_ut_tag *ut_tag;
	/**
	 * @current_controlees: Current list of controlees.
	 */
	struct qlist_head current_controlees;
	/**
	 * @n_current_controlees: Number of elements in the list of current
	 * controlees.
	 */
	uint8_t n_current_controlees;
	/**
	 * @measurements: Measurement configurations which influence
	 * diagnostics.
	 * TODO: remove the 's' in another next commit.
	 */
	struct fira_measurement measurements;
	/**
	 * @rx_ctx: Custom rx context for all responders.
	 */
	void *rx_ctx;
	/**
	 * @crypto: crypto related variables in the session.
	 */
	struct fira_crypto *crypto;
	/**
	 * @se_key_req: pending dynamic key request.
	 */
	struct fbs_se_key_request *se_key_req;
	/**
	 * @sts: sts related variables.
	 */
	struct {
		/**
		 * @last_rotation_block_index: index to the last block where the
		 * rotation occurred.
		 */
		uint32_t last_rotation_block_index;
	} sts;
	/**
	 * @data_size_in_ranging_round: maximum MDSDU size that might be sent
	 * on a single round.
	 */
	uint16_t data_size_in_ranging_round;
	/**
	 * @app_data_payload_snd: Application data payload outgoing data info.
	 */
	struct {
		/**
		 * @total_length: Total data length of payload segments.
		 */
		uint32_t total_length;
		/**
		 * @payload_segments: Current list of payload segments to be
		 * sent by order of arrival.
		 * @see struct fira_data_payload_segment.
		 */
		struct qlist_head payload_segments;
		/**
		 * @n_payload_segments: Number of elements in the
		 * payload_segments list.
		 */
		uint8_t n_payload_segments;
		/**
		 * @sequence_number: Sequence number for whole Data Message.
		 */
		uint16_t sequence_number;
		/**
		 * @status: Status of the application data transfer.
		 * See &enum fira_data_transfer_status.
		 */
		uint8_t status;
		/**
		 * @tx_count: Counter for repetitions of application data
		 * transmission.
		 */
		uint8_t tx_count;
		/**
		 * @is_repetition_completed: Flag to signalize that all
		 * scheduled data transfer repetitions are completed.
		 */
		bool is_repetition_completed;
		/**
		 * @all_segments_received: Flag to indicate that all required
		 * segments of application data payload were received. Used to
		 * signalize if payload is ready to be transmitted.
		 */
		bool all_segments_received;
	} app_data_payload_snd;

	/**
	 * @app_data_payload_rcv: Application data payload incoming data info.
	 */
	struct {
		/**
		 * @buffers: List of application data buffers per source
		 * address. See &fira_data_payload_rcv_buffer.
		 */
		struct qlist_head buffers;
		/**
		 * @n_buffers: Number of devices that sent application data
		 * segments in the current ranging round.
		 */
		uint8_t n_buffers;
	} app_data_payload_rcv;
	/**
	 * @frame_in_block_dropped: Flag to signalize if some of the rframes
	 * were not properly received. Used to check if Data Payload is
	 * complete.
	 */
	bool frame_in_block_dropped;
	/**
	 * @contention_based: Contention based related data.
	 */
	struct fira_contention_based_data *contention_based;
	/**
	 * @twr_data_trx: TWR specific session variables.
	 */
	struct fira_twr_data_transfer *twr_data_trx;
	/**
	 * @primary_session: Pointer to the HUS Primary to which the phase is bound. Only applicable
	 * for a phase. For now, a HUS Phase can have only one bound HUS Primary.
	 */
	struct fira_session *primary_session;
	/**
	 * @phase_sts_index: When the session is a HUS Phase, the STS index is a counter
	 * maintained by the Phase Controller. This counter is incremented on Controller side and
	 * received from the Control Message on the Controlee.
	 */
	uint32_t phase_sts_index;
	/**
	 * @unbind_request: Request to unbind the HUS Phase.
	 */
	bool unbind_request;
};

static inline struct fira_session *fbs_to_fira(const struct fbs_session *fbs_session)
{
	/* Only the argument is const to avoid to have 2 functions. */
	return qparent_of(fbs_session, struct fira_session, base);
}

static inline struct fbs_region *fira_to_local(const struct fira_session *fira_session)
{
	return fira_session->base.local;
}

static inline struct fira_workspace *fira_to_workspace(const struct fira_session *fira_session)
{
	return &fira_to_local(fira_session)->shared_ws.fira;
}

static inline bool fira_session_is_phase(const struct fira_session *fira_session)
{
	return fira_session->base.stype == QUWBS_FBS_SESSION_TYPE_RANGING_ONLY_PHASE ||
	       fira_session->base.stype == QUWBS_FBS_SESSION_TYPE_IN_BAND_DATA_PHASE ||
	       fira_session->base.stype == QUWBS_FBS_SESSION_TYPE_RANGING_WITH_DATA_PHASE;
}

/**
 * fira_session_new() - Create a new session.
 * @llhw: Low layer hardware context.
 * @session_id: The session identifier.
 * @stype: Session type.
 *
 * Return: The new session or NULL on error.
 */
struct fbs_session *fira_session_new(struct mcps802154_llhw *llhw, uint32_t session_id,
				     enum quwbs_fbs_session_type stype);

/**
 * fira_session_free() - Delete a session.
 * @session: Session to remove, must be inactive.
 */
void fira_session_free(struct fira_session *session);

/*
 * fira_session_is_init_state() - Is the session in INIT state.
 * @session: FiRa session context.
 *
 * return: True when all mandatory parameters have been set.
 */
bool fira_session_is_init_state(const struct fira_session *session);

/**
 * fira_session_is_ready() - Test whether a session is ready to be started.
 * @session: Session to test.
 *
 * Return: The reason code.
 */
enum quwbs_fbs_reason_code fira_session_is_ready(const struct fira_session *session);

/**
 * fira_session_report() - Report state change and ranging result for a session.
 * @session: Session to report.
 * @report_info: report information to exploit for the reporting.
 */
void fira_session_report(struct fira_session *session, const struct fira_report_info *report_info);

/**
 * fira_session_data_transfer_status() - Report status of data transfer.
 * Generate message for SESSION_DATA_TRANSFER_STATUS_NTF.
 * @session: Session to report.
 * @seq_nr: Sequence number to report.
 * @status: Status to report.
 * @tx_count: TX count value to report.
 */
void fira_session_data_transfer_status(struct fira_session *session, uint16_t seq_nr,
				       uint8_t status, uint8_t tx_count);

/**
 * fira_session_report_data_credit_ntf() - Reports data credit availability.
 * Generate message for SESSION_DATA_CREDIT_NTF.
 * @session: Session to report.
 * @credit_available: false: data credit not available, true: data credit
 * available.
 */
void fira_session_report_data_credit_ntf(struct fira_session *session, bool credit_available);

/**
 * fira_session_get_meas_seq_step() - Get current measurement step.
 * @session: FiRa session context.
 *
 * Return: Current measurement sequence step for given session.
 */
static inline const struct fira_measurement_sequence_step *
fira_session_get_meas_seq_step(const struct fira_session *session)
{
	const struct fira_measurement *meas = &session->measurements;
	const struct fira_measurement_sequence *seq = meas->sequence;

	return &seq->steps[meas->index];
}

/**
 * fira_rctu_to_cm() - Convert TOF from rctu to cm.
 * @llhw: Low layer hardware context.
 * @tof_rctu: Value in rctu to convert.
 *
 * Return: Value converted to cm.
 */
static inline int32_t fira_rctu_to_cm(const struct mcps802154_llhw *llhw, int64_t tof_rctu)
{
	const int32_t distance_mm = mcps802154_rctu_to_mm(llhw, tof_rctu);
	const int32_t distance_mm_rounded = distance_mm + (tof_rctu > 0 ? 5 : -5);

	return distance_mm_rounded / 10;
}

/**
 * fira_session_update_key() - Update FiRa session or sub-session key.
 * @current_key: Current key.
 * @new_key: New key.
 * @current_key_len: Current key length.
 * @new_key_len: New key length.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_session_update_key(uint8_t **current_key, uint8_t *new_key, uint8_t *current_key_len,
				  uint8_t new_key_len);

/**
 * fira_session_is_device_synchronized() - Checks if the device is still
 * synchronized.
 * @session: Session context.
 * @block_index_sync: Last synchronized block index.
 *
 * Return: True if device is synchronized.
 */
bool fira_session_is_device_synchronized(const struct fira_session *session, int block_index_sync);

/**
 * fira_session_forward_to_next_ranging_block() - Update the session to forward
 * to the next ranging block (possibly skipping a few).
 * @session: FiRa session context.
 * @n_ranging_blocks: Number of ranging blocks to be skipped.
 */
void fira_session_forward_to_next_ranging_block(struct fira_session *session, int n_ranging_blocks);

/**
 * fira_session_report_rcv_app_payload() - Send SESSION_DATA_MESSAGE_RCV
 * notification with all the available received segments during the ranging
 * round.
 * @session: Current session context.
 * @data_status: Signalize if error encountered during reception of the payload.
 */
void fira_session_report_rcv_app_payload(struct fira_session *session,
					 enum fira_data_message_status data_status);

/**
 * fira_session_select_measurement() - Select measurement sequence from param.
 * @session: FiRa session context.
 */
void fira_session_select_measurement(struct fira_session *session);

/**
 * fira_session_update_measurement() - Update measurement for an access_done.
 * @session: FiRa session context.
 */
void fira_session_update_measurement(struct fira_session *session);

/**
 * fira_session_store_psdu() - Store the PSDU if needed and clean it.
 * @session: FiRa session context.
 * @slot_idx: Current slot.
 * @psdu: PSDU to report.
 */
void fira_session_store_psdu(struct fira_session *session, int slot_idx, struct sk_buff *psdu);

/**
 * fira_session_update_rx_margin_duration_dtu() - Initialize the rx margin
 * depending of block_duration_rx_margin_ppm
 * @session: FiRa session context.
 */
void fira_session_update_rx_margin_duration_dtu(struct fira_session *session);

/**
 * fira_session_receive_dynamic_key() - Session receives value from key manager.
 * @session: FiRa session context.
 * @se_key_req: FiRa dynamic key request descriptor.
 * @status: The error status from the key manager.
 */
void fira_session_receive_dynamic_key(struct fira_session *session,
				      struct fbs_se_key_request *se_key_req, enum qerr status);

/**
 * fira_session_active_enter() - Session becomes active.
 * @session: FiRa session context.
 */
void fira_session_active_enter(struct fira_session *session);

/**
 * fira_session_active_leave() - Session becomes inactive.
 * @session: FiRa session context.
 * @rc: Reason code to put in the notification message.
 */
void fira_session_active_leave(struct fira_session *session, enum quwbs_fbs_reason_code rc);

/**
 * fira_session_set_remote_peers_addr() - Add remote peer address.
 * @session: FiRa session context.
 * @nl_addrs: Netlink attribute containing the addresses parameters.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_session_set_remote_peers_addr(struct fira_session *session,
					     const struct nlattr *nl_addrs);

/**
 * fira_session_report_local_aoa() - Add AoA measurements to the message.
 * @msg: Message.
 * @nest_attr_id: ID in the nest.
 * @info: Report AoA info.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_session_report_local_aoa(struct sk_buff *msg, int nest_attr_id,
					const struct fira_local_aoa_info *info);

/**
 * fira_session_build_ranging_sts_quality() - Compute Sts quality metric.
 * @ranging_info: Ranging information needed to build the returned value.
 *
 * Return: Sts quality metric from 1 to 255.
 */
uint8_t fira_session_build_ranging_sts_quality(const struct fira_ranging_info *ranging_info);

/* TODO: delete this when the report is handled from deferred. */
void fira_deferred_psdu(struct fira_workspace *ws);
