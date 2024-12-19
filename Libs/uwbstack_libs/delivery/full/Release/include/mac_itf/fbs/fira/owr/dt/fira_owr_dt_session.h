/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154.h"
#include "qlist.h"

#include <stdint.h>

struct fira_owr_dt_responder {
	/**
	 * @entry: Entry in the responders list.
	 * @see struct fira_owr_dt_anchor_round.
	 */
	struct qlist_head entry;
	/**
	 * @addr: Address of the Responder.
	 */
	mcps802154_device_address addr;
	/**
	 * @slot_index: Index of the slot assigned to this Responder
	 * in the Round.
	 */
	uint8_t slot_index;
	/**
	 * @tof_result: Time Of Flight as a result of SS-TWR by Initiator
	 * during previous Round. Warning: the value could be negative.
	 */
	int16_t tof_rctu;
};

/**
 * fira_session_owr_dt_get_max_rounds_in_block() - Calculate the maximum number
 * of ranging rounds inside a ranging block based on the session parameters.
 * @session: FiRa session context.
 *
 * Return: Maximum number of ranging rounds inside a ranging block.
 */
static inline int fira_session_owr_dt_get_max_rounds_in_block(const struct fira_session *session)
{
	const struct fbs_session_runtime *runtime = session->base.runtime;
	int max_slots_in_ranging_block = runtime->block_duration_dtu / runtime->slot_duration_dtu;
	int max_rounds_in_ranging_block =
		max_slots_in_ranging_block / session->params.round_duration_slots;

	return max_rounds_in_ranging_block;
}
