/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_session.h"
#include "fbs/fbs_session_runtime.h"
#include "fira/fira_session.h"
#include "fira/fira_session_app.h"
#include "net/fira_region_params.h"
#include "qlist.h"
#include "qtils.h"

#include <stdbool.h>
#include <stdint.h>

#define FIRA_OWR_DT_ANCHOR_HOP_COUNT_INVALID 0xff
#define FIRA_OWR_DT_ANCHOR_HOP_COUNT_REFERENCE 0

extern const struct fira_session_app fira_owr_dt_anchor_session_app;

struct fira_owr_dt_anchor_runtime {
	/**
	 * @runtime: inherit from base runtime struct.
	 */
	struct fbs_session_runtime runtime;
	/**
	 * @region_started: Flag to manage region/network by reference anchor.
	 */
	bool region_started;
	/**
	 * @tx_timestamp_type: type of tx-timestamp received from Poll DTM.
	 */
	enum fira_owr_dtm_timestamp_type tx_timestamp_type;
	/**
	 * @tx_timestamp_len: bit size of tx-timestamp received from Poll DTM.
	 */
	enum fira_owr_dtm_timestamp_len tx_timestamp_len;
	struct fira_owr_dt_anchor_hop_count {
		/**
		 * @hop_counts: Hop count of this anchor.
		 */
		uint8_t local;
		/**
		 * @hop_counts: Index of received hop_counts.
		 */
		uint8_t current_idx;
		/**
		 * @hop_counts: Hop counts received from responders in a given block.
		 */
		uint8_t received[FIRA_RESPONDERS_MAX];
	} hop_count;
};

/**
 * struct fira_owr_dt_anchor_round: Group of persistent variables
 * used to define a Ranging Round.
 */
struct fira_owr_dt_anchor_round {
	/**
	 * @entry: Entry in the rounds list.
	 * @see struct fira_session_owr_dt_anchor.
	 */
	struct qlist_head entry;
	/**
	 * @index: Index of the Round.
	 */
	uint8_t index;
	/**
	 * @acting_role: Internal role played by a DT-Anchor
	 * during this ranging round (as indexed).
	 */
	enum fira_dt_anchor_acting_role acting_role;
	/**
	 * @ranging_method_ds_twr: True when ranging method is DS-TWR.
	 */
	bool ranging_method_ds_twr;
	union {
		/* FIRA_DT_ANCHOR_ACTING_INITIATOR */
		struct {
			/**
			 * @slot_index_present: True when every item of the list
			 * of Responders contains explicit slot indexes from
			 * UCI.
			 */
			bool slot_index_present;
			/**
			 * @n_responder: Number of elements in the responders
			 * list.
			 */
			uint8_t n_responder;
			/**
			 * @responders: The list of Responders for this ranging
			 * round sorted by slot index.
			 * @see struct fira_dt_anchor_responder.
			 */
			struct qlist_head responders;
		};
		/* FIRA_DT_ANCHOR_ACTING_RESPONDER */
		struct {
			/**
			 * @tof_rctu: Time Of Flight as a result of DS-TWR by
			 * Responder during previous round. Warning: the value
			 * could be negative.
			 */
			int16_t tof_rctu;
			/**
			 * @time_sync: Private workspace for the time synchronization submodule.
			 * @see struct fira_time_sync
			 */
			struct fira_time_sync *time_sync;
		};
	};
};

/**
 * struct fira_session_owr_dt_anchor: Group of persistent variables used when
 * the device is a DT-Anchor.
 */
struct fira_session_owr_dt_anchor {
	/**
	 * @rounds: List of round descriptors sorted by round index.
	 * @see struct fira_owr_dt_anchor_round.
	 */
	struct qlist_head rounds;
	/**
	 * @pending_rounds: List of round descriptors waiting for activation.
	 * @see struct fira_owr_dt_anchor_round.
	 */
	struct qlist_head pending_rounds;
};

/**
 * fira_owr_dt_anchor_session_get_runtime() - Retrieve session OWR DT anchor
 * runtime Context.
 * @session: Session context.
 *
 * Return: OWR DT anchor runtime context.
 */
static inline struct fira_owr_dt_anchor_runtime *
fira_owr_dt_anchor_session_get_runtime(const struct fira_session *session)
{
	struct fbs_session_runtime *runtime = session->base.runtime;
	return qparent_of(runtime, struct fira_owr_dt_anchor_runtime, runtime);
}

/**
 * fira_owr_dt_anchor_round_get_active_slot_count() - Compute the number of
 * slots which are active for the target Round. WARNING: slot indexes could be
 * non-sequentials thus can be different from round duration in number of slots.
 *
 * @round: Round context.
 *
 * Return: The number of known slots (responder shall receive POLL to know).
 */
int fira_owr_dt_anchor_round_get_active_slot_count(const struct fira_owr_dt_anchor_round *round);

/**
 * fira_owr_dt_anchor_session_get_round() - Retrieves the requested Round
 * descriptor.
 * @session: Fira session context.
 * @round_index: The index of the requested round.
 *
 * Return: The found context or NULL.
 */
struct fira_owr_dt_anchor_round *
fira_owr_dt_anchor_session_get_round(const struct fira_session *session, int round_index);

/**
 * fira_owr_dt_anchor_session_ranging_round_done() - The procedure executed
 * when the ranging round is finished.
 * @session: Session context.
 * @report_info: Report information to forward to fira_session_report.
 */
void fira_owr_dt_anchor_session_ranging_round_done(struct fira_session *session,
						   struct fira_report_info *report_info);
