/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fira/fira_frame_defines.h"

#include <stdbool.h>
#include <stdint.h>

#define FIRA_IE_PAYLOAD_DATA_MESSAGE_LEN(payload_len) (FIRA_IE_VENDOR_OUI_LEN + 3 + payload_len)

#define FIRA_DATA_MESSAGE_CONTENT_TYPE_PAYLOAD 0
#define FIRA_DATA_MESSAGE_UWB_MSG_ID (0x0f << 0)
#define FIRA_DATA_MESSAGE_CONTENT_TYPE (0x0f << 4)

struct sk_buff;
struct qlist_head;
struct fira_data_payload_segment;

/**
 * fira_frame_payload_ie_put_begin() - Start an FiRa Payload IE.
 * @len: Frame length.
 * @skb: Frame buffer.
 *
 * Return: Pointer to first "free" byte in frame buffer.
 */
uint8_t *fira_frame_payload_ie_put_begin(uint16_t len, struct sk_buff *skb);

/**
 * fira_frame_data_message_put() - Add a data message Payload
 * IE. Packing of the data from discontinuous payload buffer represented by list
 * to frame buffer.
 * @skb: Frame buffer.
 * @payload_segments: List of data segments to be included in the Payload IE.
 * @offset: Data offset in relation to the beginning of the list.
 * @data_len: Data length to be copied.
 */
void fira_frame_data_message_put(struct sk_buff *skb, const struct qlist_head *payload_segments,
				 uint16_t offset, uint16_t data_len);

/**
 * fira_frame_data_message_check() - Check and read a data
 * message Payload IE.
 * @p: Pointer to IE message id.
 * @ie_len: Length of the IE content field.
 * @segment: Pointer to a buffer that will be used to read Payload IE data.
 *
 * Return: True if IE was successfully read.
 */
bool fira_frame_data_message_check(uint8_t *p, unsigned int ie_len,
				   struct fira_data_payload_segment **segment);
