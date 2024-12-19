/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "ca.h"
#include "fproc.h"
#include "linux/workqueue.h"
#include "mcps802154_se.h"
#include "net/mac802154.h"
#include "net/mcps802154.h"
#include "net/mcps802154_nl.h"
#include "qlist.h"
#include "qmutex.h"
#include "qsemaphore.h"
#include "qtils.h"

/**
 * enum mcps802154_pib_flags - PIB to reset flags.
 *
 * @MCPS802154_PIB_SADDR_CHANGED: The short address need to be reset.
 * @MCPS802154_PIB_IEEEADDR_CHANGED: The extended address need to be reset.
 * @MCPS802154_PIB_PANID_CHANGED: The pan id need to be reset.
 * @MCPS802154_PIB_PANC_CHANGED: The PAN coordinator behaving need to be reset.
 * @MCPS802154_PIB_PROMISCUOUS_CHANGED: The promiscuous need to be reset.
 * @MCPS802154_PIB_CHANNEL_CHANGED: The channel need to be reset.
 * @MCPS802154_PIB_UWB_HRP_PARAMS_CHANGED: The uwb hrp params need to be reset.
 * @MCPS802154_PIB_AFILT_MASK: Mask of only IEEE802154 part.
 * @MCPS802154_PIB_ALL_MASK: Mask of all fields.
 */
enum mcps802154_pib_flags {
	MCPS802154_PIB_SADDR_CHANGED = IEEE802154_AFILT_SADDR_CHANGED,
	MCPS802154_PIB_IEEEADDR_CHANGED = IEEE802154_AFILT_IEEEADDR_CHANGED,
	MCPS802154_PIB_PANID_CHANGED = IEEE802154_AFILT_PANID_CHANGED,
	MCPS802154_PIB_PANC_CHANGED = IEEE802154_AFILT_PANC_CHANGED,

	/* Starting at bit 16 as other values are reserved for IEEE802154. */
	MCPS802154_PIB_PROMISCUOUS_CHANGED = BIT(16),
	MCPS802154_PIB_CHANNEL_CHANGED = BIT(17),
	MCPS802154_PIB_UWB_HRP_PARAMS_CHANGED = BIT(18),

	MCPS802154_PIB_AFILT_MASK = MCPS802154_PIB_SADDR_CHANGED | MCPS802154_PIB_IEEEADDR_CHANGED |
				    MCPS802154_PIB_PANID_CHANGED | MCPS802154_PIB_PANC_CHANGED,
	MCPS802154_PIB_ALL_MASK = MCPS802154_PIB_AFILT_MASK | MCPS802154_PIB_PROMISCUOUS_CHANGED |
				  MCPS802154_PIB_CHANNEL_CHANGED |
				  MCPS802154_PIB_UWB_HRP_PARAMS_CHANGED,
};

/**
 * struct mcps802154_pib - PIB (PAN Information Base): this is a database of
 * 802.15.4 settings.
 */
struct mcps802154_pib {
	/**
	 * @mac_extended_addr: Current extended address.
	 */
	uint64_t mac_extended_addr;
	/**
	 * @mac_pan_id: The identifier of the PAN on which the device is
	 * operating. 0xffff if the device is not associated.
	 */
	uint16_t mac_pan_id;
	/**
	 * @mac_short_addr: The address the device uses to communicate inside
	 * its PAN. 0xffff if the device is not associated, 0xfffe if the device
	 * is associated but has no short address.
	 */
	uint16_t mac_short_addr;
	/**
	 * @mac_promiscuous: Indicate whether the promiscuous mode is enabled.
	 */
	bool mac_promiscuous;
	/**
	 * @mac_max_frame_retries: Number of retries on TX.
	 */
	int8_t mac_max_frame_retries;
	/**
	 * @phy_current_channel: Current channel parameters.
	 */
	struct mcps802154_channel phy_current_channel;
	/**
	 * @hrp_uwb_params: Current HRP UWB parameters.
	 */
	struct mcps802154_hrp_uwb_params hrp_uwb_params;
};

/**
 * struct mcps802154_local - MCPS private data.
 */
struct mcps802154_local {
	/**
	 * @llhw: Low-level hardware.
	 */
	struct mcps802154_llhw llhw;
	/**
	 * @mcps_se: Secure element private context.
	 */
	struct mcps802154_se mcps_se;
	/**
	 * @hw: Pointer to MCPS hw instance.
	 */
	struct ieee802154_hw *hw;
	/**
	 * @ops: Low-level driver operations.
	 */
	const struct mcps802154_ops *ops;
	/**
	 * @mcps_nl: MCPS Netlink instance.
	 */
	struct mcps802154_nl mcps_nl;
	/**
	 * @ca: Channel access context.
	 */
	struct mcps802154_ca ca;
	/**
	 * @fproc: Frame processing context.
	 */
	struct mcps802154_fproc fproc;
	/**
	 * @fsm_lock: FSM lock to avoid multiple access.
	 */
	struct qmutex *fsm_lock;
	/**
	 * @state_sem: Semaphore for asynchronous stopping implementation.
	 */
	struct qsemaphore *state_sem;
	/**
	 * @tx_work: Transmit work to schedule async actions.
	 */
	struct work_struct tx_work;
	/**
	 * @start_stop_request: Request to start (true) or stop (false) from
	 * mac802154 layer.
	 */
	bool start_stop_request;
	/**
	 * @started: Current started state.
	 */
	bool started;
	/**
	 * @broken: Currently broken.
	 */
	bool broken;
	/**
	 * @pib: PAN Information Base.
	 */
	struct mcps802154_pib pib;
	/**
	 * @mac_pan_coord: Indicate whether the hardware filtering should
	 * operate as coordinator.
	 */
	bool mac_pan_coord;
	/**
	 * @pib_set_mask: Indicate which pib need to be reset to default.
	 */
	enum mcps802154_pib_flags pib_set_mask;
	/**
	 * @report_queue: Queue of report frame to be processed.
	 */
	struct sk_buff_head report_queue;
	/**
	 * @report_work: Process work of report event.
	 */
	struct work_struct report_work;
};

static inline struct mcps802154_local *llhw_to_local(struct mcps802154_llhw *llhw)
{
	return qparent_of(llhw, struct mcps802154_local, llhw);
}

static inline struct mcps802154_local *txwork_to_local(struct work_struct *tx_work)
{
	return qparent_of(tx_work, struct mcps802154_local, tx_work);
}

static inline struct mcps802154_local *mcps_nl_to_local(struct mcps802154_nl *mcps_nl)
{
	return qparent_of(mcps_nl, struct mcps802154_local, mcps_nl);
}

extern const struct ieee802154_ops mcps802154_ops;

/**
 * mcps802154_region_init() - Initialize region generic stuff (i.e. event
 * queue and workqueue ...)
 * @local: mcps802154 private data
 */
void mcps802154_region_init(struct mcps802154_local *local);

/**
 * mcps802154_region_exit() - Free/clean region generic stuff (i.e. event
 * queue and workqueue ...)
 * @local: mcps802154 private data
 */
void mcps802154_region_exit(struct mcps802154_local *local);

/**
 * mcps802154_region_event() - Send a previously allocated and filled
 * buffer.
 * @llhw: Low-level device pointer.
 * @skb: Buffer to send.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_region_event(struct mcps802154_llhw *llhw, struct sk_buff *skb);

/**
 * mcps802154_notify_packet_drop_and_release_buffer() - Notify packet drop
 * and release buffer.
 * @skb: Message buffer to be released.
 *
 */
void mcps802154_notify_packet_drop_and_release_buffer(struct sk_buff *skb);
