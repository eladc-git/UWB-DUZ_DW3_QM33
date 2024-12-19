/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/lld_region_common_nl.h"
#include "net/lldd_region_nl.h"
#include "net/mcps802154_schedule.h"
#include "qerr.h"

#include <stdbool.h>
#include <stdint.h>

/* Forward declaration. */
struct lldd_local;
struct lldd_frame_beacon_info;
struct genl_info;
struct sk_buff;

/**
 * struct lldd_fsm_state - LLDC FSM state.
 *
 * This structure contains the callbacks which are called on an event to handle
 * the transition from the active state.
 */
struct lldd_fsm_state {
	/** @id: name of the state. */
	enum lldd_state_id id;
	/** @enter: Run when the state is entered. */
	void (*enter)(struct lldd_local *local);
	/** @leave: Run when the state is left. */
	void (*leave)(struct lldd_local *local);
	/** @start: Handle start request. */
	enum qerr (*start)(struct lldd_local *local, const struct genl_info *info);
	/** @stop: Handle stop request. */
	enum qerr (*stop)(struct lldd_local *local);
	/** @track: Handle track request. */
	enum qerr (*track)(struct lldd_local *local, const struct genl_info *info, uint16_t pan_id,
			   uint16_t coord_short_addr);
	/** @track_found: Handle tracked network found event. */
	void (*track_found)(struct lldd_local *local,
			    const struct lldd_frame_beacon_info *beacon_info,
			    uint32_t beacon_timestamp_dtu);
	/** @associate: Handle association request. */
	enum qerr (*associate)(struct lldd_local *local, const struct genl_info *info,
			       const uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN],
			       const uint8_t *payload, int payload_len);
	/** @associated: Handle association success. */
	void (*associated)(struct lldd_local *local, uint16_t short_addr, int ul_slot);
	/** @ul_ack: Handle ack of last UL. */
	void (*ul_ack)(struct lldd_local *local, bool acked);
	/** @xmit_skb: Handle xmit_skb. */
	int (*xmit_skb)(struct lldd_local *local, struct sk_buff *skb);
	/** @get_demand: Handle get_demand. */
	int (*get_demand)(struct lldd_local *local, uint32_t next_timestamp_dtu,
			  int max_duration_dtu, struct mcps802154_region_demand *demand);
	/** @get_access: Handle get_access. */
	struct mcps802154_access *(*get_access)(struct lldd_local *local,
						uint32_t next_timestamp_dtu);
	/** @access_done: Handle access_done. */
	void (*access_done)(struct lldd_local *local, enum mcps802154_access_done_reason reason);
};

void lldd_fsm_init(struct lldd_local *local);

void lldd_fsm_uninit(struct lldd_local *local);

void lldd_fsm_change_state(struct lldd_local *local, const struct lldd_fsm_state *new_state);

enum qerr lldd_fsm_start(struct lldd_local *local, const struct genl_info *info);

enum qerr lldd_fsm_stop(struct lldd_local *local);

enum qerr lldd_fsm_track(struct lldd_local *local, const struct genl_info *info, uint16_t pan_id,
			 uint16_t coord_short_addr);

void lldd_fsm_track_found(struct lldd_local *local,
			  const struct lldd_frame_beacon_info *beacon_info,
			  uint32_t beacon_timestamp_dtu);

void lldd_fsm_ul_ack(struct lldd_local *local, bool acked);

enum qerr lldd_fsm_associate(struct lldd_local *local, const struct genl_info *info,
			     const uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN],
			     const uint8_t *payload, int payload_len);

void lldd_fsm_associated(struct lldd_local *local, uint16_t short_addr, int ul_slot);

int lldd_fsm_get_demand(struct lldd_local *local, uint32_t next_timestamp_dtu, int max_duration_dtu,
			struct mcps802154_region_demand *demand);

struct mcps802154_access *lldd_fsm_get_access(struct lldd_local *local,
					      uint32_t next_timestamp_dtu);

void lldd_fsm_access_done(struct lldd_local *local, enum mcps802154_access_done_reason reason);

int lldd_fsm_xmit_skb(struct lldd_local *local, struct sk_buff *skb);
