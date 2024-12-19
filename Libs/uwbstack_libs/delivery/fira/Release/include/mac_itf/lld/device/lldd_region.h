/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "linux/skbuff.h"
#include "lld/device/lldd_frame.h"
#include "lld/device/lldd_stats.h"
#include "lld/lld_common.h"
#include "lld/lld_frame.h"
#include "lld/lld_mfs.h"
#include "net/lld_region_common_nl.h"
#include "net/mcps802154.h"
#include "net/mcps802154_schedule.h"
#include "qtils.h"

#include <stdbool.h>
#include <stdint.h>

struct lldd_local;
#include "linux/workqueue.h"
#include "net/lldd_region_nl.h"

#define LLDD_FRAMES_MAX 2
#define LLDD_INACTIVE_EXPONENT_MAX 14
#define LLDD_TRACKING_LOSS_THRESHOLD_MIN 1
#define LLDD_TRACKING_LOSS_THRESHOLD_MAX 65535
#define LLDD_TRACKING_LOSS_THRESHOLD_DEFAULT 8

/**
 * struct lldd_params - Parameters.
 */
struct lldd_params {
	/**
	 * @agg_limit_bytes: Limit MPDU aggregated payload size after the first
	 * sub-frame.
	 */
	int agg_limit_bytes;
	/**
	 * @smart_power: True to enable smart power feature, false otherwise.
	 */
	bool smart_power;
	/**
	 * @inactive_exponent: If not 0, the device will behave as an inactive
	 * device. It will participate only in blocks with an index which is a
	 * multiple of 2 to the power of inactiveExponent.
	 */
	int inactive_exponent;
	/**
	 * @tracking_loss_threshold: Number of consecutive failed blocks for a
	 * device to declare tracking loss.
	 */
	int tracking_loss_threshold;
	/**
	 * @mfs_params: MFS parameters.
	 */
	struct lld_mfs_params mfs_params[LLD_MFS_ID_NB];
};

/**
 * enum lldd_subaccess_type - Sub-access type, piece of access.
 */
enum lldd_subaccess_type {
	/**
	 * @LLDD_SUBACCESS_TYPE_ADDITIONAL_CAP: Additional CAP in Sub-block
	 * with combined CAP.
	 */
	LLDD_SUBACCESS_TYPE_ADDITIONAL_CAP,
	/**
	 * @LLDD_SUBACCESS_TYPE_AUX: Auxiliary sub-block.
	 */
	LLDD_SUBACCESS_TYPE_AUX,
	/**
	 * @LLDD_SUBACCESS_TYPE_NON_ASSOCIATED: Send a non-associated frame.
	 */
	LLDD_SUBACCESS_TYPE_NON_ASSOCIATED,
	/**
	 * @LLDD_SUBACCESS_TYPE_NOTHING: No sub-access claim.
	 */
	LLDD_SUBACCESS_TYPE_NOTHING,
	/**
	 * @LLDD_SUBACCESS_TYPE_MAIN: Main sub-block.
	 */
	LLDD_SUBACCESS_TYPE_MAIN,
};

/**
 * struct lldd_inactive_sent_info - Sent inactive IE information.
 */
struct lldd_inactive_sent_info {
	/**
	 * @exponent: Value sent in an inactive IE in indicated block.
	 */
	uint8_t exponent;
	/**
	 * @block_index: Block index in which inactive IE was sent.
	 */
	uint32_t block_index;
};

/**
 * struct lldd_deferred - Deferred context.
 */
struct lldd_deferred {
	/**
	 * @cb: Deferred callback.
	 */
	void (*cb)(struct lldd_local *);
	/**
	 * @skb: Buffer to handle.
	 */
	struct sk_buff *skb;
	/**
	 * @skb_len: Original buffer length, or 0 if frame is invalid.
	 */
	int skb_len;
	/**
	 * @timestamp_dtu: Timestamp in frame if frame valid.
	 */
	uint32_t timestamp_dtu;
	/**
	 * @rx_timestamp_dtu: Reception timestamp if frame valid.
	 */
	uint32_t rx_timestamp_dtu;
	/**
	 * @lqi: LQI of received frame if frame valid.
	 */
	uint8_t lqi;
	/**
	 * @aux_subblock: Whether this is an auxiliary sub-block or not.
	 */
	bool aux_subblock;
};

