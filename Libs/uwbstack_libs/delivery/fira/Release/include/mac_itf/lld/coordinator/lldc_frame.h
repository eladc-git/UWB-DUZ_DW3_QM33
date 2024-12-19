/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "lld/lld_common.h"
#include "lld/lld_frame.h"
#include "lld/lld_mfs.h"
#include "net/lld_region_common_nl.h"

#include <stdbool.h>
#include <stdint.h>

struct lldc_local;
struct sk_buff;

/**
 * struct lldc_frame_ul_info - Information in a received UL frame.
 */
struct lldc_frame_ul_info {
	/**
	 * @mfs_init: For each MFS, whether the MFS is in the INIT state.
	 */
	bool mfs_init[LLD_MFS_ID_NB];
	/**
	 * @mfs_next_expected_seq: For each MFS, if not in INIT state, the next
	 * expected sequence number.
	 */
	uint8_t mfs_next_expected_seq[LLD_MFS_ID_NB];
	/**
	 * @inactive_exponent: Device advertised its inactive state, will
	 * participate only in blocks with an index which is a multiple of 2 to
	 * the power of inactive_exponent.
	 */
	int inactive_exponent;
};

/**
 * struct lldc_frame_dl_buff - DL frame context.
 */
struct lldc_frame_dl_buff {
	/**
	 * @next_frame_type: Next frame type.
	 */
	enum lld_next_frame_type next_frame_type;
	/**
	 * @next_frame: Address of next frame fields inside the DL frame.
	 */
	uint8_t *next_frame;
	/**
	 * @next_frame_ifs: Next frame IFS.
	 */
	const struct lld_ifs *next_frame_ifs;
	/**
	 * @next_frame_repetition: Next frame repetition.
	 */
	int next_frame_repetition;
	/**
	 * @next_sb_combined_with_cap: True when new sub-block is a combined
	 * sub-block and CAP.
	 */
	bool next_sb_combined_with_cap;
};

/**
 * struct lldc_assoc_req_info - Association request context.
 */
struct lldc_assoc_req_info {
	/**
	 * @extend_addr: Device extended address.
	 */
	uint64_t extend_addr;
	/**
	 * @seq_num: Sequence number.
	 */
	uint8_t seq_num;
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
};

/**
 * struct lldc_frame_ul_payload_info - Payload Information in a received UL
 * frame.
 */
struct lldc_frame_ul_payload_info {
	/**
	 * @ranging_measurement: Ranging measurement information.
	 */
	struct lld_ranging_measurement ranging_measurement;
};

/**
 * lldc_frame_dl_put_begin() - Start to fill a DL frame header.
 * @local: LLD context.
 * @dl_buff: DL frame context.
 * @skb: Frame buffer.
 * @aux: True if this is an auxiliary sub-block.
 * @data_payload: True if a payload will be appended to the frame.
 * @sync_timestamp_dtu: Synchronization timestamp in DTU.
 *
 * Also append payload dispatch byte if payload will be appended.
 *
 * Return: header length or negative error.
 */
int lldc_frame_dl_put_begin(const struct lldc_local *local, struct lldc_frame_dl_buff *dl_buff,
			    struct sk_buff *skb, bool aux, bool data_payload,
			    uint32_t sync_timestamp_dtu);

/**
 * lldc_frame_dl_put_end() - End to fill a DL frame header.
 * @local: LLD context.
 * @dl_buff: DL frame context.
 * @dl_duration_dtu: Duration of DL frame.
 */
void lldc_frame_dl_put_end(const struct lldc_local *local, struct lldc_frame_dl_buff *dl_buff,
			   int dl_duration_dtu);

/**
 * lldc_frame_beacon_put() - Fill a beacon frame.
 * @local: LLD context.
 * @skb: Frame buffer.
 * @distance_dtu: Distance to main DL frame.
 *
 * Return: QERR_SUCCESS or negative error.
 */
int lldc_frame_beacon_put(const struct lldc_local *local, struct sk_buff *skb, int distance_dtu);

/**
 * lldc_frame_ul_header_check() - Check a UL frame header and report content.
 * @local: LLD context.
 * @skb: Frame buffer.
 * @short_addr: Expected device address.
 * @info: Filled with received information.
 *
 * Also check and consume payload dispatch byte if present.
 *
 * Return: true if header is correct.
 */
bool lldc_frame_ul_header_check(const struct lldc_local *local, struct sk_buff *skb,
				uint16_t short_addr, struct lldc_frame_ul_info *info);

/**
 * lldc_frame_association_request_check() - Check an association frame.
 * @local: LLD context.
 * @skb: Frame buffer.
 * @info: Association request decoded.
 *
 * Return: True if frame is an association request.
 */
bool lldc_frame_association_request_check(const struct lldc_local *local, struct sk_buff *skb,
					  struct lldc_assoc_req_info *info);

/**
 * lldc_frame_association_response_put() - Fill an association response frame.
 * @local: LLD context.
 * @skb: Frame buffer.
 * @extended_addr: Extended destination address.
 * @short_addr: Short destination address.
 * @status: Association response status.
 *
 * Return: frame length or negative error.
 */
int lldc_frame_association_response_put(const struct lldc_local *local, struct sk_buff *skb,
					uint64_t extended_addr, uint16_t short_addr,
					enum lld_association_response_status status);

/**
 * lldc_frame_payload_ie_read() - Check and read payload IEs.
 * @skb: UL frame.
 * @info: UL frame payload information.
 *
 * Return: True if no error was found.
 */
bool lldc_frame_payload_ie_read(struct sk_buff *skb, struct lldc_frame_ul_payload_info *info);
