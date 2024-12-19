/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/fbs_region_nl.h"
#include "net/mcps802154_schedule.h"
#include "qerr.h"

#include <stdbool.h>
#include <stdint.h>

/* Forward declaration. */
struct fira_session;
struct fira_report_info;
struct fbs_session_demand;
struct genl_info;
struct mcps802154_llhw;
struct nlattr;
struct sk_buff;

/**
 * struct fira_session_app - Specific FiRa session behavior.
 */
struct fira_session_app {
	/**
	 * @init: Initializes the ranging-specific context.
	 * &session: FiRa session.
	 *
	 * Return: QERR_SUCCESS or error.
	 */
	enum qerr (*init)(struct fira_session *session);
	/**
	 * @uninit: Uninit handler.
	 * &see: fira_session_app_uninit
	 */
	void (*uninit)(struct fira_session *session);
	/**
	 * @call: Call request handler.
	 * &see: fira_session_app_call
	 */
	int (*call)(struct fira_session *session, enum fbs_call call_id, struct nlattr **attrs,
		    const struct genl_info *info);
	/**
	 * @is_ready: is_ready handler.
	 * &see: fira_session_app_is_ready
	 */
	enum quwbs_fbs_reason_code (*is_ready)(const struct fira_session *session);
	/**
	 * @start: Start handler.
	 * &see: fira_session_app_start
	 */
	enum quwbs_fbs_reason_code (*start)(struct fira_session *session);
	/**
	 * @stop: Stop handler.
	 * &see: fira_session_app_stop
	 */
	void (*stop)(struct fira_session *session, bool delayed);
	/**
	 * @active_enter: Active enter handler.
	 * &see: fira_session_app_active_enter
	 */
	void (*active_enter)(struct fira_session *session);
	/**
	 * @active_leave: Active enter handler.
	 * &see: fira_session_app_active_leave
	 */
	void (*active_leave)(struct fira_session *session);
	/**
	 * @get_demand: Get session demand handler.
	 * &see: fira_session_app_get_demand
	 */
	int (*get_demand)(const struct fira_session *session, uint32_t next_timestamp_dtu,
			  int max_duration_dtu, struct fbs_session_demand *fsd);
	/**
	 * @get_access: Get access handler.
	 * &see: fira_session_app_get_access
	 */
	enum qerr (*get_access)(struct fira_session *session, const struct fbs_session_demand *fsd);
	/**
	 * @access_done: access done handler.
	 * &see: fira_session_app_access_done
	 */
	void (*access_done)(struct fira_session *session, enum mcps802154_access_done_reason reason,
			    bool report_allowed);
	/**
	 * @check_missed_ranging: check missed ranging handler.
	 * &see: fira_session_app_check_missed_ranging
	 */
	uint32_t (*check_missed_ranging)(struct fira_session *session, uint32_t timestamp_dtu);
	/**
	 * @is_report_permitted: is report permitted handler.
	 * &see: fira_session_app_is_report_permitted
	 */
	bool (*is_report_permitted)(const struct mcps802154_llhw *llhw,
				    struct fira_session *session,
				    const struct fira_report_info *report_info,
				    uint16_t *notification_bitfield);
	/**
	 * @report_ranging_data: report ranging data handler.
	 * &see: fira_session_app_report_ranging_data
	 */
	enum qerr (*report_ranging_data)(const struct mcps802154_llhw *llhw,
					 const struct fira_session *session,
					 const struct fira_report_info *report_info,
					 uint16_t notification_bitfield, struct sk_buff *msg);
	/**
	 * @set_data_size_in_ranging_round: Compute payload data size.
	 * &see: fira_session_app_set_data_size_in_ranging_round
	 */
	void (*set_data_size_in_ranging_round)(struct fira_session *session);
	/**
	 * @validate_config: Validate the session configuration.
	 * &see: fira_session_app_validate_config
	 */
	void (*validate_config)(struct fira_session *session);
};

/**
 * fira_session_app_init() - Initialize all specific session types.
 * @session: Session context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_session_app_init(struct fira_session *session);

/**
 * fira_session_app_uninit() - Uninitialize all specific session types.
 * @session: Session context.
 */
void fira_session_app_uninit(struct fira_session *session);

/**
 * fira_session_app_select() - Select session type from params.
 * @session: Session context.
 */
void fira_session_app_select(struct fira_session *session);

