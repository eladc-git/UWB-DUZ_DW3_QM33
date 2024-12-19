/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "linux/skbuff.h"
#include "linux/workqueue.h"
#include "lld/coordinator/lldc_stats.h"
#include "lld/lld_common.h"
#include "lld/lld_frame.h"
#include "lld/lld_mfs.h"
#include "net/lld_region_common_nl.h"
#include "net/mcps802154.h"
#include "net/mcps802154_schedule.h"
#include "qerr.h"
#include "qlist.h"
#include "qtils.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LLDC_FRAMES_MAX (LLD_DEVICES_MAX + 1)

/**
 * LLDC_SHORT_ADDR - Fixed coordinator short address.
 */
#define LLDC_SHORT_ADDR 0xfffc

struct lldc_assoc_req_info;
struct lldc_local;

/**
 * struct lldc_params - Parameters.
 */
struct lldc_params {
	/**
	 * @allow_pairing: Allow pairing with unknown devices, requests will be
	 * forwarded to higher layers.
	 */
	bool allow_pairing;
	/**
	 * @agg_limit_bytes: Limit MPDU aggregated payload size after the first
	 * sub-frame.
	 */
	int agg_limit_bytes;
	/**
	 * @beacon_ifs: Interval between the last sub-block DL frame and the
	 * beacon frame.
	 */
	struct lld_ifs beacon_ifs;
	/**
	 * @beacon_interval_dtu: Interval between two beacons transmissions
	 * when the coordinator is in ACTIVE state.
	 */
	int beacon_interval_dtu;
	/**
	 * @beacon_payload: Payload embedded in transmitted beacons.
	 */
	uint8_t beacon_payload[LLD_BEACON_PAYLOAD_LEN_MAX];
	/**
	 * @beacon_payload_len: Length of beacon payload.
	 */
	size_t beacon_payload_len;
	/**
	 * @cap_interval_max_dtu: Maximum interval between two CAP when the
	 * coordinator is in ACTIVE state.
	 */
	int cap_interval_max_dtu;
	/**
	 * @cap_slots_per_second: Average number of contention slots per
	 * second.
	 */
	int cap_slots_per_second;
	/**
	 * @device_loss_threshold: Number of consecutive failed blocks for a
	 * coordinator to declare a device loss.
	 */
	int device_loss_threshold;
	/**
	 * @non_assoc_ifs: Interval between the last sub-block DL frame and the
	 * first frame of non-associated exchange.
	 */
	struct lld_ifs non_assoc_ifs;
	/**
	 * @probing_beacon_as_dl: True to use a beacon frame to fill the role
	 * of both DL frame and beacon frame when the coordinator is in the
	 * PROBING state.
	 */
	bool probing_beacon_as_dl;
	/**
	 * @probing_cap_slots: Override CAP_SLOTS_PER_SECOND when the
	 * coordinator is in PROBING state.
	 */
	int probing_cap_slots;
	/**
	 * @probing_interval_dtu: Interval between two beacons transmissions
	 * when the coordinator is in PROBING state. The same interval is used
	 * between two CAP.
	 */
	int probing_interval_dtu;
	/**
	 * @sb_beacon_cap_slot_ifs: Interval between the beacon frame acting as
	 * a DL frame and the first additional CAP slot of a combined main
	 * sub-block and CAP.
	 */
	struct lld_ifs sb_beacon_cap_slot_ifs;
	/**
	 * @sb_dl_cap_slot_ifs: Interval between the last sub-block DL frame and
	 * the first additional CAP slot of a combined sub-block and CAP.
	 */
	struct lld_ifs sb_dl_cap_slot_ifs;
	/**
	 * @smart_power: True to enable smart power feature, false otherwise.
	 */
	bool smart_power;
	/**
	 * @mfs_params: MFS parameters.
	 */
	struct lld_mfs_params mfs_params[LLD_MFS_ID_NB];
};

/**
 * struct lldc_ranging_reply - Ranging Treply information.
 */
