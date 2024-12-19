/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_region.h"
#include "mcps802154_se.h"
#include "net/fbs_region_nl.h"
#include "qerr.h"
#include "qlist.h"
#include "qtils.h"
#include "quwbs/fbs/defs.h"

#include <stdbool.h>
#include <stdint.h>

struct fbs_session_demand;
struct mcps802154_access;
struct sk_buff;
struct fbs_session_runtime;

/**
 * struct fbs_session - FiRa Based Session (FBS) context.
 *
 * A FBS session is the instantiation produced by the new_session handler
 * which is in the fbs_stype_ops definition.
 */
struct fbs_session {
	/** @entry: Registration node for active/inactive list. */
	struct qlist_head entry;
	/** @ops: Operations of the FBS typed session. */
	const struct fbs_stype_ops *ops;
	/** @local: FBS region context. */
	struct fbs_region *local;
	/** @runtime: FBS session runtime storage context. */
	struct fbs_session_runtime *runtime;
	/** @stype: Session type provided on the session init command. */
	enum quwbs_fbs_session_type stype;
	/**
	 * @state: FBS state is cached to optimize CPU load.
	 *
	 * As long as value is INIT, FBS must check that it is still INIT
	 * Once the session is ACTIVE, it is also available in the active list.
	 *
	 * Notes:
	 * - The state is equal to ACTIVE when asynchronous start operations are done.
	 * - The session can be INIT or IDLE on its creation (new_session), because a session can
	 *   have some mandatory parameters or not.
	 *   As reminder, a mandatory parameter is UNDEFINED on the session creation.
	 */
	enum quwbs_fbs_session_state state;
	/**
	 * @ranging_count: The number of times ranging has been attempted
	 * during the session.
	 */
	uint32_t ranging_count;
	/** @id: Session identifier on the medium. */
	uint32_t id;
	/** @handle: Session handle for session management. */
	uint32_t handle;
	/** @event_portid: Port identifier to use for notifications. */
	uint32_t event_portid;
	/**
	 * @scheduler_entry: List entry for the FBS scheduler.
	 *
	 * TODO:
	 * It should be in "struct fbs_scheduler_session_info" because it is only valid when the
	 * session is active (= when "runtime" is valid).
	 * But it could take more refactoring work to align all the source code.
	 * Because the "struct fbs_session_runtime" could become more a "struct fbs_session_active"
	 * as it can become the context to provide to the session during access instead of "struct
	 * fbs_session,".
	 * "struct fbs_session" is valid in an idle or active state.
	 */
	struct qlist_head scheduler_entry;
};

/**
 * struct fbs_se_key_request - Overloading structure for dynamic key request.
 */
struct fbs_se_key_request {
	/** @req: Request for key request queue manager. */
	struct mcps802154_se_request req;
	/** @fbs_session: The FBS session requesting key. */
	struct fbs_session *fbs_session;
};

/**
 * fbs_session_set_runtime() - Temporary function.
 * @fbs_session: FBS session context.
 * @runtime: FBS runtime context to assign to the session.
 * @priority: Priority of the session provided by parameters.
 *
 * This have to be reworked when parameters organisation will not be a big
 * structure. This function is primarily intended to ensure that the priority is
 * copied.
 */
void fbs_session_set_runtime(struct fbs_session *fbs_session, struct fbs_session_runtime *runtime,
			     uint8_t priority);

static inline struct fbs_session *access_to_session(const struct mcps802154_access *access)
{
	return access_to_region(access)->current_session;
}

static inline struct fbs_se_key_request *
mcps802154_request_to_fbs_request(struct mcps802154_se_request *mcps)
{
	return qparent_of(mcps, struct fbs_se_key_request, req);
}

/**
 * fbs_session_active_enter() - Switches the session from inactive to active.
 * @fbs_session: FBS session context.
 *
 * Note: The switch is done after Secure Element (SE) operations (if there are
 * any).
 */
void fbs_session_active_enter(struct fbs_session *fbs_session);

/**
 * fbs_session_active_leave() - Switches the session from active to inactive.
 * @fbs_session: FBS session context.
 * @rc: Reason code to put in the notification message.
 */
void fbs_session_active_leave(struct fbs_session *fbs_session, enum quwbs_fbs_reason_code rc);

/**
 * fbs_session_deferred() - Saves the request to defer a session job.
 * @fbs_session: FBS session context which wants to defer a job.
 *
 * Notes:
 * - Only one deferred region/session at a time is possible.
 * - The classic usage is to be called by the current_session.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr fbs_session_deferred(struct fbs_session *fbs_session);

/**
 * fbs_session_get_state() - Computes the FiRa state.
 * @fbs_session: FBS session context.
 *
 * Return: The session state, which matches FiRA standard expectations.
 */
