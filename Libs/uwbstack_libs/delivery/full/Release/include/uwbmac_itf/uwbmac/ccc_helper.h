/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <net/ccc_region_nl.h>
#include <net/fbs_region_params.h>
#include <qerr.h>
#include <stdbool.h>
#include <stdint.h>
#include <uwbmac/fbs_helper.h>

struct uwbmac_context;
struct uwbmac_buf;

#ifdef __cplusplus
extern "C" {
#endif

/* Compatibility name for Other repositories. */
#define ccc_helper_session_status_ntf fbs_helper_session_status_ntf

#define CCC_PARAM_CAPABILITY_MAX_LEN 20
#define CCC_NB_SLOT_MAX 14
/*
 * struct ccc_capabilities - CCC capabilities.
 *
 * This structure contains the CCC capabilities as reported by the MAC.
 */
struct ccc_capabilities {
#define F(name) bool has_##name

#define P(name, type) \
	F(name);      \
	type name

#define T(name, type) \
	F(name);      \
	type name[CCC_PARAM_CAPABILITY_MAX_LEN]

	P(chaps_per_slot, uint8_t);
	P(sync_codes, uint32_t);
	P(hopping_config_modes_and_sequences, uint8_t);
	P(channels, uint8_t);
	T(versions, uint16_t);
	T(uwb_configs, uint8_t);
	T(pulse_shape_combos, uint8_t);
	P(ran_multiplier, uint32_t);

#undef T
#undef P
#undef F
};

/**
 * struct ccc_ranging_measurements - CCC ranging measurements.
 */
struct ccc_ranging_measurements {
	/**
	 * @status: Zero if ok, 1 otherwise.
	 */
	uint8_t status;
	/**
	 * @slot_index: slot index
	 */
	uint8_t slot_index;
	/**
	 * @timestamp: timestamp of the slot
	 */
	uint64_t timestamp;
};

/**
 * struct ccc_ranging_results - ccc ranging results.
 */
struct ccc_ranging_results {
	/**
	 * @session_handle: Session handle of the ranging result.
	 */
	uint32_t session_handle;
	/**
	 * @sts_index: Current STS index.
	 */
	uint32_t sts_index;
	/**
	 * @block_index: Index of current block.
	 */
	uint16_t block_index;
	/**
	 * @round_index: Index of current round.
	 */
	uint16_t round_index;
	/**
	 * @n_measurements: Number of measurements stored in the measurements
	 * table.
	 */
	int n_measurements;
	/**
	 * @measurements: Ranging measurements information.
	 */
	struct ccc_ranging_measurements measurements[CCC_NB_SLOT_MAX];
	/**
	 * @psdus_report: Report containing all the psdus.
	 */
	struct uwbmac_buf *psdus_report;
};

struct ccc_helper_session_cfg {
	enum ccc_device_type device_type;
	uint64_t ccc_uwb_time0;
	uint32_t uwb_session_id;
	uint32_t sts_index0;
	uint32_t hop_mode_key;
	uint16_t selected_protocol_version;
	uint16_t selected_uwb_config_id;
	uint8_t selected_uwb_channel;
	uint8_t hop_config_bitmask;
	uint8_t sync_code_index;
	uint32_t tBlock_ms;
	uint16_t slot_duration_rstu;
	uint8_t number_responders_nodes;
	uint8_t number_slot_per_round;
	uint8_t selected_pulse_shape_combo;
	uint16_t ursk_ttl;
	uint8_t device_role;
	uint8_t multi_node_mode;
	uint8_t key_rotation;
	uint8_t session_info_ntf_config;
	uint8_t ranging_round_control;
	uint8_t report_psdus;
	struct fira_measurement_sequence *meas_seq;
	uint32_t current_sts_index;
	bool mac_payload_encryption;
	uint8_t mac_mode;
};

union ccc_helper_notification {
	const struct ccc_ranging_results *ranging_results_ntf;
	const struct fbs_helper_session_status_ntf *session_status_ntf;
};

/**
 * typedef ccc_helper_notification_cb - Notification callback type.
 * @call_id: Notification identifier.
 * @ntf_data: Notification field values.
 * @user_data: User data pointer given to ccc_helper_open.
 */
typedef void (*ccc_helper_notification_cb)(enum fbs_call call_id,
					   const union ccc_helper_notification ntf_data,
					   void *user_data);

/*
 * struct ccc_context - CCC helper context.
 */
struct ccc_context {
	ccc_helper_notification_cb notification_cb;
	void *user_data;
	struct fbs_helper fbs_helper;
};

/**
 * ccc_helper_open() - Initialize the ccc helper context.
 * @ctx: CCC helper context.
 * @uwbmac: UWB MAC context.
 * @notification_cb: Callback function for notifications feedback.
 * @scheduler: Scheduler name to use with the region.
 * @region_id: Region identifier to associate with the region.
 * @user_data: User data pointer to give back in callback.
 *
 * NOTE: This function must be called first.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_open(struct ccc_context *ctx, struct uwbmac_context *uwbmac,
			  ccc_helper_notification_cb notification_cb, const char *scheduler,
			  int region_id, void *user_data);

/**
 * ccc_helper_close() - De-initialize the ccc helper context.
 * @ctx: CCC helper context.
 */
void ccc_helper_close(struct ccc_context *ctx);

/**
 * ccc_helper_set_device_status_cb() - Set the device status callback.
 * @ctx: CCC helper context.
 * @cb: Callback for all device status notifications.
 *
 * NOTE: Temporary api before we inverse dependancy with fbs_helper.
 * Once this is done client will have to directly use
 * fbs_helper_set_device_status_ntf_cb.
 *
 * Return: QERR_SUCCESS on success, on error otherwise.
 */
enum qerr ccc_helper_set_device_status_cb(struct ccc_context *ctx,
					  fbs_helper_device_status_ntf_cb cb);

/**
 * ccc_helper_set_scheduler() - Set the scheduler and open the MAC region.
 * @ctx: CCC helper context.
 *
 * NOTE: This function must be called while the UWB MAC is stopped.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_set_scheduler(struct ccc_context *ctx);

/**
 * ccc_helper_get_capabilities() - Get the CCC region capabilities.
 * @ctx: CCC helper context.
 * @capabilites: CCC capabilites.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_get_capabilities(struct ccc_context *ctx,
				      struct ccc_capabilities *capabilites);

/**
 * ccc_helper_init_session() - Initialize a ccc session.
 * @ctx: CCC helper context.
 * @session_id: Session identifier.
 * @session_type: Session type value.
 * @rsp: Session init response message information.
 *
 * This function must be called first to create and initialize the ccc session.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_init_session(struct ccc_context *ctx, uint32_t session_id,
				  enum quwbs_fbs_session_type session_type,
				  struct fbs_session_init_rsp *rsp);

/**
 * ccc_helper_start_session() - Start a ccc session.
 * @ctx: CCC helper context.
 * @session_handle: Session handle.
 *
 * This function must be called after ccc session was initialized.
 *
 * Return: 0 or positive value on success, negative value on error.
 */
int ccc_helper_start_session(struct ccc_context *ctx, uint32_t session_handle);

/**
 * ccc_helper_stop_session() - Stop a ccc session.
 * @ctx: CCC helper context.
 * @session_handle: Session handle.
 *
 * This function stop the session ranging.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_stop_session(struct ccc_context *ctx, uint32_t session_handle);

/**
 * ccc_helper_deinit_session() - Deinitialize a ccc session.
 * @ctx: CCC helper context.
 * @session_handle: Session handle.
 *
 * This function is called to free all memory allocated by the session.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_deinit_session(struct ccc_context *ctx, uint32_t session_handle);

/**
 * ccc_helper_get_session_parameters() - Get session parameters.
 * @ctx: CCC helper context.
 * @session_handle: Session handle.
 * @session_params: Session parameters.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_get_session_parameters(struct ccc_context *ctx, uint32_t session_handle,
					    void *session_params);

/**
 * ccc_helper_session_get_state() - Get session state.
 * @ctx: CCC helper context.
 * @session_handle: Session handle.
 * @state: Session state.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_session_get_state(struct ccc_context *ctx, uint32_t session_handle,
				       int *state);

/**
 * ccc_helper_set_session_measurement_sequence() - Sets the measurement
 *   sequence.
 * @ctx: CCC helper context.
 * @session_handle: Session handle.
 * @meas_seq: Sequence of measurement sequence steps,
 *   configures the Antenna Flexibility features.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr
ccc_helper_set_session_measurement_sequence(struct ccc_context *ctx, uint32_t session_handle,
					    const struct fira_measurement_sequence *meas_seq);

/**
 * ccc_helper_set_session_channel_number() - Sets the channel number.
 * @ctx: CCC helper context.
 * @session_handle: Session handle.
 * @channel_number: Channel_number.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_set_session_channel_number(struct ccc_context *ctx, uint32_t session_handle,
						uint8_t channel_number);

/**
 * ccc_helper_get_ranging_count() - Get ranging count, the number of times
 * ranging has been attempted during the session.
 * @ctx: CCC helper context.
 * @session_id: Session identifier.
 * @rsp: Ranging count response message information.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_get_ranging_count(struct ccc_context *ctx, uint32_t session_id,
				       struct fbs_ranging_count_rsp *rsp);

/**
 * ccc_helper_get_session_channel_number() - Gets the channel number.
 * @ctx: CCC helper context.
 * @session_handle: Session handle.
 * @channel_number: Channel_number.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr ccc_helper_get_session_channel_number(struct ccc_context *ctx, uint32_t session_handle,
						uint8_t *channel_number);

#define CCC_HELPER_SETTER_PARAM_DECL(parameter_name, type) \
	enum qerr ccc_helper_session_params_set_##parameter_name(struct fbs_helper_msg *msg, type v)

CCC_HELPER_SETTER_PARAM_DECL(device_type, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(ccc_uwb_time0, uint64_t);
CCC_HELPER_SETTER_PARAM_DECL(sts_index0, uint32_t);
CCC_HELPER_SETTER_PARAM_DECL(ranging_round_control, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(hop_mode_key, uint32_t);
CCC_HELPER_SETTER_PARAM_DECL(selected_protocol_version, uint16_t);
CCC_HELPER_SETTER_PARAM_DECL(selected_uwb_config_id, uint16_t);
CCC_HELPER_SETTER_PARAM_DECL(selected_uwb_channel, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(hop_config_bitmask, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(sync_code_index, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(tBlock_ms, uint32_t);
CCC_HELPER_SETTER_PARAM_DECL(slot_duration_rstu, uint16_t);
CCC_HELPER_SETTER_PARAM_DECL(number_responders_nodes, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(number_slot_per_round, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(selected_pulse_shape_combo, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(ursk_ttl, uint16_t);
CCC_HELPER_SETTER_PARAM_DECL(device_role, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(multi_node_mode, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(key_rotation, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(session_info_ntf_config, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(report_psdus, uint8_t);
CCC_HELPER_SETTER_PARAM_DECL(tx_antenna_selection, int8_t);
CCC_HELPER_SETTER_PARAM_DECL(rx_antenna_selection, int8_t);
CCC_HELPER_SETTER_PARAM_DECL(current_sts_index, uint32_t);
CCC_HELPER_SETTER_PARAM_DECL(mac_payload_encryption, bool);
CCC_HELPER_SETTER_PARAM_DECL(mac_mode, uint8_t);
#undef CCC_HELPER_SETTER_PARAM_DECL
enum qerr ccc_helper_session_params_set_ursk(struct fbs_helper_msg *msg, const uint8_t *);

#ifdef __cplusplus
}
#endif
