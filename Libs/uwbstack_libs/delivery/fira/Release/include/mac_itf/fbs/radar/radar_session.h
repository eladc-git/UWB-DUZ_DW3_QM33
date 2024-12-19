/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_region.h"
#include "fbs/fbs_session.h"
#include "fbs/fbs_session_runtime.h"
#include "mcps802154_i.h"
#include "net/mcps802154.h"
#include "net/radar_region_params.h"
#include "qerr.h"
#include "qtils.h"
#include "quwbs/fbs/defs.h"
#include "tmp_fbs_region.h"

#include <stdbool.h>
#include <stdint.h>

enum radar_calibration_state {
	RADAR_CALIBRATED = 0,
	RADAR_NOT_CALIBRATED,
	RADAR_CALIBRATED_IN_CURRENT_BURST,
};

/**
 * struct radar_session_runtime - Radar session runtime context.
 */
struct radar_session_runtime {
	/**
	 * @runtime: inherit from base runtime struct.
	 */
	struct fbs_session_runtime runtime;
	/**
	 * @max_access_duration_dtu: maximum access duration in DTU.
	 */
	int32_t max_access_duration_dtu;
};

/**
 * struct radar_ranging_info - Ranging information.
 */
struct radar_ranging_info {
	/**
	 * @next: Pointer to next radar_ranging_info struct.
	 */
	struct radar_ranging_info *next;
	/**
	 * @status: Success or failure reason.
	 */
	enum radar_ranging_status status;
	/**
	 * @seq_nb: Counter of a single radar measurement (CIR or RX fail).
	 * Starting with 0 when the radar session is started.
	 */
	uint32_t seq_nb;
	/**
	 * @timestamp_rstu: Timestamp of received frame associated with current
	 * CIR. RSTU units.
	 */
	uint32_t timestamp_rstu;
	/**
	 * @rx_cir: Structure containing CIR measurement result.
	 */
	struct mcps802154_rx_segment_cir rx_cir;
};

struct radar_ranging_info_list {
	/**
	 * @head: Head of the radar_ranging_info_list.
	 */
	struct radar_ranging_info *head;
	/**
	 * @current: Pointer to currently processed radar_ranging_info_list
	 * element.
	 */
	struct radar_ranging_info *current;
	/**
	 * @status: Combined status of all list elements.
	 */
	enum radar_ranging_status status;
};

/* Session configuration structure */
struct radar_session_params {
	uint8_t channel_number;
	uint8_t rframe_config;
	uint8_t preamble_code_index;
	uint8_t preamble_duration;
	uint8_t session_priority;
	uint32_t burst_period_ms;
	uint16_t sweep_period_rstu;
	uint8_t sweeps_per_burst;
	uint8_t radar_data_type;
	uint8_t antenna_group_nb;
	uint8_t samples_per_sweep;
	int16_t sweep_offset;
	uint8_t bytes_per_sample;
	uint8_t tx_profile_idx;
	uint8_t short_range_profile_idx;
	uint16_t number_of_bursts;
};

struct radar_session {
	/** @base: FBS session context. */
	struct fbs_session base;
	/**
	 * @params: Radar session parameters.
	 */
	struct radar_session_params params;
	/**
	 * @hrp_uwb_params: HRP UWB parameters, read only while the session is
	 * active.
	 */
	struct mcps802154_hrp_uwb_params hrp_uwb_params;
	/**
	 * @radar_ranging_info_list: Information on ranging for the current
	 * session. Number of ranging data is equal to session param -
	 * RADAR_TIMING_PARAMS.SWEEP_PER_BURST.
	 */
	struct radar_ranging_info_list radar_ranging_info_list;
	/**
	 * @measurement_nb: Counter of conducted measurements. No matter if RX succeed
	 * of failed. Starts from zero.
	 */
	uint32_t measurement_nb;
	/**
	 * @sweep_nb: Counter of sweeps in burst. Zeroed every access.
	 */
	uint8_t sweep_nb;
	/**
	 * @range_profile_idx_cnt: Index of row in tx_power LUT (kept in LLHW)
	 * corresponded to short range TX profile.
	 */
	int short_range_profile_idx;
	/**
	 * @calibration_state: Calibration state.
	 */
	enum radar_calibration_state calibration_state;
	/*
	 * @is_prebuffer_forced_on: Is the prebuffer set to "force on" (PLL) mode.
	 */
	bool is_prebuffer_forced_on;
	/*
	 * @is_sweep_ongoing: indication if sweep is currently performed.
	 */
	bool is_sweep_ongoing;
	/**
	 * @is_configured: Is session configured.
	 */
	bool is_configured;
};

static inline struct radar_session *fbs_to_radar(const struct fbs_session *fbs_session)
{
	/* Only the argument is const to avoid to have 2 functions. */
	return qparent_of(fbs_session, struct radar_session, base);
}

static inline struct fbs_region *radar_to_fbs_region(const struct radar_session *session)
{
	return session->base.local;
}

static inline struct mcps802154_llhw *radar_to_llhw(const struct radar_session *session)
{
	return radar_to_fbs_region(session)->tmp_local.llhw;
}

static inline struct mcps802154_local *radar_to_local(const struct radar_session *session)
{
	return llhw_to_local(radar_to_llhw(session));
}

/**
 * radar_session_access_done() - Access end of the session.
 * @session: Radar session context.
 */
void radar_session_access_done(struct radar_session *session);

/**
 * radar_session_start() - Start the session.
 * @session: Radar session context.
 *
 * Return: QERR_SUCCESS on success or an error otherwise.
 */
enum qerr radar_session_start(struct radar_session *session);

/**
 * radar_session_leave_active() - Session becomes inactive.
 * @session: Radar session context.
 * @rc: Reason code forwarded to SESSION_STATUS notification.
 */
void radar_session_leave_active(struct radar_session *session, enum quwbs_fbs_reason_code rc);

/*
 * radar_session_is_init_state() - Is the session in INIT state.
 * @session: Radar session context.
 *
 * return: True when all mandatory parameters have been set.
 */
bool radar_session_is_init_state(const struct radar_session *session);

/**
 * radar_session_get_runtime() - Retrieve session Radar session runtime
 * Context.
 * @session: Session context.
 *
 * Return: Radar session runtime context.
 */
static inline struct radar_session_runtime *
radar_session_get_runtime(const struct radar_session *session)
{
	struct fbs_session_runtime *runtime = session->base.runtime;
	return qparent_of(runtime, struct radar_session_runtime, runtime);
}
