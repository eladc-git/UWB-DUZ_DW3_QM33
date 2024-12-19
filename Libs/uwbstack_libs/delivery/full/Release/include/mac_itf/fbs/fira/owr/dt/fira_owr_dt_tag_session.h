/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_session.h"
#include "fbs/fbs_session_runtime.h"
#include "fira/fira_frame_defines.h"
#include "fira/fira_region.h"
#include "fira/fira_session.h"
#include "fira/fira_session_app.h"
#include "net/fira_region_params.h"
#include "net/mcps802154_schedule.h"
#include "qlist.h"
#include "qtils.h"

#include <stdbool.h>
#include <stdint.h>

extern const struct fira_session_app fira_owr_dt_tag_session_app;

struct fira_owr_dt_tag_runtime {
	/**
	 * @runtime: inherit from base runtime struct.
	 */
	struct fbs_session_runtime runtime;
};

/*
 * This offset exists between values of OWR Message Type field in OWR Message
 * (see Table 36 in FiRa MAC Technical Requirements v2.0.0_0.9r0) and Message
 * Type field in DL-TDoA Ranging Measurement Result (see Table 35, chapter
 * 7.5.2, "Ranging Start Command", in FiRa UCI Generic Technical Specification
 * v2.0.0_0.9r0).
 */
#define FIRA_DT_MEASUREMENT_CONVERT_MESSAGE_TYPE(x) ((x)-2)

/**
 * struct fira_owr_dt_tag_round: Group of persistent variables
 * used to define a Ranging Round.
 */
struct fira_owr_dt_tag_round {
	/**
	 * @entry: Entry in the rounds list.
	 * @see struct fira_session_owr_dt_anchor.
	 */
	struct qlist_head entry;
	/**
	 * @index: Index of the Round.
	 */
	uint8_t index;
};

/**
 * struct fira_session_owr_dt_tag: Group of persistent variables used when
 * the device is a DT-Tag.
 */
struct fira_session_owr_dt_tag {
	/**
	 * @active_rounds: List of round descriptors sorted by round index.
	 */
	struct qlist_head active_rounds;
	/**
	 * @pending_rounds: List of round descriptors to be added at the
	 * end of the block.
	 */
	struct qlist_head pending_rounds;
	/**
	 * @round: Points to the current active round for this DT-Tag session.
	 */
	struct fira_owr_dt_tag_round *round;
	/**
	 * @current_ranging_method: Ranging method to be used by a DT-Tag
	 * in the current ranging round.
	 */
	enum fira_dl_tdoa_ranging_method current_ranging_method;
};

/**
 * struct fira_ranging_info_owr_dt - Ranging information structure which is used
 * to fill DL-TDoA Measurement Result (applicale for DT-Tags).
 */
