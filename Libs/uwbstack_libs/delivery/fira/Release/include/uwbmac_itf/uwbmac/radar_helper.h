/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "qerr.h"
#include "uwbmac/fbs_helper.h"

#include <stdint.h>

struct uwbmac_context;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * struct radar_capabilities - Radar capabilities.
 *
 * This structure contains the Radar capabilities as reported by the MAC.
 */
struct radar_capabilities {
	uint8_t radar_support;
};

/*
 * struct radar_sample - Used to store radar data of individual sample.
 */
struct radar_sample {
	/**
	 * @sequence_number: Sequence number of the sample.
	 */
	uint32_t sequence_number;
	/**
	 * @timestamp: Timestamp of the sample.
	 */
	uint32_t timestamp;
	/**
	 * @cir_real_imag_len: Length of CIR data segment.
	 */
	int cir_real_imag_len;
	/**
	 * @cir_real_imag: CIR data segment.
	 */
	uint8_t *cir_real_imag;
};

/**
 * struct radar_result - Used for conveying Radar result.
 */
struct radar_result {
	/**
	 * @session_handle: Session handle of the radar result.
	 */
	uint32_t session_handle;
	/**
	 * @status: Zero if ok, or error reason.
	 */
	uint8_t status;
	/**
	 * @sweep_offset: Offset from 0cm distance in samples.
	 */
	int16_t sweep_offset;
	/**
	 * @bytes_per_sample: Bytes per radar sample.
	 */
	uint8_t bytes_per_sample;
	/**
	 * @samples_per_sweep: Number of taps in particular sweep (CIR). Each sample is a complex
	 * number x + iy. Where length of x and y is equal to each other and cumulatively is equal
	 * to &radar_result.bytes_per_sample field.
	 */
	uint8_t samples_per_sweep;
	/**
	 * @number_of_sweeps: Number of sweeps (CIRs) in notification
	 */
	uint8_t number_of_sweeps;
	/**
	 * @radar_samples: Radar samples structure.
	 */
	struct radar_sample *radar_samples;
};

/**
 * struct radar_timing_params - Radar session timing parameters.
 */
struct radar_timing_params {
	/**
	 * @burst_period_ms: Duration between two consecutive radar
	 * bursts in ms.
	 */
	uint32_t burst_period_ms;
	/**
	 * @sweep_period_rstu: Duration between two consecutive radar
	 * sweeps in RSTU.
	 */
	uint16_t sweep_period_rstu;
	/**
	 * @sweeps_per_burst: Number of sweeps within a burst.
	 */
	uint8_t sweeps_per_burst;
};

/* struct radar_parameters  - radar session parameters */
struct radar_session_parameters {
	/**
	 * @channel_number: UWB channel for this session.
	 *
	 * See &enum radar_channel_number
	 */
	uint8_t channel_number;
	/**
	 * @rframe_config: STS Packet configuration.
	 *
	 * See &enum radar_rframe_config
	 */
	uint8_t rframe_config;
	/**
	 * @preamble_code_index:
	 *
	 * UWB preamble code index.
	 *
	 * Possible values:
	 * - 2-3: PRF 16MHz
	 * - 9-24: BPRF (64 Mhz)
	 * - 25-32: HPRF (125 MHz)
	 */
	uint8_t preamble_code_index;
	/**
	 * @preamble_duration:
	 *
	 * See &enum radar_preamble_duration.
	 */
	uint8_t preamble_duration;
	/**
	 * @session_priority:
	 *
	 * Priority of the session. Value range 1-100, 255.
	 */
	uint8_t session_priority;
	/**
	 * @radar_timing_params: Struct to handle
	 * packing/unpacking timing params from uci to nl message and
	 * back
	 */
	struct radar_timing_params timing_params;
	/**
	 * @samples_per_sweep: Number of samples per sweep.
	 *
	 */
	uint8_t samples_per_sweep;
	/**
	 * @sweep_offset: Number of samples offset before First Path.
	 *
	 */
	int16_t sweep_offset;
	/**
	 * @bytes_per_sample: Bytes per radar sample.
	 *
	 * See &enum bytes_per_sample
	 */
	uint8_t bytes_per_sample;
	/**
	 * @radar_data_type: Type of radar data to be reported in
	 * RADAR_DATA_MESSAGE, see &enum radar_data_type
	 */
	uint8_t radar_data_type;
	/**
	 * @antenna_group_nb: TX antenna group index.
	 */
	uint8_t antenna_group_nb;
	/**
	 * @tx_profile_idx: Index of TX profile.
	 *
	 * Possible values:
	 *
	 * See &enum radar_tx_profile.
	 */
	uint8_t tx_profile_idx;
	/**
	 * @number_of_bursts: Number of bursts to be performed in radar session.
	 *
	 * Default value:
	 *
	 *  0x00 = Unlimited bursts.
	 */
	uint16_t number_of_bursts;
};