struct lldc_ranging_reply {
	/**
	 * @time_rctu: Ranging Treply for the current or the last ranging block.
	 */
	int time_rctu;
	/**
	 * @present: TRUE if ranging Treply is present.
	 */
	bool present;
};

/**
 * struct lldc_ranging_procedure - Ranging procedure information.
 */
struct lldc_ranging_procedure {
	/**
	 * @reply_current: Ranging Treply for the current ranging block.
	 */
	struct lldc_ranging_reply reply_current;
	/**
	 * @reply_last: Ranging Treply for the last ranging block.
	 */
	struct lldc_ranging_reply reply_last;
	/**
	 * @init_rctu: Ranging initial frame timestamp.
	 */
	uint64_t init_rctu;
	/**
	 * @init_present: TRUE if ranging initial frame timestamp is present.
	 */
	bool init_present;
};

/**
 * struct lldc_associated_device - Associated device.
 */
struct lldc_associated_device {
	/**
	 * @dev_key: Device identity key.
	 */
	uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN];
	/**
	 * @short_addr: Short address of the associated device.
	 */
	uint16_t short_addr;
	/**
	 * @error: Error code for the UL frame of this device.
	 */
	enum mcps802154_rx_error_type error;
	/**
	 * @stats: Device statistics.
	 */
	struct lldc_stats stats;
	/**
	 * @inactive_exponent: Device advertised its inactive state, will
	 * participate only in blocks with an index which is a multiple of 2 to
	 * the power of inactive_exponent.
	 */
	int inactive_exponent;
	/**
	 * @n_failed_blocks: Number of consecutives failed blocks for this
	 * device.
	 */
	int n_failed_blocks;
	/**
	 * @ranging_capability_flags: Device ranging capability flags.
	 */
	enum lld_ranging_capability_flags ranging_capability_flags;
	/**
	 * @ranging_procedure: Ranging procedure information.
	 */
	struct lldc_ranging_procedure ranging_procedure;
};

/**
 * enum lldc_subaccess_type - Sub-access type, piece of access.
 */
enum lldc_subaccess_type {
	/**
	 * @LLDC_SUBACCESS_TYPE_MAIN: Main sub-block.
	 */
	LLDC_SUBACCESS_TYPE_MAIN,
	/**
	 * @LLDC_SUBACCESS_TYPE_ADDITIONAL_CAP: Additional CAP in Sub-block with
	 * combined CAP.
	 */
	LLDC_SUBACCESS_TYPE_ADDITIONAL_CAP,
	/**
	 * @LLDC_SUBACCESS_TYPE_AUX: Auxiliary sub-block.
	 */
	LLDC_SUBACCESS_TYPE_AUX,
	/**
	 * @LLDC_SUBACCESS_TYPE_DATA_BURST: Data burst frames.
	 */
	LLDC_SUBACCESS_TYPE_DATA_BURST,
	/**
	 * @LLDC_SUBACCESS_TYPE_NON_ASSOCIATED: Send a non-associated frame.
	 */
	LLDC_SUBACCESS_TYPE_NON_ASSOCIATED,
	/**
	 * @LLDC_SUBACCESS_TYPE_BEACON: Send a beacon frame.
	 */
	LLDC_SUBACCESS_TYPE_BEACON,
	/**
	 * @LLDC_SUBACCESS_TYPE_NOTHING: No sub-access claim.
	 */
	LLDC_SUBACCESS_TYPE_NOTHING,
};

/**
 * struct lldc_association_request - association request.
 */
struct lldc_association_request {
	/**
	 * @extended_addr: Extended address of the association request.
	 */
	uint64_t extended_addr;
	/**
	 * @dev_key: Device identity key.
	 */
	uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN];
	/**
	 * @request_time_ms: Request time.
	 */
	uint64_t request_time_ms;
	/**
	 * @entry: Entry in list of association requests.
	 */
	struct qlist_head entry;
};

/**
 * struct lldc_association_response - association response.
 */
