/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fira/fira_frame_defines.h"
#include "fira/fira_session.h"
#include "fira/fira_session_app.h"
#include "fira/twr/fira_twr_session.h"
#include "linux/slab.h"
#include "net/fira_region_params.h"
#include "net/mcps802154_frame.h"
#include "quwbs/fbs/defs.h"

#include <stdbool.h>

struct qlist_head;

extern const struct fira_session_app fira_session_app_twr_time_based;

#define FIRA_FRAME_WITH_DM_IE_LEN                                                                \
	(IEEE802154_FC_LEN + IEEE802154_SHORT_ADDR_LEN + IEEE802154_SCF_LEN +                    \
	 FIRA_FRAME_DM_HEADER_IE_LEN + IEEE802154_IE_HEADER_LEN + FIRA_FRAME_DM_PAYLOAD_IE_LEN + \
	 FIRA_MIC_LEN + IEEE802154_FCS_LEN)

/**
 * fira_twr_time_based_session_controlees_count() - Get the number of controlees
 * notified in the ranging control message.
 * @session: Session.
 *
 * Return: Number of controlees.
 */
int fira_twr_time_based_session_controlees_count(const struct fira_session *session);

/**
 * fira_twr_time_based_session_get_slot_count() - Compute the number of slots
 * for the session.
 * @session: FiRa session context.
 *
 * Return: Number of slots.
 */
int fira_twr_time_based_session_get_slot_count(const struct fira_session *session);

/**
 * fira_twr_time_based_session_new_controlee() - Add new controlee.
 * @session: Session.
 * @controlee: Controlee to add.
 *
 * If succeed, function as taken ownership of the structure and removed it
 * from the original list.
 *
 * Return: see &enum fira_multicast_update_status.
 */
enum fira_multicast_update_status
fira_twr_time_based_session_new_controlee(struct fira_session *session,
					  struct fira_controlee *controlee);

/**
 * fira_twr_time_based_session_restart_controlees() - Restart all controlees and
 * erase pending delete requested.
 * @session: FiRa session context.
 *
 * Return: Number of controlee removed.
 */
void fira_twr_time_based_session_restart_controlees(struct fira_session *session);

/**
 * fira_twr_time_based_session_del_controlee() - Delete one controlee.
 * @session: Session.
 * @controlee: Controlee to delete.
 *
 * If succeed, function has removed given controlee from its current list and
 * freed it.
 *
 * Return: see &enum fira_multicast_update_status.
 */
enum fira_multicast_update_status
fira_twr_time_based_session_del_controlee(struct fira_session *session,
					  struct fira_controlee *controlee);

/**
 * fira_twr_time_based_session_update_controlees() - Update controlee's states.
 * @session: Session to test.
 */
void fira_twr_time_based_session_update_controlees(struct fira_session *session);

/**
 * fira_twr_time_based_session_send_multicast_list_ntf() - Send session update
 * controller multicast list ntf (see &enum fira_multicast_update_status).
 * @session: Current session.
 * @controlees_list: List of controlees that will be part of the notification.
 */
void fira_twr_time_based_session_send_multicast_list_ntf(struct fira_session *session,
							 struct qlist_head *controlees_list);

/**
 * fira_twr_time_based_cleanup_controlees() - Clean-up controlees.
 * @session: Current session.
 * @deleted_controlees: List of controlees that will be removed.
 */
void fira_twr_time_based_cleanup_controlees(struct fira_session *session,
					    struct qlist_head *deleted_controlees);

/**
 * fira_twr_session_controlee_active() - Check if the controlee is currently
 * active.
 * @controlee: Controlee.
 *
 * Return: True if the controlee is currently active.
 */
static inline bool fira_twr_session_controlee_active(struct fira_controlee *controlee)
{
	switch (controlee->state) {
	case FIRA_CONTROLEE_STATE_RUNNING:
	case FIRA_CONTROLEE_STATE_PENDING_DEL:
		return true;
	default:
		return false;
	}
}

static inline bool fira_twr_session_use_ss_cfo(const struct fira_session_params *params)
{
	/*
	 * We want the clock offset in SS-TWR:
	 * - On the controlee side for deferred mode.
	 * - Or on the controller side for non-deferred mode.
	 */
	return (params->ranging_round_usage == FIRA_RANGING_ROUND_USAGE_SSTWR_DEFERRED &&
		params->device_type == QUWBS_FBS_DEVICE_TYPE_CONTROLEE) ||
	       (params->ranging_round_usage == FIRA_RANGING_ROUND_USAGE_SSTWR_NON_DEFERRED &&
		params->device_type == QUWBS_FBS_DEVICE_TYPE_CONTROLLER);
}

static inline void fira_twr_destroy_controlee(struct fira_controlee *controlee)
{
	kfree_sensitive(controlee->sub_session.key);
	kfree(controlee);
}

static inline bool
fira_twr_time_based_session_controlee_in_error_state(const enum fira_controlee_state state)
{
	return state == FIRA_CONTROLEE_STATE_KEY_FETCH_FAILED ||
	       state == FIRA_CONTROLEE_STATE_SUB_SESSION_KEY_NOT_FOUND ||
	       state == FIRA_CONTROLEE_STATE_SUB_SESSION_KEY_NOT_APPLICABLE ||
	       state == FIRA_CONTROLEE_STATE_SESSION_KEY_NOT_FOUND ||
	       state == FIRA_CONTROLEE_STATE_SUB_SESSION_ID_NOT_FOUND;
}