/**
 * struct lldd_assoc_req_info - Association request context.
 */
struct lldd_assoc_req_info {
	/**
	 * @dev_key: Device identity key.
	 */
	uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN];
	/**
	 * @payload: Payload embedded in the association request.
	 */
	uint8_t payload[LLD_ASSOCIATION_PAYLOAD_LEN_MAX];
	/**
	 * @payload_len: Payload length.
	 */
	int payload_len;
};

/**
 * struct lldd_local - Local context.
 */
struct lldd_local {
	/**
	 * @region: Region instance returned to MCPS.
	 */
	struct mcps802154_region region;
	/**
	 * @llhw: Low-level device pointer.
	 */
	struct mcps802154_llhw *llhw;
	/**
	 * @access: Access returned to MCPS.
	 */
	struct mcps802154_access access;
	/**
	 * @frame: Access frame referenced from access.
	 */
	struct mcps802154_access_frame frame;
	/**
	 * @channel: Channel parameters.
	 */
	struct mcps802154_channel channel;
	/**
	 * @beacon_hrp_uwb_params: HRP UWB parameters for beacon.
	 */
	struct mcps802154_hrp_uwb_params beacon_hrp_uwb_params;
	/**
	 * @hrp_uwb_params: HRP UWB parameters.
	 */
	struct mcps802154_hrp_uwb_params hrp_uwb_params;
	/**
	 * @frame_mtu: Frame MTU.
	 */
	int frame_mtu;
	/**
	 * @frame_skb: Buffer used for sent frames.
	 */
	struct sk_buff *frame_skb;
	/**
	 * @state: State of the device.
	 */
	const struct lldd_fsm_state *state;
	/**
	 * @params: Parameters.
	 */
	struct lldd_params params;
	/**
	 * @stats: Statistics.
	 */
	struct lldd_stats stats;
	/**
	 * @reset_stats_request: True when a reset of the stats is requested.
	 */
	bool reset_stats_request;
	/**
	 * @network_params: Network parameters given by the coordinator.
	 */
	struct lld_network_params network_params;
	/**
	 * @mfs_tx: Tx MAC Frame Streams.
	 */
	struct lld_mfs_tx mfs_tx[LLD_MFS_ID_NB];
	/**
	 * @mfs_rx: Rx MAC Frame Streams.
	 */
	struct lld_mfs_rx mfs_rx[LLD_MFS_ID_NB];
	/**
	 * @extended_addr: Extended address.
	 */
	uint64_t extended_addr;
	/**
	 * @event_portid: Port identifier to use for notifications.
	 */
	uint32_t event_portid;
	/**
	 * @pan_id: PAN ID, or IEEE802154_PAN_ID_BROADCAST when not set.
	 */
	uint16_t pan_id;
	/**
	 * @coord_short_addr: Address of the coordinator, or
	 * LLD_SHORT_ADDR_UNDEFINED when not set.
	 */
	uint16_t coord_short_addr;
	/**
	 * @short_addr: Own short address given by coordinator, or
	 * LLD_SHORT_ADDR_UNDEFINED when not associated.
	 */
	uint16_t short_addr;
	/**
	 * @n_ul_slots: Number of UL slots.
	 */
	int n_ul_slots;
	/**
	 * @ul_slot: Assigned UL slot when associated, else -1.
	 */
	int ul_slot;
	/**
	 * @cap: Local cap information.
	 */
	struct {
		/**
		 * @tx_seq_num: Sequence number of last skb in
		 * 'tx_non_assoc_skb_queue' sent.
		 */
		int tx_seq_num;
		/**
		 * @repetition: Number of cap repetitions on combined
		 * sub-block and CAP.
		 */
		int repetition;
		/**
		 * @be: Back off exponent used by contention algorithm.
		 */
		int be;
		/**
		 * @slot: Number of cap slots to skip.
		 */
		int slot;
		/**
		 * @slot_valid: True when 'slot' value above is valid.
		 */
		bool slot_valid;
		/**
		 * @left_additional_slots: Number of additional cap slots left
		 * in subaccess ADDITIONAL_CAP type.
		 */
		int left_additional_slots;
		/**
		 * @wait_ack: True when the skb have to be dequeued on
		 * acknowledge.
		 */
		bool wait_ack;
		/**
		 * @next_sb_combined_with_cap: True when next sub-block is
		 * combined with CAP.
		 */
		bool next_sb_combined_with_cap;
	} cap;
	/**
	 * @block_start_dtu: Timestamp of the current block.
	 *
	 * The lldd_update() function updates the "current" block, sub-block,
	 * etc. The "current" term refers to the access being done or the next
	 * one do to. The lldd_update() makes sure that the "current" setup
	 * refer to an access in the future.
	 */
	uint32_t block_start_dtu;
	/**
	 * @block_index: Current block index.
	 */
	uint32_t block_index;
	/**
	 * @subaccess_timestamp_dtu: Current sub-access timestamp, in DTU.
	 */
	uint32_t subaccess_timestamp_dtu;
	/**
	 * @subblock_index: Current sub-block index.
	 */
	int subblock_index;
	/**
	 * @block_not_failed: Current block is not failed (at least one valid DL
	 * with no negative acknowledgement).
	 */
	bool block_not_failed;
	/**
	 * @tx_ul: True if an UL slot is scheduled in the current sub-block.
	 */
	bool tx_ul;
	/**
	 * @tx_ul_aux: True if an UL slot should be scheduled in the next
	 * auxiliary sub-block.
	 */
	bool tx_ul_aux;
	/**
	 * @next_subaccess_type: Next sub-access type.
	 */
	enum lldd_subaccess_type next_subaccess_type;
	/**
	 * @next_subaccess_timestamp_dtu: Next sub-access timestamp, in DTU.
	 */
	uint32_t next_subaccess_timestamp_dtu;
	/**
	 * @network_time_offset_dtu: Last network time offset calculated
	 * (difference between coordinator and device time).
	 */
	uint32_t network_time_offset_dtu;
	/**
	 * @n_block_failed: Number of consecutive failed block.
	 */
	int n_block_failed;
	/**
	 * @stop_request: Region has been requested to stop.
	 */
	bool stop_request;
	/**
	 * @block_skip: Block skip information.
	 */
	struct lld_block_skip block_skip;
	/**
	 * @block_struct_change: Block structure change information.
	 */
	struct lld_block_struct_change block_struct_change;
	/**
	 * @next_block_struct_change_index: Block index of the next block
	 * structure change.
	 */
	uint32_t next_block_struct_change_index;
	/**
	 * @wait_block_struct_change: Region is waiting for an announced block
	 * structure change.
	 */
	bool wait_block_struct_change;
	/**
	 * @inactive_info: Sending info for inactive exponent.
	 */
	struct lldd_inactive_sent_info inactive_info;
	/**
	 * @last_dl_block_index: Block index of last received DL frame.
	 */
	uint32_t last_dl_block_index;
	/**
	 * @deferred: Deferred context.
	 */
	struct lldd_deferred deferred;
	/**
	 * @report_state_changed_state_id: Id of the state to report.
	 */
	enum lldd_state_id report_state_changed_state_id;
	/**
	 * @ranging: True when ranging is activated in the current block.
	 */
	bool ranging;
	/**
	 * @ranging_init_rctu: Timestamp of the ranging initial frame for the
	 * current block.
	 */
	uint64_t ranging_init_rctu;
	/**
	 * @ranging_measurement: Ranging measurement information.
	 */
	struct lld_ranging_measurement ranging_measurement;
	/**
	 * @report_state_changed_work: Transmit work to schedule state changed
	 * report.
	 */
	struct work_struct report_state_changed_work;
	/**
	 * @report_network_detected: Structure containing info for network
	 * detected report.
	 */
	struct {
		/**
		 * @skb: Beacon frame payload.
		 */
		struct sk_buff *skb;
		/**
		 * @beacon_info: Information in a received beacon frame.
		 */
		struct lldd_frame_beacon_info beacon_info;
		/**
		 * @lqi: Link quality indicator.
		 */
		uint8_t lqi;
	} report_network_detected;
	/**
	 * @tx_non_assoc_skb_queue: When unassociated, queue of skb for
	 * transmission.
	 */
	struct sk_buff_head tx_non_assoc_skb_queue;
	/**
	 * @tx_non_assoc_seq: Sequence number to use for the next enqueued
	 * buffer in tx non associated queue.
	 */
	int tx_non_assoc_seq;
	/**
	 * @report_network_detected_work: Transmit work to schedule network
	 * detected report.
	 */
	struct work_struct report_network_detected_work;
	/**
	 * @rx_frame_payload_queue: Queue of frame payloads to be processed.
	 */
	struct sk_buff_head rx_frame_payload_queue;
	/**
	 * @rx_frame_payload_work: Transmit work to schedule processing of
	 * received frame payload.
	 */
	struct work_struct rx_frame_payload_work;
	/**
	 * @report_association_failure_work: Transmit work to schedule
	 * processing of association failure report.
	 */
	struct work_struct report_association_failure_work;
	/**
	 * @association_request_work: Structure containing info for association
	 * request.
	 */
	struct work_struct association_request_work;
	/**
	 * @xmit_skb_queue: Queue of buffers to prepare for transmission.
	 */
	struct sk_buff_head xmit_skb_queue;
	/**
	 * @xmit_skb_work: Transmit work to schedule preparation of buffers to
	 * transmit.
	 */
	struct work_struct xmit_skb_work;
	/**
	 * @association_request: Association request context.
	 */
	struct lldd_assoc_req_info association_request;
	/**
	 * @report_association_failure_reason: Association failure reason.
	 */
	enum lldd_association_failure_reason report_association_failure_reason;
	/**
	 * @next_step_index: Next step index.
	 */
	uint16_t next_step_index;
};