struct fira_ranging_info_owr_dt {
	/**
	 * @common: The subset common for all the session modes. Shall be the first field
	 * of this struct.
	 */
	struct fira_ranging_info common;
	/**
	 * @message_type: Type of the message received.
	 */
	enum fira_owr_message_type message_type;
	/**
	 * @tx_timestamp_type: Type of the TX timestamp (local time base vs
	 * common time base) included in the received message.
	 */
	enum fira_owr_dtm_timestamp_type tx_timestamp_type;
	/**
	 * @tx_timestamp_len: Length of the TX timestamp (40-bit vs 64-bit)
	 * included in the received message.
	 */
	enum fira_owr_dtm_timestamp_len tx_timestamp_len;
	/**
	 * @rx_timestamp_len: Length of the RX timestamp (40-bit vs 64-bit)
	 * calculated during the reception of the received message.
	 */
	enum fira_owr_dtm_timestamp_len rx_timestamp_len;
	/**
	 * @anchor_location_type: Type of the coordinate system of DT-Anchor
	 * location (0: WGS84, 1: relative) (if included).
	 */
	enum fira_dt_location_coord_system_type anchor_location_type;
	/**
	 * @remote_cfo_present: True if remote_cfo value is present, false otherwise.
	 */
	bool remote_cfo_present;
	/**
	 * @local_cfo_present: True if local_cfo value is present, false otherwise.
	 */
	bool local_cfo_present;
	/**
	 * @rssi_present: True if rssi value is present, false otherwise.
	 */
	bool rssi_present;
	/**
	 * @anchor_location_present: True if anchor_location value is present, false otherwise.
	 */
	bool anchor_location_present;
	/**
	 * @active_ranging_round_indexes_len: Number of active ranging round
	 * indexes included in the measurement.
	 */
	uint8_t active_ranging_round_indexes_len;
	/**
	 * @rssi: RSSI measured by DT-Tag during the reception (encoded as Q7.1).
	 */
	uint8_t rssi;
	/**
	 * @local_cfo: Clock Frequency Offset measured locally with respect to
	 * the DT-Anchor that sent the message received (encoded as Q6.10).
	 */
	uint16_t local_cfo;
	/**
	 * @remote_cfo: Clock Frequency Offset of a Responder DT-Anchor with
	 * respect to the Initiator DT-Anchor of the ranging round as included
	 * in the received message (encoded as Q6.10).
	 */
	uint16_t remote_cfo;
	/**
	 * @tx_timestamp_rctu: TX timestamp included in the received message (in RCTU).
	 */
	uint64_t tx_timestamp_rctu;
	/**
	 * @rx_timestamp_rctu: RX timestamp calculated during the reception of the message (in
	 * RCTU).
	 */
	uint64_t rx_timestamp_rctu;
	/**
	 * @initiator_reply_time_rctu: Reply time of the Initiator DT-Anchor
	 * measured between the reception of Response DTM and the transmission
	 * of Final DTM (used only in DS-TWR, unit: RCTU).
	 */
	uint32_t initiator_reply_time_rctu;
	/**
	 * @responder_reply_time_rctu: Reply time of the Responder DT-Anchor
	 * measured between the reception of Poll DTM and the transmission of
	 * Response DTM (unit: RCTU).
	 */
	uint32_t responder_reply_time_rctu;
	/**
	 * @anchor_location: Location coordinates of DT-Anchor that sent the
	 * message received.
	 */
	uint8_t anchor_location[FIRA_OWR_DTM_LOCATION_SIZE_MAX];
	/**
	 * @initiator_responder_tof_rctu: Time of Flight measured between the
	 * Initiator DT-Anchor and the Responder DT-Anchor (for SS-TWR it's
	 * calculated by Initiator DT-Anchor and included in Poll DTM and for
	 * DS-TWR it's calculated by Responder DT-Anchor and included in
	 * Response DTM, unit: RCTU)
	 */
	uint16_t initiator_responder_tof_rctu;
	/**
	 * @active_ranging_round_indexes: List of active ranging round indexes
	 * in which the DT-Anchor that sent the message received participates.
	 */
	uint8_t *active_ranging_round_indexes;
};

/**
 * fira_owr_dt_tag_session_get_runtime() - Retrieve session OWR DT tag runtime
 * Context.
 * @session: Session context.
 *
 * Return: OWR DT tag runtime context.
 */
static inline struct fira_owr_dt_tag_runtime *
fira_owr_dt_tag_session_get_runtime(const struct fira_session *session)
{
	struct fbs_session_runtime *runtime = session->base.runtime;
	return qparent_of(runtime, struct fira_owr_dt_tag_runtime, runtime);
}

static inline struct fira_ranging_info_owr_dt *
fira_owr_dt_tag_session_get_ranging_info_owr_dt(const struct fira_ranging_info *ranging_info)
{
	return qparent_of(ranging_info, struct fira_ranging_info_owr_dt, common);
}

/**
 * fira_owr_dt_tag_round_list_get - Search and return an item
 * matching the given round_index in a given round list.
 * @rounds: round list where to search the round.
 * @round_index: round index to find.
 *
 * Return: pointer to the matching round item, or NULL.
 */
struct fira_owr_dt_tag_round *fira_owr_dt_tag_round_list_get(const struct qlist_head *rounds,
							     int round_index);

void fira_owr_dt_tag_session_access_done(struct fira_session *session,
					 enum mcps802154_access_done_reason reason);
