/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154.h"
#include "net/mcps802154_nl_defs.h"
#include "net/mcps802154_schedule.h"
#include "qerr.h"

#include <stddef.h>
#include <stdint.h>

struct mcps802154_local;

#define MCPS802154_FPROC_VENDOR_CONTINUE 0
#define MCPS802154_FPROC_VENDOR_STOP 1
#define MCPS802154_FPROC_VENDOR_CANCEL 2
#define MCPS802154_FPROC_VENDOR_ERROR 3

/**
 * struct mcps802154_fproc_state - FProc FSM state.
 *
 * This structure contains the callbacks which are called on an event to handle
 * the transition from the active state.
 */
struct mcps802154_fproc_state {
	/** @name: State name. */
	const char *name;
	/** @enter: Run when the state is entered. */
	void (*enter)(struct mcps802154_local *local);
	/** @leave: Run when the state is left. */
	void (*leave)(struct mcps802154_local *local);
	/** @rx_frame: Handle frame reception. */
	void (*rx_frame)(struct mcps802154_local *local);
	/** @rx_timeout: Handle reception timeout. */
	void (*rx_timeout)(struct mcps802154_local *local);
	/** @rx_error: Handle reception error. */
	void (*rx_error)(struct mcps802154_local *local, enum mcps802154_rx_error_type error);
	/** @tx_done: Handle end of transmission. */
	void (*tx_done)(struct mcps802154_local *local);
	/** @broken: Handle unrecoverable error. */
	void (*broken)(struct mcps802154_local *local);
	/** @timer_expired: Handle timer expiration, ignored if NULL. */
	void (*timer_expired)(struct mcps802154_local *local);
	/** @schedule_change: Handle schedule change. */
	void (*schedule_change)(struct mcps802154_local *local);
};

/** struct mcps802154_fproc - FProc private data. */
struct mcps802154_fproc {
	/** @state: Pointer to current state. */
	const struct mcps802154_fproc_state *state;
	/** @access: Access being handled. */
	struct mcps802154_access *access;
	/** @tx_skb: Buffer for frame being sent. */
	struct sk_buff *tx_skb;
	/** @frame_idx: Frame index for multiple frames method. */
	size_t frame_idx;
	/** @frame: Frame information used for Tx/Rx. */
	const struct mcps802154_access_frame *frame;
	/** @deferred: Pointer to region context requesting deferred call. */
	struct mcps802154_region *deferred;
};

extern const struct mcps802154_fproc_state mcps802154_fproc_stopped;

/**
 * mcps802154_fproc_init() - Initialize FProc.
 * @local: MCPS private data.
 */
void mcps802154_fproc_init(struct mcps802154_local *local);

/**
 * mcps802154_fproc_uninit() - Uninitialize FProc.
 * @local: MCPS private data.
 */
void mcps802154_fproc_uninit(struct mcps802154_local *local);

/**
 * mcps802154_fproc_device_state() - Report device state.
 * @local: MCPS private data.
 * @state: New device state.
 */
void mcps802154_fproc_device_state(struct mcps802154_local *local,
				   enum mcps802154_device_state state);

/**
 * mcps802154_fproc_change_state() - Change the active state.
 * @local: MCPS private data.
 * @new_state: State to switch to.
 */
void mcps802154_fproc_change_state(struct mcps802154_local *local,
				   const struct mcps802154_fproc_state *new_state);

/**
 * mcps802154_fproc_access() - Get access and handle it.
 * @local: MCPS private data.
 * @next_timestamp_dtu: Date of next access opportunity.
 */
void mcps802154_fproc_access(struct mcps802154_local *local, uint32_t next_timestamp_dtu);

/**
 * mcps802154_fproc_access_now() - Get access for current date, and handle it.
 * @local: MCPS private data.
 */
void mcps802154_fproc_access_now(struct mcps802154_local *local);

/**
 * mcps802154_fproc_access_done() - Done with the access, release it.
 * @local: MCPS private data.
 * @reason: Reason of the end of access.
 */
void mcps802154_fproc_access_done(struct mcps802154_local *local,
				  enum mcps802154_access_done_reason reason);

/**
 * mcps802154_fproc_access_done_r() - Done with the access, release it, use
 * error code for reason.
 * @local: MCPS private data.
 * @r: error code, 0 or negative error.
 */
static inline void mcps802154_fproc_access_done_r(struct mcps802154_local *local, int r)
{
	mcps802154_fproc_access_done(local, r ? MCPS802154_ACCESS_DONE_REASON_ERROR :
						MCPS802154_ACCESS_DONE_REASON_SUCCESS);
}

