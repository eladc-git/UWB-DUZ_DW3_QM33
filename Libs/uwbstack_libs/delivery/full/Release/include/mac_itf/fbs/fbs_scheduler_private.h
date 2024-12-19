/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

struct fbs_region;
struct fbs_session;
struct fbs_session_demand;
struct fbs_scheduler_session;

/**
 * struct fbs_scheduler_ops - Set of callbacks to be supported by schedulers.
 */
struct fbs_scheduler_ops {
	/**
	 * @move_session_to_active_list: Moves the session in the active list.
	 * @fbs_session: FBS Session context.
	 */
	void (*move_session_to_active_list)(struct fbs_session *fbs_session);
	/**
	 * @get_next_session: Gets next session pointer which will do an access.
	 * @local: FBS region context.
	 * @next_timestamp_dtu: Next access opportunity.
	 * @max_duration_dtu: Max duration of the next access opportunity.
	 *
	 * Return: Pointer to the next session which will have the access, or NULL for idle.
	 */
	struct fbs_session *(*get_next_session)(struct fbs_region *local,
						uint32_t next_timestamp_dtu, int max_duration_dtu);
	/**
	 * @schedule_done: Session scheduled has done its access.
	 * @local: FBS region context.
	 *
	 * An access done is a access started with success or error.
	 * An access canceled is ignored.
	 */
	void (*schedule_done)(struct fbs_region *local);
};

/**
 * struct fbs_scheduler - FBS session scheduler context.
 */
struct fbs_scheduler {
	/**
	 * @elected_session: NULL when no session has been planned. A valid pointer otherwise.
	 *
	 * The validity of the pointer provides the capability to do the session scheduler once by
	 * access when the region scheduler is "on_demand".
	 */
	struct fbs_session *elected_session;
	/** @ops: Pointer to the scheduler operations. */
	const struct fbs_scheduler_ops *ops;
};

/**
 * fbs_scheduler_get_scheduler_session_info() - Get the scheduler session info from the session.
 * @fbs_session: FBS session context.
 *
 * Return: The pointer on the scheduler session info.
 */
struct fbs_scheduler_session_info *
fbs_scheduler_get_scheduler_session_info(const struct fbs_session *fbs_session);

/**
 * fbs_scheduler_update_ops() - Update the scheduler operations to use.
 * @local: FBS region context.
 */
void fbs_scheduler_update_ops(struct fbs_region *local);

/**
 * fbs_scheduler_get_next_session_for_access() - Gets next session which will do an access.
 * @local: FBS region context.
 * @next_timestamp_dtu: Next access opportunity.
 * @max_duration_dtu: Max duration of the next access opportunity.
 */
struct fbs_session *fbs_scheduler_get_next_session_for_access(struct fbs_region *local,
							      uint32_t next_timestamp_dtu,
							      int max_duration_dtu);

/**
 * fbs_scheduler_get_session_demand() - Submit an opportunity to the FBS session.
 * @fbs_session: FBS session context.
 * @next_timestamp_dtu: Next access opportunity.
 * @max_duration_dtu: Max duration of the next access opportunity.
 * @fsd: Output to be updated by the fbs_session.
 *
 * Return: 1 when the FBS session accept the opportunity, 0 when it is declined.
 */
int fbs_scheduler_get_session_demand(const struct fbs_session *fbs_session,
				     uint32_t next_timestamp_dtu, int max_duration_dtu,
				     struct fbs_session_demand *fsd);

/**
 * fbs_scheduler_reset_consecutive_schedule() - Reset the consecutive scheduling.
 * @fbs_session: FBS session context.
 */
void fbs_scheduler_reset_consecutive_schedule(struct fbs_session *fbs_session);

/**
 * fbs_scheduler_manage_update_block_start_valid() - Manages automatic session startup limit.
 * @fbs_session: FBS session context.
 * @next_timestamp_dtu: Next access opportunity.
 */
void fbs_scheduler_manage_update_block_start_valid(struct fbs_session *fbs_session,
						   uint32_t next_timestamp_dtu);

/**
 * fbs_scheduler_done() - Session scheduled has done its access.
 * @local: FBS region context.
 */
void fbs_scheduler_done(struct fbs_region *local);
