/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/fbs_region_nl.h"
#include "net/mcps802154_schedule.h"
#include "qerr.h"
#include "qlist.h"
#include "quwbs/fbs/defs.h"

#include <stdbool.h>
#include <stdint.h>

struct mcps802154_llhw;
struct nlattr;
struct genl_info;
struct sk_buff;
struct fbs_session;
struct fbs_session_demand;
struct fbs_se_key_request;

/**
 * struct fbs_stype_ops - FBS session type operations.
 */
struct fbs_stype_ops {
	/** @shortname: Session type short name for usages in traces. */
	const char *shortname;
	/**
	 * @consecutive_schedule_min: Minimum number of consecutive schedule to do.
	 *
	 * - When the value is zero (default) or one, there is no consecutive schedule to do.
	 *   The session is scheduled once, then another one can be scheduled (if there is).
	 * - When the value is two, two consecutives schedule are requested. So the sessions will
	 *   have the access two times consecutively. Then another session can be scheduled (if
	 *   there is).
	 * - And so on.
	 */
	uint8_t consecutive_schedule_min;
	/** @range_stype_begin: Begin of FBS session type. */
	enum quwbs_fbs_session_type range_stype_begin;
	/** @range_stype_end: End of FBS session type. */
	enum quwbs_fbs_session_type range_stype_end;
	/** @get_caps_info: Stype capabilties callback to fill the reply msg. */
	enum qerr (*get_caps_info)(struct mcps802154_llhw *llhw, struct sk_buff *rsp);
	/** @is_init_state: Returns true when one mandatory parameter is undefined. */
	bool (*is_init_state)(const struct fbs_session *fbs_session);
	/** @new_session: New session initialization callback. */
	struct fbs_session *(*new_session)(struct mcps802154_llhw *llhw,
					   enum quwbs_fbs_session_type stype, uint32_t session_id);
	/** @delete_session: Delete session callback (DEINIT). */
	void (*delete_session)(struct fbs_session *fbs_session);
	/** @deferred: Deferred session callback. */
	enum qerr (*deferred)(struct fbs_session *fbs_session);
	/** @notify_stop: Immediate stop required . */
	void (*notify_stop)(struct fbs_session *fbs_session, enum quwbs_fbs_reason_code rc);
	/** @receive_dynamic_key: Receive dynamic key callback. */
	void (*receive_dynamic_key)(struct fbs_session *fbs_session,
				    struct fbs_se_key_request *key_req, enum qerr status);
	/** @call: Generic command call callback. */
	int (*call)(struct fbs_session *fbs_session, enum fbs_call call_id,
		    struct nlattr **call_attrs, const struct genl_info *info);
	/** @stop_request: STOP command call callback. */
	void (*stop_request)(struct fbs_session *fbs_session, bool delayed);
	/** @deinit_request: Issue a deinit request, QERR_SUCCESS if deinit is done immediately. */
	enum qerr (*deinit_request)(struct fbs_session *fbs_session);
	/** @get_demand: Request the fsd in the opportunity windows. */
	int (*get_demand)(const struct fbs_session *fbs_session, uint32_t next_timestamp_dtu,
			  int max_duration_dtu, struct fbs_session_demand *fsd);
	/** @get_access: Get access callback. */
	struct mcps802154_access *(*get_access)(struct fbs_session *fbs_session,
						const struct fbs_session_demand *fsd);
	/** @access_done: Access done callback. */
	void (*access_done)(struct fbs_session *fbs_session,
			    enum mcps802154_access_done_reason reason, bool report_allowed);
	/** @check_missed_ranging: Checks if an access have been missed. */
	uint32_t (*check_missed_ranging)(struct fbs_session *fbs_session, uint32_t timestamp_dtu);
	/** @restricted_channel_updated: Checks the channel number. */
	enum qerr (*restricted_channel_updated)(struct fbs_session *fbs_session);
};

/**
 * struct fbs_stype_ops_item - FBS session type item for registering.
 */
struct fbs_stype_ops_item {
	/** @entry: Registration node for FBS session type list. */
	struct qlist_head entry;
	/** @ops: Operations of the session type. */
	const struct fbs_stype_ops *ops;
};

/**
 * fbs_stype_register() - Register a session type item.
 * @stype_item: "FBS session type item" to be added to the global list.
 *
 * Return: QERR_SUCCESS on success, or an error otherwise.
 */
enum qerr fbs_stype_register(struct fbs_stype_ops_item *stype_item);

/**
 * fbs_stype_unregister() - Unregister a session type item.
 * @stype_item: "FBS session type item" to be deleted from the global list.
 */
void fbs_stype_unregister(struct fbs_stype_ops_item *stype_item);

/**
 * fbs_stype_search() - Searches the FBS operations by its session type value.
 * @stype: Session type value, which matches FiRa standard expectations.
 *
 * Return: FBS operations that matches the session type or NULL if not found.
 */
const struct fbs_stype_ops *fbs_stype_search(enum quwbs_fbs_session_type stype);

/**
 * fbs_stype_is_init_state() - Asks the session if it has a mandatory parameter that is undefined.
 * @fbs_session: FBS session context.
 *
 * Return: True when a mandatory parameter is undefined, false otherwise.
 */
enum qerr fbs_stype_is_init_state(const struct fbs_session *fbs_session);

/**
 * fbs_stype_get_caps_info() - Get capabilities to each session type registered
 * @llhw: Low layer hardware context.
 * @rsp: Response message where to put every capabilities.
 *
 * Return: QERR_SUCCESS on success, or an error otherwise.
 */
enum qerr fbs_stype_get_caps_info(struct mcps802154_llhw *llhw, struct sk_buff *rsp);

/**
 * fbs_stype_delete_session() - Handles the DEINIT request (delete).
 * @fbs_session: FBS session context.
 *
 * Note: DEINIT is brutal and can happen in any FiRA state.
 */
void fbs_stype_delete_session(struct fbs_session *fbs_session);

/**
 * fbs_stype_stop_session() - Stop the FBS session.
 * @fbs_session: FBS session context.
 * @delayed: True when the STOP command has occurred during access.
 */
void fbs_stype_stop_session(struct fbs_session *fbs_session, bool delayed);

/**
 * fbs_stype_restricted_channel_updated() - Handle the restricted channel updated.
 * @fbs_session: FBS session context.
 */
void fbs_stype_restricted_channel_updated(struct fbs_session *fbs_session);
