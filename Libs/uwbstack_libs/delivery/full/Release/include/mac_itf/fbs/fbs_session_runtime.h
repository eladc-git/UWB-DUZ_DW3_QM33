/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/fbs_scheduler.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * struct fbs_session_runtime - Base struct for all FBS session type
 * runtime context storage.
 */
struct fbs_session_runtime {
	/** @scheduler_info: Informations to be used only by the scheduler. */
	struct fbs_scheduler_session_info scheduler_info;
	/** @block_index: Block index used on the last access. */
	uint32_t block_index;
	/** @slot_duration_dtu: Slot duration in DTU. */
	int32_t slot_duration_dtu;
	/** @block_duration_dtu: Block duration in DTU. */
	int32_t block_duration_dtu;
	/**
	 * @rx_margin_dtu: Rx margin used by a session synchronized on its
	 * first Rx slot.
	 */
	int32_t rx_margin_dtu;
	/**
	 * @block_start_dtu: Block start timestamp in dtu of the last
	 * get_access.
	 */
	uint32_t block_start_dtu;
	/** @schedule_role: Schedule role profile. */
	enum fbs_schedule_role schedule_role;
	/**
	 * @block_start_valid: True when block_start_dtu is valid.
	 * It's false on the first access wo initiation delay.
	 */
	bool block_start_valid;
	/** @stop_request: Session stop has been requested. */
	bool stop_request;
	/** @synchronized: Whether a session is synchronized. */
	bool synchronized;
	/** @deinit_request: Session deinit has been requested. Only applicable for a phase */
	bool deinit_request;
};
