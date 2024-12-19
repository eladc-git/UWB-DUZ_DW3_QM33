/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/ccc/ccc_workspace.h"
#include "fbs/fbs_scheduler_private.h"
#include "fbs/fira/fira_region.h"
#include "net/fbs_region_nl.h"
#include "net/fbs_region_params.h"
#include "net/mcps802154.h"
#include "net/mcps802154_schedule.h"
#include "qerr.h"
#include "qlist.h"
#include "qtils.h"
#include "tmp_fbs_region.h"

#include <stdbool.h>
#include <stdint.h>

struct fbs_session;

/**
 * union shared_workspaces - Unique workspace shared between all sessions.
 *
 * TODO: This union is a temporary solution to deliver in time the multi-session
 * of CCC + FiRa in the FBS. It will be reworked to remove the direct dependency
 * of the FBS region on typed session (the includes).
 *
 * We could have something like: the sizeof(workspace) in the
 * fbs_stype_ops.workspace_size;
 *
 * static const fbs_stype_ops = {
 *    .workspace_size = sizeof(workspace),
 * };
 *
 * And them, the FBS region allocates the max size of all registered session,
 * during the fbs_open.
 */
union shared_workspaces {
	/** @fira: FiRa workspace. */
	struct fira_workspace fira;
	/** @ccc: CCC workspace. */
	struct ccc_workspace ccc;
};

/**
 * struct fbs_region - Fira Based Session (FBS) region context.
 */
struct fbs_region {
	/** @tmp_local: Temporary common local between regions. */
	struct tmp_fbs_local tmp_local;
	/** @access: Access returned to MCPS. */
	struct mcps802154_access access;
	/** @frame: Access frame referenced from access.  */
	struct mcps802154_access_frame frame;
	/** @sts_param: STS parameters for access frame. */
	struct mcps802154_sts_params sts_param;
	/** @channel: Channel parameters for access. */
	struct mcps802154_channel channel;
	/** @inactive_sessions: List of inactive sessions. */
	struct qlist_head inactive_sessions;
	/** @active_sessions: List of active sessions. */
	struct qlist_head active_sessions;
	/** @current_session: Pointer to the current session. */
	struct fbs_session *current_session;
	/** @deferred_session: Pointer to the deferred session. */
	struct fbs_session *deferred_session;
	/** @scheduler: Session scheduler context. */
	struct fbs_scheduler scheduler;
	/** @shared_ws: Shared workspace used by the current session. */
	union shared_workspaces shared_ws;
	/** @block_duration_rx_margin_ppm: Hack to simplify unit test. */
	int block_duration_rx_margin_ppm;
	/** @stop_request: Current session have a pending STOP to do. */
	bool stop_request;
	/** @restricted_channel_updated: True when channel has been updated during the access. */
	bool restricted_channel_updated;
	/** @deinit_request: Current session have a pending DEINIT to do. */
	bool deinit_request;
	/** @next_session_handle: Next session handle value to deliver. */
	uint32_t next_session_handle;
};

static inline struct fbs_region *region_to_local(struct mcps802154_region *region)
{
	return qparent_of(region, struct fbs_region, tmp_local.region);
}

static inline struct fbs_region *access_to_region(const struct mcps802154_access *access)
{
	return qparent_of(access, struct fbs_region, access);
}

/**
 * fbs_region_register() - Registers FBS region in the MAC.
 *
 * Return: QERR_SUCCESS on success, an error otherwise.
 */
enum qerr fbs_region_register(void);

/**
 * fbs_region_unregister() - Unregisters the FiRa protocol.
 *
 * All instanciations of the region can continue to run. Only new
 * instantiations of this region are no longer possible.
 */
void fbs_region_unregister(void);

/**
 * fbs_region_search_session_by_id() - Searches the FBS session by its ID.
 * @local: FBS region context.
 * @session_id: Session identifier to search.
 *
 * Return: FBS session context that matches the ID or NULL if not found or duplicated.
 */
struct fbs_session *fbs_region_search_session_by_id(const struct fbs_region *local,
						    uint32_t session_id);

/**
 * fbs_region_search_session_by_handle() - Searches the FBS session by its handle.
 * @local: FBS region context.
 * @session_handle: Session handle to search.
 *
 * Return: FBS session context that matches the handle or NULL if not found.
 */
struct fbs_session *fbs_region_search_session_by_handle(const struct fbs_region *local,
							uint32_t session_handle);

/**
 * fbs_device_status_ntf() - Report device status.
 * @local: FiRa Base Session local context.
 * @session: FiRa Base Session context to report.
 * @state: State to report.
 */
void fbs_device_status_ntf(struct tmp_fbs_local *local, struct fbs_session *session,
			   enum fbs_device_state state);

/**
 * fbs_region_alloc_rsp_skb() - Allocates a reply message.
 * @local: FBS region context.
 * @call_id: Response message identifier.
 * @approx_len: Approximate size required.
 *
 * Return: Pointer to an allocated message, or NULL on error.
 */
struct sk_buff *fbs_region_alloc_rsp_skb(struct fbs_region *local, enum fbs_call call_id,
					 int approx_len);