struct lldc_association_response {
	/**
	 * @extended_addr: Extended address of the association response.
	 */
	uint64_t extended_addr;
	/**
	 * @status: Association response status.
	 */
	enum lld_association_response_status status;
	/**
	 * @short_addr: Short address or LLD_SHORT_ADDR_UNDEFINED.
	 */
	uint16_t short_addr;
	/**
	 * @entry: Entry in list of association responses.
	 */
	struct qlist_head entry;
};

/**
 * struct lldc_paired_device - paired device.
 */
struct lldc_paired_device {
	/**
	 * @dev_key: Device identity key.
	 */
	uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN];
	/**
	 * @entry: Entry in list of paired devices.
	 */
	struct qlist_head entry;
};

/**
 * struct lldc_deferred - Deferred context.
 */
struct lldc_deferred {
	/**
	 * @cb: Deferred callback.
	 */
	void (*cb)(struct lldc_local *);
	/**
	 * @skb: Buffer to handle.
	 */
	struct sk_buff *skb;
	/**
	 * @skb_len: Original buffer length.
	 */
	int skb_len;
	/**
	 * @device_index: Device index of the received frame.
	 */
	int device_index;
	/**
	 * @lqi: LQI of received frame.
	 */
	uint8_t lqi;
};

/**
 * struct lldc_ranging_element - Ranging report information element.
 */
struct lldc_ranging_element {
	/**
	 * @short_addr: Address of the participating device.
	 */
	uint16_t short_addr;
	/**
	 * @tof_rctu: Computed Time of Flight.
	 */
	int tof_rctu;
	/**
	 * @tof: TRUE if Time of Flight information is present.
	 */
	bool tof;
	/**
	 * @ranging_measurement: Ranging measurement information.
	 */
	struct lld_ranging_measurement ranging_measurement;
};

/**
 * struct lldc_ranging_report - Ranging information.
 */
struct lldc_ranging_report {
	/**
	 * @n_elements: Number of ranging elements.
	 */
	int n_elements;
	/**
	 * @n_elements_max: Maximum number of ranging elements.
	 */
	int n_elements_max;
	/**
	 * @ranging_element: Array of ranging elements.
	 */
	struct lldc_ranging_element *ranging_element;
};

/**
 * struct lldc_local - Local context.
 */
