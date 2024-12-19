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
#include "linux/slab.h"
#include "net/fira_region_params.h"
#include "qerr.h"
#include "qlist.h"
#include "qtils.h"
#include "quwbs/fbs/defs.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* IWYU pragma: no_include <asm/int-ll64.h> */

struct fbs_session_demand;
struct mcps802154_llhw;
struct sk_buff;

/**
 * enum fira_controlee_state - State of controlee.
 * @FIRA_CONTROLEE_STATE_PENDING_RUN: The controlee is waiting for its
 *	initialization either at start or, if session is already active, at the end
 *	of the round.
 * @FIRA_CONTROLEE_STATE_KEY_FETCH_FAILED: The controlee is in an error state as
 *	its sub-session key was not able to be fetched from Key Manager.
 * @FIRA_CONTROLEE_STATE_SUB_SESSION_KEY_NOT_FOUND: The controlee is in an error
 *	state as its sub-session key was not found.
 * @FIRA_CONTROLEE_STATE_SUB_SESSION_KEY_NOT_APPLICABLE: The controlee is in an
 *	error state as sub-sessions are not applicable for the current sts_config.
 * @FIRA_CONTROLEE_STATE_SESSION_KEY_NOT_FOUND: The controlee is in an error
 *	state as session key was not found (but its sub-session key was).
 * @FIRA_CONTROLEE_STATE_SUB_SESSION_ID_NOT_FOUND: The controlee is in an error
 *	state as its sub-session ID was not found.
 * @FIRA_CONTROLEE_STATE_WAITING_KEY: The controlee wait for key-manager
 *	response to be set to RUNNING state at the end of round.
 * @FIRA_CONTROLEE_STATE_RUNNING: The controlee is running.
 * @FIRA_CONTROLEE_STATE_STOPPING: The controlee is stopping.
 * @FIRA_CONTROLEE_STATE_PENDING_DEL: The controlee will be set to DELETING
 *	state at the end of round.
 * @FIRA_CONTROLEE_STATE_DELETING: The controlee is being deleted.
 */
enum fira_controlee_state {
	FIRA_CONTROLEE_STATE_PENDING_RUN,
	FIRA_CONTROLEE_STATE_KEY_FETCH_FAILED,
	FIRA_CONTROLEE_STATE_SUB_SESSION_KEY_NOT_FOUND,
	FIRA_CONTROLEE_STATE_SUB_SESSION_KEY_NOT_APPLICABLE,
	FIRA_CONTROLEE_STATE_SESSION_KEY_NOT_FOUND,
	FIRA_CONTROLEE_STATE_SUB_SESSION_ID_NOT_FOUND,
	FIRA_CONTROLEE_STATE_WAITING_KEY,
	FIRA_CONTROLEE_STATE_RUNNING,
	FIRA_CONTROLEE_STATE_STOPPING,
	FIRA_CONTROLEE_STATE_PENDING_DEL,
	FIRA_CONTROLEE_STATE_DELETING,
};

struct fira_twr_runtime {
	/**
	 * @runtime: inherit from base runtime struct.
	 */
	struct fbs_session_runtime runtime;
	/**
	 * @stop_inband: Session has been requested to stop by controller. This
	 * field is only used for controlee sessions.
	 */
	bool stop_inband;
	/**
	 * @n_in_band_stop_left: N stop in band ranging left.
	 *
	 * Number of ranging which announce the stop in band of the controller
	 * on the medium.
	 */
	uint8_t n_in_band_stop_left;
};

/**
 * struct fira_controlee - Represent a controlee.
 */