/**
 * typedef radar_helper_notification_cb - Notification callback type.
 * @result: Radar ranging result.
 * @user_data: User data pointer given to radar_helper_open.
 */
typedef void (*radar_helper_notification_cb)(const struct radar_result *result, void *user_data);

/*
 * struct radar_context - RADAR helper context.
 */
struct radar_context {
	void *user_data;
	struct fbs_helper fbs_helper;
	radar_helper_notification_cb notification_cb;
	fbs_helper_session_status_ntf_cb session_status_ntf_cb;
};

/**
 * radar_helper_open() - Initialize the internal resources of the helper.
 * @ctx: Radar helper context.
 * @uwbmac: UWBMAC context.
 * @notification_cb: Callback to call when a notification is available.
 * @scheduler: Scheduler name used for the radar session.
 * @region_id: Region ID.
 * @user_data: User data pointer to give back in callback.
 *
 * NOTE: This function must be called first. @radar_helper_close must be called
 * at the end of the application to ensure resources are freed.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr radar_helper_open(struct radar_context *ctx, struct uwbmac_context *uwbmac,
			    radar_helper_notification_cb notification_cb, const char *scheduler,
			    int region_id, void *user_data);

/**
 * radar_helper_close() - Free all internal resources of the helper.
 * @ctx: Radar helper context.
 */
void radar_helper_close(struct radar_context *ctx);

/**
 * radar_helper_set_session_status_ntf_cb() - Register a callback to get session status
 * notification.
 * @ctx: Radar helper context.
 * @cb: Callback to use to get the session status ntf (can be NULL)
 */
enum qerr radar_helper_set_session_status_ntf_cb(struct radar_context *ctx,
						 fbs_helper_session_status_ntf_cb cb);

/**
 * radar_helper_set_scheduler() - Set the scheduler and the region of RADAR.
 * @ctx: Radar helper context.
 *
 * NOTE: This function must be called while the UWB MAC is stopped.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr radar_helper_set_scheduler(struct radar_context *ctx);

/**
 * radar_helper_get_capabilities() - Get the Radar region capabilities.
 * @ctx: Radar helper context.
 * @capabilites: Radar capabilites.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr radar_helper_get_capabilities(struct radar_context *ctx,
					struct radar_capabilities *capabilites);

/**
 * radar_helper_init_session() - Initialize a radar session.
 * @ctx: Radar helper context.
 * @session_handle: Session identifier.
 * @rsp: Session init response message information.
 *
 * This function must be called first to create and initialize the radar
 * session.
 *
 * Return: 0 or error.
 */
int radar_helper_init_session(struct radar_context *ctx, uint32_t session_handle,
			      struct fbs_session_init_rsp *rsp);

/**
 * radar_helper_start_session() - Start a radar session.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 *
 * This function must be called after radar session was initialized.
 *
 * Return: 0 or error.
 */
int radar_helper_start_session(struct radar_context *ctx, uint32_t session_handle);

/**
 * radar_helper_stop_session() - Stop a radar session.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 *
 * This function stop the session ranging.
 *
 * Return: 0 or error.
 */
int radar_helper_stop_session(struct radar_context *ctx, uint32_t session_handle);

/**
 * radar_helper_deinit_session() - Deinitialize a radar session.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 *
 * This function is called to free all memory allocated by the session.
 * This function must be called when the session is stopped.
 *
 * Return: 0 or error.
 */
int radar_helper_deinit_session(struct radar_context *ctx, uint32_t session_handle);

/**
 * radar_helper_get_session_parameters() - Get all session parameters at once.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @session_params: Session parameters.
 *
 * Return: 0 or error.
 */
int radar_helper_get_session_parameters(struct radar_context *ctx, uint32_t session_handle,
					struct radar_session_parameters *session_params);

/**
 * radar_helper_session_get_state() - Get session state.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @state: Session state.
 *
 * Return: 0 or error.
 */
