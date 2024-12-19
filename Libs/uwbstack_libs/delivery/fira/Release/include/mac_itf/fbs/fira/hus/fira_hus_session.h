/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fira/fira_session.h"
#include "qlist.h"
#include "qtils.h"

#include <stdbool.h>
#include <stdint.h>

struct fira_hus_controller_phase {
	/**
	 * @entry: Entry in the list of phases of the HUS controller.
	 * @see struct hus_controller_phases.
	 */
	struct qlist_head entry;
	/**
	 * @addr: Address of the Controller of the phase.
	 */
	mcps802154_device_address addr;

	/**
	 * start_slot_index: Slot index of the first slot of the phase.
	 */
	uint16_t start_slot_index;
	/**
	 * end_slot_index: Slot index of the last slot of the phase.
	 */
	uint16_t end_slot_index;
	/**
	 * @session_id: Session ID of the phase.
	 */
	uint32_t session_id;
	/**
	 * @indicator: Phase type (0=CAP/1=CFP).
	 */
	uint8_t indicator;
	/**
	 * @matching_session: Pointer to the session which will be scheduled in the corresponding
	 * phase.
	 */
	struct fbs_session *matching_session;
};

struct fira_hus_session {
	/**
	 * @base: FiRa Session context.
	 */
	struct fira_session fira;
	/**
	 * @controller_phases: List of phases scheduled by the HUS controller. On the Controller,
	 * this list is filled from a UCI message. On the Controlee, this list is filled from
	 * the Control Message Type 3 received in-band.
	 * @see struct fira_hus_controller_phase.
	 */
	struct qlist_head controller_phases;
	/**
	 * @n_controller_phases: Number of elements in the list of controller phases.
	 */
	uint8_t n_controller_phases;
	/**
	 * @sub_access: Access ops of the phase which is currently scheduled.
	 */
	const struct mcps802154_access_ops *sub_access;
	/**
	 * @current_phase: Pointer to the phase which is currently scheduled by the HUS.
	 */
	struct fira_hus_controller_phase *current_phase;
	/**
	 * @start_timestamp_dtu: Timestamp of the beginning of the HUS, which is used to perform
	 * correct slot allocation.
	 */
	uint32_t start_timestamp_dtu;
};

static inline struct fira_hus_session *fira_to_hus(const struct fira_session *fira_session)
{
	return qparent_of(fira_session, struct fira_hus_session, fira);
}