struct fira_controlee {
	/**
	 * @short_addr: Short address of the controlee.
	 */
	uint16_t short_addr;
	/**
	 * @sub_session: Sub-Session information.
	 */
	struct {
		/**
		 * @crypto: Crypto related variables in the sub-session. Only
		 * valid if the current device is a controller/initiator and
		 * sts_config if either FBS_STS_MODE_DYNAMIC_INDIVIDUAL_KEY or
		 * FBS_STS_MODE_PROVISIONED_INDIVIDUAL_KEY.
		 */
		struct fira_crypto *crypto;
		/**
		 * @se_key_req: pending dynamic key request.
		 */
		struct fbs_se_key_request *se_key_req;
		/**
		 * @key: Sub-session key used by the controlee.
		 */
		uint8_t *key;
		/**
		 * @id: Sub-Session ID for the controlee device.
		 */
		uint32_t id;
		/**
		 * @key_len: Length of the sub-session key used by
		 * the controlee.
		 */
		uint8_t key_len;
		/**
		 * @valid: Is the controlee using a sub-session.
		 */
		bool valid;
	} sub_session;
	/**
	 * @state: Current state of the controlee.
	 */
	enum fira_controlee_state state;
	/**
	 * @session_info_ntf_status: Internal status used to determine if
	 * SESSION_INFO_NTF should be sent.
	 */
	enum fira_session_info_ntf_status session_info_ntf_status;
	/**
	 * @in_band_termination_attempt_counter: in band termination counter.
	 * in band termination signal is sent by controller to a controlee
	 * until it is decremented to zero.
	 */
	int8_t in_band_termination_attempt_counter;
	/**
	 * @entry: Entry in list of controlees.
	 */
	struct qlist_head entry;
};

/**
 * struct fira_twr_report_info - TWR specific report information.
 */
struct fira_twr_report_info {
	/**
	 * @stopped_controlees: NULL, or short address of all stopped
	 * controlees.
	 */
	const uint16_t *stopped_controlees;
	/**
	 * @n_stopped_controlees: Number of controlees stopped in array above.
	 */
	size_t n_stopped_controlees;
};

struct fira_twr_data_transfer {
	/**
	 * @is_data_in_current_rround: A flag saying if application data
	 * will be sent piggybacked during the ranging round.
	 */
	bool is_data_in_current_rround;
	/**
	 * @payload_offset: Offset used to send a payload segmented in
	 * multiple frames.
	 */
	uint16_t payload_offset;
	/**
	 * @seq_num_rcv: Sequence number of packet received.
	 */
	uint16_t seq_num_rcv;
};

static inline uint8_t *fira_twr_alloc_session_key(uint8_t *key, uint8_t prev_len, uint8_t new_len)
{
	if (key && new_len <= prev_len)
		return key;

	kfree_sensitive(key);
	return (uint8_t *)kmalloc(new_len, GFP_KERNEL);
}

/**
 * fira_twr_session_rr_is_dstwr() - Check whether ranging round is a double
 * two way ranging.
 * @p: Session parameters.
 *
 * Return: True or false.
 */
static inline bool fira_twr_session_rr_is_dstwr(const struct fira_session_params *p)
{
	return p->ranging_round_usage == FIRA_RANGING_ROUND_USAGE_DSTWR_DEFERRED ||
	       p->ranging_round_usage == FIRA_RANGING_ROUND_USAGE_DSTWR_NON_DEFERRED;
}

/**
 * fira_twr_session_rr_is_sstwr() - Check whether ranging round is a single
 * sided two way ranging.
 * @p: Session parameters.
 *
 * Return: True or false.
 */
static inline bool fira_twr_session_rr_is_sstwr(const struct fira_session_params *p)
{
	return p->ranging_round_usage == FIRA_RANGING_ROUND_USAGE_SSTWR_DEFERRED ||
	       p->ranging_round_usage == FIRA_RANGING_ROUND_USAGE_SSTWR_NON_DEFERRED ||
	       p->ranging_round_usage ==
		       FIRA_RANGING_ROUND_USAGE_ESS_TWR_NON_DEFERRED_CONTENTION_BASED;
}

/**
 * fira_twr_session_rr_is_deferred() - Check whether ranging round is with a
 * deferred mode the schedule, and configure its parameters.
 * @p: Session parameters.
 *
 * Return: True or false.
 */
static inline bool fira_twr_session_rr_is_deferred(const struct fira_session_params *p)
{
	return p->ranging_round_usage == FIRA_RANGING_ROUND_USAGE_SSTWR_DEFERRED ||
	       p->ranging_round_usage == FIRA_RANGING_ROUND_USAGE_DSTWR_DEFERRED;
}

/**
 * fira_twr_session_ranging_round_done() - Update controlees, notify upper
 * layers and rotate crypto keys if needed.
 * @session: Session context.
 * @report_info: Report information to forward fira_session_report.
 */
void fira_twr_session_ranging_round_done(struct fira_session *session,
					 struct fira_report_info *report_info);

enum qerr fira_twr_session_report_measurement(const struct mcps802154_llhw *llhw,
					      const struct fira_session *session,
					      struct sk_buff *msg,
					      const struct fira_ranging_info *ranging_data);