struct lldc_local {
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
	 * @device_index: Conversion table from frame index to device index.
	 */
	int device_index[LLD_DEVICES_MAX];
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
	 * @state: State of the coordinator.
	 */
	const struct lldc_fsm_state *state;
	/**
	 * @params: Parameters.
	 */
	struct lldc_params params;
	/**
	 * @network_params: Network parameters.
	 */
	struct lld_network_params network_params;
	/**
	 * @associated_devices: Array of associated devices.
	 */
	struct lldc_associated_device associated_devices[LLD_DEVICES_MAX];
	/**
	 * @association_requests: List of pending association requests.
	 */
	struct qlist_head association_requests;
	/**
	 * @association_responses: List of association responses.
	 */
	struct qlist_head association_responses;
	/**
	 * @paired_devices: List of paired devices.
	 */
	struct qlist_head paired_devices;
	/**
	 * @coord_stats: Global coordinator statistics.
	 */
	struct lldc_coord_stats coord_stats;
	/**
	 * @reset_stats_request: True when a reset of the stats is requested.
	 */
	bool reset_stats_request;
	/**
	 * @mfs_tx_mcast: Tx mcast MAC Frame Streams.
	 */
	struct lld_mfs_tx_mcast mfs_tx_mcast[LLD_MFS_ID_NB];
	/**
	 * @mfs_rx: Rx MAC Frame Streams.
	 */
	struct lld_mfs_rx mfs_rx[LLD_MFS_ID_NB][LLD_DEVICES_MAX];
	/**
	 * @extended_addr: Extended address.
	 */
	uint64_t extended_addr;
	/**
	 * @event_portid: Port identifier to use for notifications.
	 */
	uint32_t event_portid;
	/**
	 * @pan_id: PAN ID, or 0 if unset.
	 */
	uint16_t pan_id;
	/**
	 * @n_ul_slots: Number of UL slots.
	 */
	int n_ul_slots;
	/**
	 * @n_devices: Number of associated devices.
	 */
	int n_devices;
	/**
	 * @block_struct_change_request: True when a block structure change is
	 * requested.
	 */
	bool block_struct_change_request;
	/**
	 * @block_start_valid: True when block_start_dtu can be used by
	 * get_access.
	 */
	bool block_start_valid;
	/**
	 * @ranging: True when ranging is activated in the current block.
	 */
	bool ranging;
	/**
	 * @block_start_dtu: Timestamp of the current block.
	 *
	 * The lldc_update() function updates the "current" block, sub-block,
	 * etc. The "current" term refers to the access being done or the next
	 * one do to. The lldc_update() makes sure that the "current" setup
	 * refer to an access in the future.
	 */
	uint32_t block_start_dtu;
	/**
	 * @block_index: Current block index.
	 */
	uint32_t block_index;
	/**
	 * @subblock_index: Current sub-block index.
	 */
	int subblock_index;
	/**
	 * @subaccess_timestamp_dtu: Current sub-access timestamp, in DTU.
	 */
	uint32_t subaccess_timestamp_dtu;
	/**
	 * @ack: Single bit acknowledgement for current sub-block.
	 */
	uint32_t ack;
	/**
	 * @cap: Local cap information.
	 */
	struct {
		/**
		 * @repetition: Number of cap repetitions on combined sub-block
		 * and CAP.
		 */
		int repetition;
		/**
		 * @slot_remaining: Number of cap slots remaining in CAP
		 * subaccess.
		 */
		int slot_remaining;
		/**
		 * @dtu_per_cap_interval: Interval matching the cap slots
		 * frequency.
		 */
		int dtu_per_cap_interval;
		/**
		 * @next_sb_combined_with_cap: True when next sub-block is
		 * combined with CAP.
		 */
		bool next_sb_combined_with_cap;
		/**
		 * @timestamp_dtu: Timestamp of the last cap slots done.
		 */
		uint32_t timestamp_dtu;
		/**
		 * @non_assoc_ack_ie: All non associated acknowledged IE.
		 */
		struct lld_non_assoc_ack_ie non_assoc_ack_ie;
	} cap;
	/**
	 * @beacon_next_dtu: Timestamp at which the next beacon transmission is
	 * due.
	 */
	uint32_t beacon_next_dtu;
	/**
	 * @next_subaccess_type: Next sub-access type.
	 */
	enum lldc_subaccess_type next_subaccess_type;
	/**
	 * @next_subaccess_timestamp_dtu: Next sub-access timestamp, in DTU.
	 */
	uint32_t next_subaccess_timestamp_dtu;
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
	 * @deferred: Deferred context.
	 */
	struct lldc_deferred deferred;
	/**
	 * @association_short_addr: Short address to use for the next associated
	 * device.
	 */
	uint16_t association_short_addr;
	/**
	 * @rx_association_request: Structure containing info for association
	 * request management.
	 */
	struct {
		/**
		 * @processing: True if the region is processing an association
		 * request.
		 */
		bool processing;
		/**
		 * @extended_addr: Extended address of association response.
		 */
		uint64_t extended_addr;
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
		/**
		 * @ranging_capability_flags: Device ranging capability flags.
		 */
		enum lld_ranging_capability_flags ranging_capability_flags;
	} rx_association_request;
	/**
	 * @rx_association_request_work: Transmit work to schedule device
	 * association state changed report.
	 */
	struct work_struct rx_association_request_work;
	/**
	 * @report_ranging_work: Transmit work to schedule processing of ranging
	 * report.
	 */
	struct work_struct report_ranging_work;
	/**
	 * @ranging_data: Structure containing ongoing aggregation of ranging
	 * report info.
	 */
	struct lldc_ranging_report ranging_data;
	/**
	 * @ranging_report: Structure containing aggregated ranging report info.
	 */
	struct lldc_ranging_report ranging_report;
	/**
	 * @ranging_response_rctu: Ranging response frame timestamp information.
	 */
	uint64_t ranging_response_rctu;
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
	 * @xmit_skb_queue: Queue of buffers to prepare for transmission.
	 */
	struct sk_buff_head xmit_skb_queue;
	/**
	 * @xmit_skb_work: Transmit work to schedule preparation of buffers to
	 * transmit.
	 */
	struct work_struct xmit_skb_work;
	/**
	 * @next_step_index: Next step index.
	 */
	uint16_t next_step_index;
};

