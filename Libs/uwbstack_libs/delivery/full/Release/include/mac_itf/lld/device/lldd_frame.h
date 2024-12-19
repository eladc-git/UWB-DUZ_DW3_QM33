/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "lld/lld_common.h"
#include "lld/lld_frame.h"

#include <stdbool.h>
#include <stdint.h>

struct lldd_local;
struct sk_buff;

/**
 * struct lldd_frame_dl_info - Information in a received DL frame.
 */
struct lldd_frame_dl_info {
	/**
	 * @block_index: Received block index.
	 */
	uint32_t block_index;
	/**
	 * @timestamp_dtu: Synchronization timestamp.
	 */
	uint32_t timestamp_dtu;
	/**
	 * @n_ul_slots: Received number of UL slots.
	 */
	int n_ul_slots;
	/**
	 * @ack: True if our UL slot was acknowledged.
	 */
	bool ack;
	/**
	 * @aux_subblock: True if the DL frame is in a auxiliary sub-block.
	 */
	bool aux_subblock;
	/**
	 * @next_frame_type: Next frame type.
	 */
	enum lld_next_frame_type next_frame_type;
	/**
	 * @next_frame_repetition: Next frame repetition.
	 */
	int next_frame_repetition;
	/**
	 * @next_frame_ifs_dtu: Next frame IFS.
	 */
	int next_frame_ifs_dtu;
	/**
	 * @next_sb_combined_with_cap: True when next sub-block is a combined
	 * sub-block and CAP.
	 */
	bool next_sb_combined_with_cap;
	/**
	 * @next_sb_ds_twr_final: True when next sub-block UL frames are used
	 * as final frame for DS-TWR ranging.
	 */
	bool next_sb_ds_twr_final;
	/**
	 * @block_skip: Block skip information.
	 */
	struct lld_block_skip block_skip;
	/**
	 * @non_associated_acknowledged: Non associated acknowledge with
	 * matching extended address and sequence number.
	 */
	bool non_associated_acknowledged;
	/**
	 * @duration_dtu: Duration of the received frame.
	 */
	int duration_dtu;
	/**
	 * @block_struct_change: Block structure change information.
	 */
	struct lld_block_struct_change block_struct_change;
	/**
	 * @ranging_control: Ranging control information.
	 */
	struct lld_ranging_control ranging_control;
};

/**
 * struct lldd_frame_beacon_info - Information in a received beacon frame.
 */
struct lldd_frame_beacon_info {
	/**
	 * @short_addr: Sender short address.
	 */
	uint16_t short_addr;
	/**
	 * @pan_id: Sender PAN ID.
	 */
	uint16_t pan_id;
	/**
	 * @block_index: Received block index.
	 */
	uint32_t block_index;
	/**
	 * @distance_dtu: Distance to main DL frame.
	 */
	int distance_dtu;
	/**
	 * @n_cap_slots: Number of CAP slots.
	 */
	int n_cap_slots;
	/**
	 * @pairing_allowed: Whether pairing is allowed.
	 */
	bool pairing_allowed;
	/**
	 * @cap_slot_ifs_dtu: IFS using slot-based mode between the beacon frame
	 * and the first CAP slot.
	 */
	int cap_slot_ifs_dtu;
	/**
	 * @network_params: Received network parameters.
	 */
	struct lld_network_params network_params;
};

/**
 * struct lldd_frame_assoc_req_buff - Association request frame context.
 */
struct lldd_frame_assoc_req_buff {
	/**
	 * @dst_short_addr: Destination short address.
	 */
	uint16_t dst_short_addr;
	/**
	 * @seq_num: Sequence number.
	 */
	int seq_num;
};

/**
 * struct lldd_frame_association_resp_info - Information in a received
 * association response.
 */
struct lldd_frame_association_resp_info {
	/**
	 * @short_addr: Short address affected to the device.
	 */
	uint16_t short_addr;
	/**
	 * @association_status: Association status.
	 */
	enum lld_association_response_status association_status;
};

/**
 * lldd_frame_ul_header_put() - Fill a UL frame header.
 * @local: LLD context.
 * @skb: Frame buffer.
 * @data_payload: True if a payload will be appended to the frame.
 *
 * Also append payload dispatch byte if payload will be appended.
 *
 * Return: header length or negative error.
 */
int lldd_frame_ul_header_put(const struct lldd_local *local, struct sk_buff *skb,
			     bool data_payload);

/**
 * lldd_frame_dl_header_check() - Check a DL frame header and report content.
 * @local: LLD context.
 * @skb: Frame buffer.
 * @info: Filled with received information.
 *
 * Also check and consume payload dispatch byte if present.
 *
 * Return: true if header is correct.
 */
bool lldd_frame_dl_header_check(const struct lldd_local *local, struct sk_buff *skb,
				struct lldd_frame_dl_info *info);

/**
 * lldd_frame_beacon_header_check() - Check a beacon frame header and report
 * content.
 * @local: LLD context.
 * @skb: Frame buffer.
 * @info: Filled with received information.
 *
 * Return: true if header is correct.
 */
bool lldd_frame_beacon_header_check(const struct lldd_local *local, struct sk_buff *skb,
				    struct lldd_frame_beacon_info *info);

/**
 * lldd_frame_association_req_put() - Fill an association request.
 * @local: LLD context.
 * @skb: Frame buffer.
 * @buff: frame context to use.
 *
 * Return: header length or negative errno.
 */
int lldd_frame_association_req_put(const struct lldd_local *local, struct sk_buff *skb,
				   const struct lldd_frame_assoc_req_buff *buff);

/**
 * lldd_frame_association_response_check() - Check an association response and
 * report content.
 * @local: LLD context.
 * @skb: Frame buffer.
 * @info: Filled with received information.
 *
 * Return: true if frame is correct.
 */
bool lldd_frame_association_response_check(const struct lldd_local *local, struct sk_buff *skb,
					   struct lldd_frame_association_resp_info *info);