/**
 * fira_twr_session_is_ranging_result_report_message() - dsd.
 * @params: FiRa session context.
 *
 * Return: True when Ranging Control Phase(RCP) is excluded in ranging round.
 */
static inline bool
fira_twr_session_is_ranging_result_report_message(const struct fira_session_params *params)
{
	return params->ranging_round_control & FIRA_RANGING_ROUND_CONTROL_RCP_EXCLUDED;
}

/**
 * fira_twr_session_new_runtime() - Create TWR runtime context and attach it to session.
 * @session: FiRa session to attach TWR runtime context to.
 *
 * Return: new TWR runtime context or NULL when out of memory.
 */
struct fira_twr_runtime *fira_twr_session_new_runtime(struct fira_session *session);

/**
 * fira_twr_session_delete_runtime() - Detach and destroy TWR session runtime
 * context.
 * @session: FiRa session to attach TWR runtime context to.
 */
void fira_twr_session_delete_runtime(struct fira_session *session);

/**
 * fira_twr_session_get_runtime() - Retrieve session TWR runtime Context.
 * @session: Session context.
 *
 * Return: TWR runtime context.
 */
static inline struct fira_twr_runtime *
fira_twr_session_get_runtime(const struct fira_session *session)
{
	struct fbs_session_runtime *runtime = session->base.runtime;
	return qparent_of(runtime, struct fira_twr_runtime, runtime);
}

/**
 * fira_twr_session_is_ready() - Common code for TWR is_ready case.
 * @session: FiRa Session context.
 *
 * Return: The reason code.
 */
enum quwbs_fbs_reason_code fira_twr_session_is_ready(const struct fira_session *session);

/**
 * fira_twr_session_start() - Set&Check basic parameters on session start.
 * @session: FiRa Session context.
 *
 * Return: The reason code.
 */
enum quwbs_fbs_reason_code fira_twr_session_start(struct fira_session *session);

/**
 * fira_twr_session_init() - Common session init.
 * @session: FiRa Session context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_twr_session_init(struct fira_session *session);

/**
 * fira_twr_session_uninit() - Common session uninit.
 * @session: FiRa Session context.
 */
void fira_twr_session_uninit(struct fira_session *session);

/**
 * fira_twr_session_get_demand() - Compute demand for access.
 * @session: FiRa session context.
 * @next_timestamp_dtu: Timestamp of next possible access.
 * @max_duration_dtu: Maximum duration of next possible access.
 * @fsd: FiRa Session Demand.
 *
 * Return: 1 if session should get access, 0 otherwise.
 */
int fira_twr_session_get_demand(const struct fira_session *session, uint32_t next_timestamp_dtu,
				int max_duration_dtu, struct fbs_session_demand *fsd);

/**
 * fira_twr_session_check_missed_ranging() - Check for missed rangings.
 * @session: FiRa Session context.
 * @timestamp_dtu: Current timestamp.
 *
 * Return: the number of missed rangings.
 */
uint32_t fira_twr_session_check_missed_ranging(struct fira_session *session,
					       uint32_t timestamp_dtu);

/**
 * fira_twr_session_is_report_permitted() - Check if report is permitted.
 * @llhw: Low-level device pointer.
 * @session: FiRa Session context.
 * @report_info: The report.
 * @notification_bitfield: bitfield indicating which ranging to report.
 *
 * Return: true or false
 */
bool fira_twr_session_is_report_permitted(const struct mcps802154_llhw *llhw,
					  struct fira_session *session,
					  const struct fira_report_info *report_info,
					  uint16_t *notification_bitfield);

/**
 * fira_twr_session_report_ranging_data() - Send report
 * @llhw: Low-level device pointer.
 * @session: FiRa Session context.
 * @report_info: nformation on the ranging result.
 * @notification_bitfield: bitfield indicating which ranging to report.
 * @msg: Report buffer to be filled.
 *
 * Return: QERR_SUCCESS or error
 */
enum qerr fira_twr_session_report_ranging_data(const struct mcps802154_llhw *llhw,
					       const struct fira_session *session,
					       const struct fira_report_info *report_info,
					       uint16_t notification_bitfield, struct sk_buff *msg);

/**
 * fira_twr_session_validate_config() - Clean parameters before session start.
 * @session: FiRa Session context.
 */
void fira_twr_session_validate_config(struct fira_session *session);
