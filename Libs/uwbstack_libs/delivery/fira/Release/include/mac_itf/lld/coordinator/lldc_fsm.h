/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

/* Forward declaration. */
struct lldc_local;
struct genl_info;
struct mcps802154_region_demand;
struct sk_buff;

/**
 * enum lldc_state_id - State of the coordinator.
 * @LLDC_STATE_ID_STANDBY:
 *     Standby state.
 * @LLDC_STATE_ID_SCANNING:
 *     Scanning state.
 * @LLDC_STATE_ID_PROBING:
 *     Probing state.
 * @LLDC_STATE_ID_ACTIVE:
 *     Active state.
 */
enum lldc_state_id {
	LLDC_STATE_ID_STANDBY,
	LLDC_STATE_ID_SCANNING,
	LLDC_STATE_ID_PROBING,
	LLDC_STATE_ID_ACTIVE,
};

/**
 * struct lldc_fsm_state - LLDC FSM state.
 *
 * This structure contains the callbacks which are called on an event to handle
 * the transition from the active state.
 */
struct lldc_fsm_state {
	/** @id: name of state. */
	enum lldc_state_id id;
	/** @enter: Run when the state is entered. */
	void (*enter)(struct lldc_local *local);
	/** @leave: Run when the state is left. */
	void (*leave)(struct lldc_local *local);
	/** @start: Handle start. */
	enum qerr (*start)(struct lldc_local *local, const struct genl_info *info);
	/** @stop: Handle stop. */
	enum qerr (*stop)(struct lldc_local *local);
	/** @get_demand: Handle get_demand. */
	int (*get_demand)(struct lldc_local *local, uint32_t next_timestamp_dtu,
			  int max_duration_dtu, struct mcps802154_region_demand *demand);
	/** @get_access: Handle get_access. */
	struct mcps802154_access *(*get_access)(struct lldc_local *local,
						uint32_t next_timestamp_dtu);
	/** @xmit_skb: Handle xmit_skb. */
	int (*xmit_skb)(struct lldc_local *local, struct sk_buff *skb);
};

void lldc_fsm_init(struct lldc_local *local);

void lldc_fsm_uninit(struct lldc_local *local);

void lldc_fsm_change_state(struct lldc_local *local, const struct lldc_fsm_state *new_state);

int lldc_fsm_get_demand(struct lldc_local *local, uint32_t next_timestamp_dtu, int max_duration_dtu,
			struct mcps802154_region_demand *demand);

struct mcps802154_access *lldc_fsm_get_access(struct lldc_local *local,
					      uint32_t next_timestamp_dtu);

enum qerr lldc_fsm_start(struct lldc_local *local, const struct genl_info *info);

enum qerr lldc_fsm_stop(struct lldc_local *local);

int lldc_fsm_xmit_skb(struct lldc_local *local, struct sk_buff *skb);
