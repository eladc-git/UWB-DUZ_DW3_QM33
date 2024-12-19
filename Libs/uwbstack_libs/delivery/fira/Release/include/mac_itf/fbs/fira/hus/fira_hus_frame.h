/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#include "fira_frame_defines.h"
#include "net/af_ieee802154.h"
#include "net/fira_region_params.h"
#include "net/mcps802154_frame.h"
#include "qerr.h"

#include <stdbool.h>
#include <stdint.h>
struct fira_session;
struct fira_slot;
struct sk_buff;

#define FIRA_HUS_FC_MULTIPURPOSE_FRAME_DEFAULT \
	IEEE802154_FC_MP_VALUE_LONG(0, IEEE802154_ADDR_SHORT, 0, 0, 1, 0, 0, 0, 0)

#define FIRA_RRML_ELEMENT_SIZE (10)

/*
 * The 4 bytes correspond to:
 * - 1 byte for Message ID, Stop Session and Version.
 * - 2 Bytes for Message Control.
 * - 1 byte for Stride Length.
 */
#define FIRA_IE_PAYLOAD_CONTROL_MESSAGE_TYPE3_LEN(round_index_present, n_phases) \
	(FIRA_IE_VENDOR_OUI_LEN + 4 + 2 * (round_index_present) +                \
	 FIRA_RRML_ELEMENT_SIZE * (n_phases))

#define FIRA_CONTROL_MESSAGE_TYPE3_MESSAGE_ID (0x0f << 0)
#define FIRA_CONTROL_MESSAGE_TYPE3_STOP_SESSION (1 << 4)
#define FIRA_CONTROL_MESSAGE_TYPE3_VERSION (0x07 << 5)

#define FIRA_CONTROL_MESSAGE_TYPE3_CONTROL_HOPPING_MODE (1 << 0)
#define FIRA_CONTROL_MESSAGE_TYPE3_CONTROL_RRML_SIZE (0x7fff << 1)

#define FIRA_RRML_PHASE_INDICATOR (1 << 0)
#define FIRA_RRML_DEVICE_MAC_ADDRESS_MODE (1 << 1)
#define FIRA_RRML_SLOT_INDEX_START (0x7fffull << 2)
#define FIRA_RRML_SLOT_INDEX_END (0x7fffull << 17)

/**
 * struct fira_hus_cm -- FiRa HUS Control Message (Type 3)
 * @phase_list: Pointer to the list of phases to include in the CM Type 3.
 * @next_round_index: Round index in next ranging block.
 * @message_id: Message ID.
 * @n_phases: Number of phases in phase list.
 * @block_stride_len: Number of blocks to be skipped.
 * @round_hopping: Round hopping enabled.
 * @stop_session: Indicates if the Controller has decided to stop or continue the session.
 */
struct fira_hus_cm {
	struct qlist_head *phase_list;
	uint16_t next_round_index;
	enum fira_message_id message_id;
	uint8_t n_phases;
	uint8_t block_stride_len;
	bool round_hopping;
	bool stop_session;
};

/**
 * fira_hus_frame_header_put() - Fill header for HUS frame.
 * @session: FiRa session context.
 * @slot: Corresponding slot.
 * @skb: Frame buffer.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_hus_frame_header_put(const struct fira_session *session,
				    const struct fira_slot *slot, struct sk_buff *skb);

/**
 * fira_hus_frame_header_check() - Check header of HUS frame
 * @session: FiRa session context.
 * @slot: Corresponding slot.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must be zero initialized.
 * @phy_sts_index: STS index read from header IE.
 */
bool fira_hus_frame_header_check(struct fira_session *session, const struct fira_slot *slot,
				 struct sk_buff *skb, struct mcps802154_ie *ie,
				 uint32_t *phy_sts_index);

/**
 * fira_hus_frame_control_message_payload_put() - Fill payload for HUS frame
 * @hus_frame: HUS frame description.
 * @skb: Frame buffer.
 */
void fira_hus_frame_control_message_payload_put(struct fira_hus_cm *hus_frame, struct sk_buff *skb);

/**
 * fira_hus_frame_control_payload_check() - Check payload of HUS frame
 * @slot: Corresponding slot.
 * @skb: Frame buffer.
 * @ie: Context used to read IE, must be zero initialized.
 * @cm_frame: Outgoing parameters.
 */
bool fira_hus_frame_control_payload_check(const struct fira_slot *slot, struct sk_buff *skb,
					  struct mcps802154_ie *ie, struct fira_hus_cm *cm_frame);
