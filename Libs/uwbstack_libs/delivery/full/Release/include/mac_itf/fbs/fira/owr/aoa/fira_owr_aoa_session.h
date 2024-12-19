/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_session.h"
#include "fbs/fbs_session_runtime.h"
#include "fira/fira_region.h"
#include "fira/fira_session.h"
#include "fira/fira_session_app.h"
#include "net/mcps802154_schedule.h"
#include "qtils.h"

#include <stdbool.h>
#include <stdint.h>

#define FIRA_OWR_AOA_IFI_BGT_MIN_MS 1
#define FIRA_OWR_AOA_IFI_GT_MIN_US 200
/* Implicated by FIRA_OWR_AOA_MEASUREMENT_MESSAGE_NUM_OF_RFRAMES_MASK */
#define FIRA_OWR_AOA_MAX_FRAMES_PER_RR 15

extern const struct fira_session_app fira_owr_aoa_session_app;

/**
 * struct fira_owr_aoa_network_params - Info from/to UWB Message.
 * a) Info received in UWB Message from Advertiser in a case when DEVICE_ROLE is
 * Observer b) Info to send in UWB Message in a case when DEVICE_ROLE is
 * Advertiser.
 */
struct fira_owr_aoa_network_params {
	/**
	 * @inter_frame_interval_ms: Inter-Frame Interval
	 */
	uint8_t inter_frame_interval_ms;
	/**
	 * @rframe_idx: Index of the RFRAME in the current ranging block.
	 */
	uint8_t rframe_idx;
	/**
	 * @n_rframes_in_block: The number of RFRAMEs that are transmitted in
	 * the same ranging round. In OWR for AoA case: one block == one ranging
	 * round.
	 */
	uint8_t n_rframes_in_block;
	/**
	 * @block_index: Index of the current ranging block.
	 */
	uint16_t block_index;
	/**
	 * @block_duration_dtu: Block duration in DTU.
	 */
	uint32_t block_duration_dtu;
};

struct fira_owr_aoa_runtime {
	/**
	 * @runtime: Inheritance from base runtime struct.
	 */
	struct fbs_session_runtime runtime;
	/**
	 * @network_params: Parameters to be transmitted in/received from UWB
	 * Message.
	 */
	struct fira_owr_aoa_network_params network_params;
	union {
		/**
		 * @advertiser: Runtime context parameters specific for
		 * Advertiser role.
		 */
		struct {
			/**
			 * @psdu_max: Maximum possible PSDU in an RFRAME.
			 * This value should be calculated on session start
			 * based on the frame parameters and timing restrictions
			 * between RFRAMEs in a block.
			 */
			int psdu_max;
			/**
			 * @max_data_in_frame: Maximum data per one RFRAME.
			 */
			uint8_t max_data_in_frame;
			/**
			 * @is_data_in_current_rround: A flag indicating if the
			 * current ranging round has at least one RFRAME
			 * containing an application data.
			 */
			bool is_data_in_current_rround;
			/**
			 * @is_data_in_current_rframe: A flag indicating if the
			 * current RFRAME contains an application data.
			 */
			bool is_data_in_current_rframe;
		} advertiser;
		/**
		 * @observer: Runtime context parameters specific for Observer
		 * role.
		 */
		struct {
			/**
			 * @block_index_sync: Last block index received with
			 * success.
			 */
			int block_index_sync;
			/**
			 * @rec_pending_frame_flag: Buffer for Frame Pending
			 * flag in MHR. Should be taken from Frame Control in
			 * MHR and cross-checked with fields Sequence Number
			 * from Frame Control in MHR and Number of RFRAMEs from
			 * Payload IE.
			 */
			bool rec_pending_frame_flag;
			/**
			 * @block_reception_state: Indicator of the reception
			 * state in the current block used in access scheduling.
			 * See &enum fira_owr_aoa_rx_state.
			 */
			int block_reception_state;
			/**
			 * @first_rframe_received: Indicates if the first RFRAME
			 * has been already received in the current block.
			 */
			bool first_rframe_received;
			/**
			 * @max_n_measurements: Maximum number of aggregated
			 * measurements in a block.
			 */
			uint8_t max_n_measurements;
		} observer;
	};
};

/**
 * struct fira_ranging_info_owr_aoa - Ranging information structure which is used
 * to fill OWR for AoA Measurement Result (applicale for Observers).
 */
struct fira_ranging_info_owr_aoa {
	/**
	 * @common: The subset common for all the session modes. Shall be the first field
	 * of this struct.
	 */
	struct fira_ranging_info common;
	/**
	 * @rframe_idx: Frame index of the aggregated measurement.
	 */
	uint8_t rframe_idx;
};

/**
 * fira_owr_aoa_session_get_runtime() - Retrieve session OWR AoA runtime
 * context.
 * @session: Session context.
 *
 * Return: OWR AoA runtime context.
 */
static inline struct fira_owr_aoa_runtime *
fira_owr_aoa_session_get_runtime(const struct fira_session *session)
{
	struct fbs_session_runtime *runtime = session->base.runtime;
	return qparent_of(runtime, struct fira_owr_aoa_runtime, runtime);
}

static inline struct fira_ranging_info_owr_aoa *
fira_owr_aoa_session_get_ranging_info_owr_aoa(const struct fira_ranging_info *ranging_info)
{
	return qparent_of(ranging_info, struct fira_ranging_info_owr_aoa, common);
}

void fira_owr_aoa_session_access_done(struct fira_session *session,
				      enum mcps802154_access_done_reason reason);