enum quwbs_fbs_session_state fbs_session_get_state(const struct fbs_session *fbs_session);

static inline bool fbs_session_is_active(const struct fbs_session *fbs_session)
{
	return fbs_session->state == QUWBS_FBS_SESSION_STATE_ACTIVE;
}

/**
 * fbs_session_set_unsync_demand() - Fill fsd with a scanning medium mindset.
 * @fbs_session: FBS session context.
 * @next_timestamp_dtu: Timestamp of the next available access.
 * @max_duration_dtu: Max duration opportunity from next_timestamp_dtu.
 * @fsd: FBS session demand to update totally.
 *
 * Return: 1 margin applied to fsd otherwise 0 on failure.
 */
int fbs_session_set_unsync_demand(const struct fbs_session *fbs_session,
				  uint32_t next_timestamp_dtu, int max_duration_dtu,
				  struct fbs_session_demand *fsd);

/**
 * fbs_session_find_block_start() - Update fsd about block start.
 * @fbs_session: FBS session context.
 * @next_timestamp_dtu: Timestamp of the next available access.
 * @fsd: FBS session demand to update on block start and add_block fields.
 */
void fbs_session_find_block_start(const struct fbs_session *fbs_session,
				  uint32_t next_timestamp_dtu, struct fbs_session_demand *fsd);

/**
 * fbs_session_negotiate_duration() - Update the fsd duration dtu from max
 * duration dtu.
 * @fbs_session: FBS session context.
 * @next_timestamp_dtu: Timestamp of the next available access.
 * @max_duration_dtu: Max duration opportunity from next_timestamp_dtu.
 * @fsd: FBS session demand to update.
 *
 * Return: 1 margin applied to fsd otherwise 0 on failure.
 */
int fbs_session_negotiate_duration(const struct fbs_session *fbs_session,
				   uint32_t next_timestamp_dtu, int max_duration_dtu,
				   struct fbs_session_demand *fsd);

/**
 * fbs_session_apply_rx_margin() - Apply the rx margin to fsd.
 * @fbs_session: FBS session context.
 * @next_timestamp_dtu: Timestamp of the next available access.
 * @fsd: FBS session demand to fill.
 */
void fbs_session_apply_rx_margin(const struct fbs_session *fbs_session, uint32_t next_timestamp_dtu,
				 struct fbs_session_demand *fsd);

/**
 * fbs_session_alloc_ntf_skb() - Allocates a notification message to send.
 * @fbs_session: FBS session context.
 * @call_id: Notification message identifier.
 * @approx_len: Approximate size required.
 *
 * Return: Pointer to an allocated message, or NULL on error.
 */
struct sk_buff *fbs_session_alloc_ntf_skb(const struct fbs_session *fbs_session,
					  enum fbs_call call_id, int approx_len);

/**
 * fbs_session_free_ntf_skb() - Free a notification message.
 * @msg: Notification message to free.
 * @call_id: Notification message identifier.
 */
void fbs_session_free_ntf_skb(struct sk_buff *msg, enum fbs_call call_id);

/**
 * fbs_session_new_se_key_req() - Allocate and init a new FBS SE key request.
 * @fbs_session: FBS session context.
 * @id: Identifier of the requested key.
 * @size: Size in byte of the key.
 *
 * Return: A valid pointer or NULL on error.
 */
struct fbs_se_key_request *fbs_session_new_se_key_req(struct fbs_session *fbs_session, uint32_t id,
						      uint8_t size);

/**
 * fbs_session_delete_se_key_req() - Delete the FBS SE key request.
 * @fbs_se_key_req: Pointer on the object to free (can be NULL)
 */
void fbs_session_delete_se_key_req(struct fbs_se_key_request *fbs_se_key_req);

/**
 * fbs_session_submit_se_key_req() - Submit the FBS SE key request.
 * @fbs_se_key_req: Pointer on the object to submit.
 */
void fbs_session_submit_se_key_req(struct fbs_se_key_request *fbs_se_key_req);

/**
 * fbs_session_cancel_se_key_req() - Cancel the FBS SE key request and delete
 * it if still pending.
 * @fbs_se_key_req: Pointer on the object to cancel.
 */
void fbs_session_cancel_se_key_req(struct fbs_se_key_request *fbs_se_key_req);

/**
 * fbs_convert_qerr_into_reason_code() - Convert, if needed,
 * returned error status into reason code.
 * @err: Returned error.
 *
 * Return: The reason code.
 */
enum quwbs_fbs_reason_code fbs_convert_qerr_into_reason_code(enum qerr err);

/**
 * fbs_session_status_ntf() - Report session status.
 * @fbs_session: FBS context to report.
 * @reason_code: Reason code to report.
 */
void fbs_session_status_ntf(struct fbs_session *fbs_session,
			    enum quwbs_fbs_reason_code reason_code);