static inline struct lldd_local *region_to_local(struct mcps802154_region *region)
{
	return qparent_of(region, struct lldd_local, region);
}

static inline struct lldd_local *access_to_local(struct mcps802154_access *access)
{
	return qparent_of(access, struct lldd_local, access);
}

/**
 * lldd_resync() - Resynchronise device with info received in DL message.
 * @local: Local context.
 * @n_ul_slots: Number of UL slots in DL message.
 * @block_index: Block index of DL message.
 * @timestamp_dtu: Timestamp of DL message.
 */
void lldd_resync(struct lldd_local *local, int n_ul_slots, uint32_t block_index,
		 uint32_t timestamp_dtu);

/**
 * lldd_report_state_changed() - Report a state change.
 * @local: Local context.
 * @state_id: Identifier of the new state.
 */
void lldd_report_state_changed(struct lldd_local *local, enum lldd_state_id state_id);

/**
 * lldd_report_network_detected() - Report the detection of a network.
 * @local: Local context.
 * @beacon_info: Beacon information.
 * @skb: Beacon payload.
 * @lqi: Link quality indicator (LQI, 0-100).
 *
 * Return: false to stop scanning (nobody is listening).
 */
bool lldd_report_network_detected(struct lldd_local *local,
				  const struct lldd_frame_beacon_info *beacon_info,
				  struct sk_buff *skb, uint8_t lqi);

/**
 * lldd_report_association_failure() - Report an association failure.
 * @local: Local context.
 * @status: Association response status.
 */
void lldd_report_association_failure(struct lldd_local *local,
				     enum lld_association_response_status status);

/**
 * lldd_get_dl_start_dtu() - Get position of DL frame inside a sub-block.
 * @local: Local context.
 *
 * Return: Offset between the sub-block start and the DL frame.
 */
static inline int lldd_get_dl_start_dtu(const struct lldd_local *local)
{
	return lld_get_dl_start_dtu(local->n_ul_slots, &local->network_params);
}

/**
 * lldd_get_subblock_duration_dtu() - Get sub-block duration.
 * @local: Local context.
 *
 * Return: The sub-block duration in dtu.
 */
static inline int lldd_get_subblock_duration_dtu(struct lldd_local *local)
{
	return lld_get_subblock_duration_dtu(local->n_ul_slots, &local->network_params);
}