static inline struct lldc_local *region_to_local(struct mcps802154_region *region)
{
	return qparent_of(region, struct lldc_local, region);
}

static inline struct lldc_local *access_to_local(struct mcps802154_access *access)
{
	return qparent_of(access, struct lldc_local, access);
}

/**
 * lldc_get_dl_start_dtu() - Get position of DL frame inside a sub-block.
 * @local: Local context.
 *
 * Return: Offset between the sub-block start and the DL frame.
 */
static inline int lldc_get_dl_start_dtu(const struct lldc_local *local)
{
	return lld_get_dl_start_dtu(local->n_ul_slots, &local->network_params);
}

/**
 * lldc_get_subblock_duration_dtu() - Get sub-block duration.
 * @local: Local context.
 *
 * Return: The sub-block duration in dtu.
 */
static inline int lldc_get_subblock_duration_dtu(const struct lldc_local *local)
{
	return lld_get_subblock_duration_dtu(local->n_ul_slots, &local->network_params);
}

/**
 * lldc_add_association_response() - Add element to list of association
 * responses.
 * @local: Local context.
 * @extended_addr: Extended address of association response.
 * @short_addr: Short address of association response.
 * @status: Association response status.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lldc_add_association_response(struct lldc_local *local, uint64_t extended_addr,
					uint16_t short_addr,
					enum lld_association_response_status status);

/**
 * lldc_associate_device() - Try to add device to list of associated devices,
 * add appropriate association response and send state changed notification.
 * @local: Local context.
 * @dev_key: Device identity key.
 * @extended_addr: Extended address of device.
 */
void lldc_associate_device(struct lldc_local *local,
			   uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN],
			   uint64_t extended_addr);

/**
 * lldc_disassociate_check() - Check if devices must be disassociated.
 * @local: Local context.
 */
void lldc_disassociate_check(struct lldc_local *local);

/**
 * lldc_skip_check() - Check if blocks must be skipped.
 * @local: Local context.
 */
void lldc_skip_check(struct lldc_local *local);

/**
 * lldc_report_dev_assoc_state_changed() - Report a device association state
 * change.
 * @local: Local context.
 * @dev_key: Device identity key.
 * @associated: True if the device is associated.
 * @short_addr: Short address of the device, if associated.
 * @flags: Device ranging capability flags.
 */
void lldc_report_dev_assoc_state_changed(struct lldc_local *local,
					 const uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN],
					 bool associated, uint16_t short_addr,
					 enum lld_ranging_capability_flags flags);

/**
 * lldc_report_pairing_request() - Report a pairing request.
 * @local: Local context.
 * @dev_key: Device identity key.
 * @association_payload: Association payload.
 * @association_payload_len: Association payload length.
 */
void lldc_report_pairing_request(struct lldc_local *local,
				 const uint8_t dev_key[LLD_DEVICE_IDENTITY_PUBLIC_KEY_LEN],
				 const uint8_t *association_payload, int association_payload_len);

/**
 * lldc_report_ranging() - Report a ranging result.
 * @local: Local context.
 * @ranging_data: Ranging report information.
 */
void lldc_report_ranging(struct lldc_local *local, struct lldc_ranging_report *ranging_data);

/**
 * lldc_rx_association_request() - Handle association request.
 * @local: Local context.
 * @info: Association request decoded.
 */
void lldc_rx_association_request(struct lldc_local *local, struct lldc_assoc_req_info *info);