/**
 * fira_session_app_call() - Manage specific call id of FiRa session.
 * @session: Session context.
 * @call_id: Identifier of the FiRa procedure.
 * @attrs: Nested attribute containing procedure parameters.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
int fira_session_app_call(struct fira_session *session, enum fbs_call call_id,
			  struct nlattr **attrs, const struct genl_info *info);

/**
 * fira_session_app_is_ready() - Returns if session is ready to switch from IDLE
 * to ACTIVE.
 * @session: FiRa session context.
 *
 * Return: The reason code.
 */
enum quwbs_fbs_reason_code fira_session_app_is_ready(const struct fira_session *session);

/**
 * fira_session_app_start() - Start request for specific type of session.
 * @session: FiRa session context.
 *
 * Return: The reason code.
 */
enum quwbs_fbs_reason_code fira_session_app_start(struct fira_session *session);

/**
 * fira_session_app_stop() - Stop request for specific type of session.
 * @session: FiRa session context.
 * @delayed: True when the STOP command has occurred during access.
 */
void fira_session_app_stop(struct fira_session *session, bool delayed);

/**
 * fira_session_app_active_enter() - Session enter in active state.
 * @session: FiRa session context.
 */
void fira_session_app_active_enter(struct fira_session *session);

/**
 * fira_session_app_active_leave() - Session leave in active state.
 * @session: FiRa session context.
 */
void fira_session_app_active_leave(struct fira_session *session);

/**
 * fira_session_app_get_demand() - Get FiRa session demand information for
 * the access opportunity.
 * @session: FiRa session context.
 * @next_timestamp_dtu: Timestamp of the next available access.
 * @max_duration_dtu: Max duration provided to the session.
 * @fsd: FiRa session demand to fill.
 *
 * Return: 1 for a session demand otherwise 0 for no demand.
 */
int fira_session_app_get_demand(const struct fira_session *session, uint32_t next_timestamp_dtu,
				int max_duration_dtu, struct fbs_session_demand *fsd);

/**
 * fira_session_app_get_access() - Get the access of specific type of session.
 * @session: FiRa session context.
 * @fsd: FiRa session demanded.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_session_app_get_access(struct fira_session *session,
				      const struct fbs_session_demand *fsd);

/**
 * fira_session_app_access_done() - Access end of specific type of session.
 * @session: FiRa session context.
 * @reason: Reason/type of access end.
 * @report_allowed: True when report are allowed to be send.
 */
void fira_session_app_access_done(struct fira_session *session,
				  enum mcps802154_access_done_reason reason, bool report_allowed);

/**
 * fira_session_app_check_missed_ranging() - Check missed ranging for specific
 * type of session.
 * @session: FiRa session context.
 * @timestamp_dtu: Current timestamps dtu.
 *
 * Return: the number of missed rangings.
 */
uint32_t fira_session_app_check_missed_ranging(struct fira_session *session,
					       uint32_t timestamp_dtu);

/**
 * fira_session_app_is_report_permitted() - Check missed ranging for specific
 * type of session.
 * @llhw: Low-level device pointer.
 * @session: FiRa session context.
 * @report_info: report information to exploit for the reporting.
 * @notification_bitfield: Field of flags to fill which conditioning
 * notification.
 *
 * Return: true if there is actually data to report, else false.
 */
bool fira_session_app_is_report_permitted(struct mcps802154_llhw *llhw,
					  struct fira_session *session,
					  const struct fira_report_info *report_info,
					  uint16_t *notification_bitfield);

/**
 * fira_session_app_report_ranging_data() - Report ranging-specific data into
 * message.
 * @llhw: Low-level device pointer.
 * @session: FiRa session context.
 * @report_info: Report info to be analyzed.
 * @notification_bitfield: Field of flags conditioning notifs.
 * @msg: Pointer to the report message buffer.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_session_app_report_ranging_data(const struct mcps802154_llhw *llhw,
					       const struct fira_session *session,
					       const struct fira_report_info *report_info,
					       uint16_t notification_bitfield, struct sk_buff *msg);

/**
 * fira_session_app_set_data_size_in_ranging_round() - Compute the maximum
 *   MDSDU size that might be sent on a single round.
 * @session: FiRa session context.
 */
void fira_session_app_set_data_size_in_ranging_round(struct fira_session *session);

/**
 * fira_session_app_validate_config() - Finalize the configuration application.
 * @session: FiRa session context.
 */
void fira_session_app_validate_config(struct fira_session *session);