int radar_helper_session_get_state(struct radar_context *ctx, uint32_t session_handle, int *state);

/**
 * radar_helper_set_session_parameters() - Set all session parameters at once.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @session_params: Session parameters.
 *
 * Return: 0 or error.
 */
int radar_helper_set_session_parameters(struct radar_context *ctx, uint32_t session_handle,
					const struct radar_session_parameters *session_params);

/**
 * radar_helper_set_channel_number() - Set channel number session parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @channel_number: Value of channel number to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_channel_number(struct radar_context *ctx, uint32_t session_handle,
				    uint8_t channel_number);

/**
 * radar_helper_set_preamble_duration() - Set preamble duration session
 * parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @preamble_duration: Value of preamble duration to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_preamble_duration(struct radar_context *ctx, uint32_t session_handle,
				       uint8_t preamble_duration);

/**
 * radar_helper_set_burst_period_ms() - Set burst_period_ms session parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session identifier.
 * @burst_period_ms: Value of burst_period_ms to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_burst_period_ms(struct radar_context *ctx, uint32_t session_handle,
				     uint32_t burst_period_ms);

/**
 * radar_helper_set_sweep_period_rstu() - Set sweep_period_rstu session
 * parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session identifier.
 * @sweep_period_rstu: Value of sweep_period_rstu to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_sweep_period_rstu(struct radar_context *ctx, uint32_t session_handle,
				       uint16_t sweep_period_rstu);

/**
 * radar_helper_set_sweeps_per_burst() - Set sweeps_per_burst session parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session identifier.
 * @sweeps_per_burst: Value of sweeps_per_burst to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_sweeps_per_burst(struct radar_context *ctx, uint32_t session_handle,
				      uint8_t sweeps_per_burst);

/**
 * radar_helper_set_radar_data_type() - Set radar data type session parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @radar_data_type: Value of radar data type to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_radar_data_type(struct radar_context *ctx, uint32_t session_handle,
				     uint8_t radar_data_type);

/**
 * radar_helper_set_antenna_group_number() - Set antenna group nb session
 * parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @antenna_group_nb: Value of antenna group nb to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_antenna_group_number(struct radar_context *ctx, uint32_t session_handle,
					  uint8_t antenna_group_nb);

/**
 * radar_helper_set_samples_per_sweep() - Set number of samples in seep session parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @samples_per_sweep: Value of number of samples to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_samples_per_sweep(struct radar_context *ctx, uint32_t session_handle,
				       uint8_t samples_per_sweep);

/**
 * radar_helper_set_sweep_offset() - Set sweep offset session parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @sweep_offset: Value of sweep offset to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_sweep_offset(struct radar_context *ctx, uint32_t session_handle,
				  int16_t sweep_offset);

/**
 * radar_helper_set_bytes_per_sample() - Set bytes per sample session parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @bytes_per_sample: Value of bytes per sample to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_bytes_per_sample(struct radar_context *ctx, uint32_t session_handle,
				      uint8_t bytes_per_sample);

/**
 * radar_helper_set_tx_profile_idx() - Set index of tx profile session
 * parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @tx_profile_idx: Value of tx profile index to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_tx_profile_idx(struct radar_context *ctx, uint32_t session_handle,
				    uint8_t tx_profile_idx);

/**
 * radar_helper_set_number_of_bursts() - Set number of bursts session parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @number_of_bursts: Number of bursts to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_number_of_bursts(struct radar_context *ctx, uint32_t session_handle,
				      uint16_t number_of_bursts);

/**
 * radar_helper_set_rframe_config() - Set rframe config session parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @rframe_config: Value of rframe to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_rframe_config(struct radar_context *ctx, uint32_t session_handle,
				   uint8_t rframe_config);

/**
 * radar_helper_set_preamble_code_index() - Set preamble code index session
 * parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @preamble_code_index: Value of preamble code index to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_preamble_code_index(struct radar_context *ctx, uint32_t session_handle,
					 uint8_t preamble_code_index);

/**
 * radar_helper_set_session_priority() - Set session priority parameter.
 * @ctx: Radar helper context.
 * @session_handle: Session handle.
 * @session_priority: Value of session priority to be set.
 *
 * Return: 0 or error.
 */
int radar_helper_set_session_priority(struct radar_context *ctx, uint32_t session_handle,
				      uint8_t session_priority);

#ifdef __cplusplus
}
#endif
