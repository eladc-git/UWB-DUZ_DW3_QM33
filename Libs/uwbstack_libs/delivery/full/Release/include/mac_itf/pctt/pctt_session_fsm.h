/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/pctt_region_nl.h"
#include "net/pctt_region_params.h"

#include <stdbool.h>
#include <stdint.h>

/* Forward declaration. */
struct pctt_local;
struct genl_info;
struct nlattr;

/**
 * struct pctt_session_fsm_state - PCTT FSM state.
 *
 * This structure contains the callbacks which are called on an event to handle
 * the transition from the current state.
 */
struct pctt_session_fsm_state {
	/** @id: name of the state. */
	enum pctt_session_state id;
	/** @enter: Run when the state is entered. */
	void (*enter)(struct pctt_local *local);
	/** @leave: Run when the state is left. */
	void (*leave)(struct pctt_local *local);
	/** @call_init: Handle INIT call. */
	enum qerr (*call_init)(struct pctt_local *local);
	/** @call_deinit: Handle DEINIT call. */
	enum qerr (*call_deinit)(struct pctt_local *local);
	/** @check_parameters: Handle a check parameters. */
	enum qerr (*check_parameters)(struct pctt_local *local, struct nlattr **attrs);
	/** @parameters_updated: Handle parameters updated event. */
	void (*parameters_updated)(struct pctt_local *local);
	/** @start: Handle start request. */
	enum qerr (*start)(struct pctt_local *local, enum pctt_id_attrs cmd_id,
			   const struct genl_info *info);
	/** @stop: Handle stop request. */
	enum qerr (*stop)(struct pctt_local *local);
	/** @get_access: Handle get_access. */
	struct mcps802154_access *(*get_access)(struct pctt_local *local,
						uint32_t next_timestamp_dtu);
	/** @access_done: Handle access_done. */
	void (*access_done)(struct pctt_local *local);
};

/**
 * pctt_session_fsm_initialize() - Initialize the FSM.
 * @local: PCTT context.
 */
void pctt_session_fsm_initialize(struct pctt_local *local);

/**
 * pctt_session_fsm_uninitialize() - Uninitialize the FSM.
 * @local: PCTT context.
 */
void pctt_session_fsm_uninitialize(struct pctt_local *local);

/**
 * pctt_session_fsm_change_state() - Change the state of the FSM.
 * @local: PCTT context.
 * @new_state: New to state to use in the FSM.
 *
 * This function shall be called only by pctt_fsm files.
 */
void pctt_session_fsm_change_state(struct pctt_local *local,
				   const struct pctt_session_fsm_state *new_state);

/**
 * pctt_session_fsm_call_init() - Process INIT request from upper layer.
 * @local: PCTT context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_session_fsm_call_init(struct pctt_local *local);

/**
 * pctt_session_fsm_call_deinit() - Process DEINIT request from upper layer.
 * @local: PCTT context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_session_fsm_call_deinit(struct pctt_local *local);

/**
 * pctt_session_fsm_check_parameters() - Check parameters change ask by upper
 * layer.
 * @local: PCTT context.
 * @attrs: Array of parameters attributes.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_session_fsm_check_parameters(struct pctt_local *local, struct nlattr **attrs);

/**
 * pctt_session_fsm_parameters_updated() - Update state according with
 * parameters.
 * @local: PCTT context.
 */
void pctt_session_fsm_parameters_updated(struct pctt_local *local);

/**
 * pctt_session_fsm_start() - Start request from upper layer.
 * @local: PCTT context.
 * @cmd_id: Command identifier.
 * @info: Netlink info used only for the portid.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_session_fsm_start(struct pctt_local *local, enum pctt_id_attrs cmd_id,
				 const struct genl_info *info);

/**
 * pctt_session_fsm_stop() - Stop request from upper layer.
 * @local: PCTT context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_session_fsm_stop(struct pctt_local *local);

/**
 * pctt_session_fsm_get_access() - Get access.
 * @local: PCTT context.
 * @next_timestamp_dtu: Next timestamp dtu opportunity.
 *
 * Return: Access pointer or null when rejected.
 */
struct mcps802154_access *pctt_session_fsm_get_access(struct pctt_local *local,
						      uint32_t next_timestamp_dtu);

/**
 * pctt_session_fsm_access_done() - Access done notified to fsm.
 * @local: PCTT context.
 */
void pctt_session_fsm_access_done(struct pctt_local *local);

/**
 * pctt_session_fsm_is_deinit() - Inform if the session exist or not.
 * @local: PCTT context.
 *
 * Return: True when the session have been initialized.
 */
bool pctt_session_fsm_is_deinit(struct pctt_local *local);
