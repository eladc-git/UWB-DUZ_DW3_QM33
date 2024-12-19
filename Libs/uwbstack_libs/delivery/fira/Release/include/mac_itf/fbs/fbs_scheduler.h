/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "quwbs/fbs/defs.h"

#include <stdbool.h>
#include <stdint.h>

struct fbs_session;

#define FBS_SCHEDULER_PRIORITY_DEFAULT 50

/**
 * enum fbs_schedule_role - Main role the session.
 * @FBS_SCHEDULE_ROLE_UNDEFINED: The session doesn't know where he is living.
 * @FBS_SCHEDULE_ROLE_COORDINATOR: The session will pilot the protocol.
 * @FBS_SCHEDULE_ROLE_COORDINATED: The session is piloted by its coordinator.
 */
enum fbs_schedule_role {
	FBS_SCHEDULE_ROLE_UNDEFINED,
	FBS_SCHEDULE_ROLE_COORDINATOR,
	FBS_SCHEDULE_ROLE_COORDINATED,
};

/**
 * struct fbs_session_demand - Next access information for one FiRa session.
 */
struct fbs_session_demand {
	/**
	 * @block_start_dtu: Block start in dtu.
	 */
	uint32_t block_start_dtu;
	/**
	 * @timestamp_dtu: Access timestamp in dtu.
	 */
	uint32_t timestamp_dtu;
	/**
	 * @max_duration_dtu: Maximum duration for the access.
	 */
	int max_duration_dtu;
	/**
	 * @add_blocks: Number of blocks to add.
	 */
	int add_blocks;
	/**
	 * @round_index: Round index to apply for the access.
	 */
	int round_index;
	/**
	 * @rx_timeout_dtu: Timeout to apply to the frame configuration when
	 *   it is the first frame of the access only (in relation to
	 *   fbs_schedule_role).
	 */
	int rx_timeout_dtu;
};

/**
 * struct fbs_scheduler_session_info - Scheduler informations per session.
 */
struct fbs_scheduler_session_info {
	/**
	 * @fsd: Result updated by the stype->get_demand.
	 *
	 * - The allocation is done by the FBS region.
	 * - It is written by the FBS session through a pointer.
	 * - It is read by FBS region.
	 */
	struct fbs_session_demand fsd;
	/**
	 * @deficit_dtu: Deficit value for the "current" schedule (fair scheduler only).
	 *
	 * The deficit is computed before the access start, and summed once the access is done.
	 * A negative value or zero is expected for the session which has the access.
	 * A positive value or zero is expected for sessions which don't have the access.
	 *
	 * When the value is zero, the session were not in conflit with another sessions.
	 * In case of overlap between 2 or more sessions, the amount added or removed depends of the
	 * priority of all the sessions which are in conflit together.
	 */
	int32_t deficit_dtu;
	/**
	 * @deficit_sum_dtu: Sum of deficit values (fair scheduler only).
	 *
	 * It represents the balance of the session when a collision happen.
	 * The session that has the highest value will get the ranging access, and will be credited
	 * (negative value) of the access duration. So the sum decrease when the access is won.
	 *
	 * The variable is not allowed to have an overflow. The range is limited from INT32_MIN to
	 * INT32_MAX.
	 */
	int32_t deficit_sum_dtu;
	/**
	 * @last_access_timestamp_dtu: Last timestamp where the session got the access.
	 * It's used only by the session scheduler as the last criteria to elect a session.
	 */
	uint32_t last_access_timestamp_dtu;
	/** @priority: Copy of the priority in the session parameters. */
	uint8_t priority;
	/**
	 * @consecutive_schedule_left: Number of consecutive schedule to do with the same session.
	 *
	 * When the value reaches zero, the scheduler is authorized to elect another session.
	 * The scheduler will be allowed to change of session too when the session doesn't exist or
	 * has no access demand.
	 */
	uint8_t consecutive_schedule_left;
	/**
	 * @consecutive_schedule_restart: True when the session has to restart the consecutive
	 * scheduler counter.
	 */
	bool consecutive_schedule_restart;
};

/**
 * fbs_scheduler_build_block_start_dtu() - Compute time0_dtu for this session
 *   depending of block_duration_rx_margin_ppm
 * @fbs_session: FBS session context.
 * @time0_ns: Start date requested, or 0 for NA.
 *
 * Return: The reason code.
 */
enum quwbs_fbs_reason_code fbs_scheduler_build_block_start_dtu(struct fbs_session *fbs_session,
							       uint64_t time0_ns);