/**
 * mcps802154_fproc_access_done_success() - Done with the access with success,
 * release it.
 * @local: MCPS private data.
 */
static inline void mcps802154_fproc_access_done_success(struct mcps802154_local *local)
{
	mcps802154_fproc_access_done(local, MCPS802154_ACCESS_DONE_REASON_SUCCESS);
}

/**
 * mcps802154_fproc_access_done_error() - Done with the access with error,
 * release it.
 * @local: MCPS private data.
 */
static inline void mcps802154_fproc_access_done_error(struct mcps802154_local *local)
{
	mcps802154_fproc_access_done(local, MCPS802154_ACCESS_DONE_REASON_ERROR);
}

/**
 * mcps802154_fproc_access_done_cancel() - Done with the access because it was
 * canceled, release it.
 * @local: MCPS private data.
 */
static inline void mcps802154_fproc_access_done_cancel(struct mcps802154_local *local)
{
	mcps802154_fproc_access_done(local, MCPS802154_ACCESS_DONE_REASON_CANCEL);
}

/**
 * mcps802154_fproc_access_reset() - Reset an access when things go wrong.
 * @local: MCPS private data.
 *
 * When an unexpected event is received, current transmitted frame and current
 * access are kept as the frame buffer is possibly used by the low level driver.
 * Later when the driver is reset or stopped, the buffer and the access can be
 * released.
 */
void mcps802154_fproc_access_reset(struct mcps802154_local *local);

/**
 * mcps802154_fproc_set_rf_params() - Set access or default RF parameters.
 * @local: MCPS private data.
 * @access: Current access to handle.
 *
 * Helper to set channel, preamble code and HRP UWB parameters.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_fproc_set_rf_params(struct mcps802154_local *local,
					 struct mcps802154_access *access);

/**
 * mcps802154_fproc_schedule_change() - Try a schedule change.
 * @local: MCPS private data.
 *
 * Inform the current state that the schedule has changed. To be called
 * exclusively from CA.
 */
void mcps802154_fproc_schedule_change(struct mcps802154_local *local);

/**
 * mcps802154_fproc_stopped_handle() - Go to stopped.
 * @local: MCPS private data.
 */
void mcps802154_fproc_stopped_handle(struct mcps802154_local *local);

/**
 * mcps802154_fproc_broken_handle() - Go to broken, or directly to stopped.
 * @local: MCPS private data.
 */
void mcps802154_fproc_broken_handle(struct mcps802154_local *local);

/**
 * mcps802154_fproc_nothing_handle() - Handle inactivity.
 * @local: MCPS private data.
 * @access: Current access to handle.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_fproc_nothing_handle(struct mcps802154_local *local,
					  struct mcps802154_access *access);

/**
 * mcps802154_fproc_rx_handle() - Handle an RX access and change state.
 * @local: MCPS private data.
 * @access: Current access to handle.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_fproc_rx_handle(struct mcps802154_local *local,
				     struct mcps802154_access *access);

/**
 * mcps802154_fproc_tx_handle() - Handle an TX access and change state.
 * @local: MCPS private data.
 * @access: Current access to handle.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_fproc_tx_handle(struct mcps802154_local *local,
				     struct mcps802154_access *access);

/**
 * mcps802154_fproc_multi_handle() - Handle a multiple frames access and change
 * state.
 * @local: MCPS private data.
 * @access: Current access to handle.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_fproc_multi_handle(struct mcps802154_local *local,
					struct mcps802154_access *access);

/**
 * mcps802154_fproc_vendor_handle() - Handle a multiple frames access manage by
 * vendor.
 * @local: MCPS private data.
 * @access: Current access to handle.
 *
 * Return: QERR or positive return code.
 */
int mcps802154_fproc_vendor_handle(struct mcps802154_local *local,
				   struct mcps802154_access *access);

/**
 * mcps802154_fproc_step_handle() - Handle a frame by frame access.
 * @local: MCPS private data.
 * @access: Current access to handle.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_fproc_step_handle(struct mcps802154_local *local,
				       struct mcps802154_access *access);

/**
 * mcps802154_fproc_rx_disable() - Cancel the Rx frame.
 * @local: MCPS private data.
 *
 * Temporary function until the usage removed from Radar.
 *
 * Return: see return value of @llhw_rx_disable().
 */
enum qerr mcps802154_fproc_rx_disable(struct mcps802154_local *local);
