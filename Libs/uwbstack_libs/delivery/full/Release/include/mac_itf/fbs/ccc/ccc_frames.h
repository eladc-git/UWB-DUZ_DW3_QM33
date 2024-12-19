/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

#ifndef __GNUC__
#pragma anon_unions
#endif

/* Car Connectivity Consortium (CCC) OUI: 04-df-69 (hex)
 * Reference: http://standards-oui.ieee.org/oui.txt
 */
#define CCC_VENDOR_OUI 0x04df69
/* ACWG/Aliro Vendor OUI: 0x4a191b (Section 12.1.5 of Aliro spec). */
#define ACWG_VENDOR_OUI 0x4a191b

/* CCC MAX Header of SP0 Packets Pre-Poll and Final_DATA
 * as described in 4.5.4.1 of CCC-WP-xxx-Digital-Key-R3_0.0.1
 */
struct ccc_aux_sec_hdr {
	uint8_t sec_ctrl;
/* Bit 0-2: Security level for authentication of MAC header and data payload */
#define ENC_MIC_64 6
/* Bit 3-4: Key is determined explicitly from the 4-octet Key Source field and Key Identifier
 *          field.
 */
#define KEY_ID_MODE 2
/* Bit 5  : 0x0, enable frame counter */
#define FRM_CNT_SUP 0
/* Bit 6  : ASN not in nonce */
#define ASN_NOT_IN_NONCE 0
/* Bit 7 */
#define RESRVED 0
#define CCC_SEC_CTRL                                                                      \
	(ENC_MIC_64 | (KEY_ID_MODE << 3) | (FRM_CNT_SUP << 5) | (ASN_NOT_IN_NONCE << 6) | \
	 (RESRVED << 7))
	/* Counter that increments for each transmitted packet from a source. */
	uint32_t frame_counter;
	/* [Source PAN ID, Source Short Ad-dress] */
	uint32_t key_src;
	/* The key index field allows unique identification of different keys with the same
	 * originator, CCC value = 0xaa. */
	uint8_t key_index;
#define CCC_AUX_SEC_KEY_INDEX 0xaa
} __attribute__((packed));

struct ccc_hdr_ie {
	/* Concatenation of 3 bit fields : */
	uint16_t len_id_type;
	/* Length 7 bits - 5 - (bit 0 - bit 6) - Length of the IE content in bytes. */
#define IE_LEN 5
	/* IE ID  8 bits - 0 - (bit 7 - bit 14) - Vendor Specific Header IE */
#define IE_ID 0
	/* Type   1 bit  - 0 - (bit 15) - Header IE */
#define IE_TYP 0
	/* Refer to IEEEE802.15.4 2015 7.4.2.1 Header IE format. */
#define CCC_IE_LEN_ID_TYP ((IE_LEN << 0) | (IE_ID << 7) | (IE_TYP << 15))
	/* @vendor_oui: Vendor OUI
	 * Vendor specific OUI per IEEE registration authority
	 * (See http://standards-oui.ieee.org/oui.txt).
	 */
	uint8_t vendor_oui[3];
	/* @vendor_info: Vendor Specific Information 2 Bytes
	 * Byte 1: UWB message ID sent in this UWB packet
	 *         (see possible values UWB_Msg_ID_ defined below)
	 * Byte 2: UWB message length in bytes sent in this UWB packet.
	 */
	uint8_t vendor_info[2];
#define UWB_Msg_ID_PrePoll 1 /* Defined with UWB_Ranging_Start_Request in 4.5.1 */
#define UWB_Msg_ID_FinalData 2 /* Defined with UWB_Ranging_Start_Request in 4.5.1 */
} __attribute__((packed));

struct ccc_mhr {
	/* Describes how the MAC frame is configured */
	uint16_t frameCtrl;
	/* Bits meanings:
	 * 0: Frame type = data frame - see table 7-1 of 802.15.4 section 7.2.1.1.
	 * 3: Every packet has security enabled
	 * 4: Frame pending 0= default, Default value for both Pre-Poll and Final Data. If final
	 *    data payload cannot be fit into a single packet due to large number of anchors,
	 *    additional final data packets has to be transmitted. In this case set this field to
	 *    0x1 in each final data packet except the last one.
	 * 5: AR ACK is NOT Required, neither by Pre-Poll nor by FinalData
	 * 6: PAN ID compress : Source Addrg Mode set to 0, Dest Addr Mode set to 2 (short address),
	 *    and Dest PAN ID not present => 4th entry of Table 7-2 in 2.
	 * 7: Reserved.
	 * 8: Sequence number is disabled.
	 * 9: Header IE is used
	 * 10-11: Dest short Address
	 * 12-13: Frame version 802.15.4 frames for data and ack.
	 * 14: Src addr mode : Source PAN ID and Source Address field are NOT present.
	 *
	 * TODO: Checks with IEEE802154_FC_VALUE exiting macro.
	 */
#define CCC_FCTRL                                                                                \
	((1 << 0) | (1 << 3) | (0 << 4) | (0 << 5) | (1 << 6) | (0 << 7) | (1 << 8) | (1 << 9) | \
	 (2 << 10) | (2 << 12) | (0 << 14))
	/* Destination Address 2   Destination Short Address */
	uint8_t destAddr[2];
	/* Auxiliary Security Header
	 * Describes security configuration for data payload encryption and message
	 * identification authentication (MIC).
	 */
	struct ccc_aux_sec_hdr aux_sec_hdr;
	struct ccc_hdr_ie hdr_ie;
	/* Header Termination IE 2 is used in to signal end of the MHR and
	 * beginning of the MAC Payload. refer to 802.15.4 Table 7-7 Element IDs
	 * for Header IEs. */
#define LEN_HT2 0 /* Bit0-6 */
#define ELT_HT2 0x7f /* Bit7-14 */
#define TYP_HT2 0 /* Bit15 */
#define HT2 ((LEN_HT2 & 0x7f) | ((ELT_HT2 & 0xff) << 7) | ((TYP_HT2 & 1) << 15))
	/* Should be always set to HT2 = 0x3f80 as defined above. */
	uint16_t hdr_term_ie2;

} __attribute__((packed));

/*
 * @note : defined values below required by _HCI_UAD_WORKAROUND
 */
/* Start position of aux_sec_hdr relative to MHR header (frameCtrl+destAddr) */
#define AUX_SEC_HDR_MHR_OFFS sizeof(uint16_t) + 2 * sizeof(uint8_t)
/* Start position of key_src relative to aux_sec_hdr_t (sec_ctrl+frame_counter). */
#define KEYSRC_AUX_SEC_HDR_OFFS sizeof(uint8_t) + sizeof(uint32_t)
/* Start position of key_src relative to MHR. */
#define KEYSRC_MHR_OFFS (AUX_SEC_HDR_MHR_OFFS + KEYSRC_AUX_SEC_HDR_OFFS)

/*
 * @brief CCC pre-Poll (also UWB_Ranging_Start_Request) message payload
 * as described in 4.5.1 of CCC-WP-xxx-Digital-Key-R3_0.0.1
 */
struct ccc_prepoll {
	/* ID of the UWB ranging session. */
	uint32_t UWB_Session_ID;
	/* STS index of the succeeding POLL message. */
	uint32_t STS_Index;
	/* Session Index i of current ranging block. */
	uint16_t i_Block;
	/* Hop flag (1=enabled) for current ranging block as set from  ranging exchange in the prev
	 * ranging block. */
	uint8_t Hop_Flag;
	/* The ranging round index for the current ranging block as set from the ranging exchange in
	 * the previous ranging block. */
	uint16_t s_Round;
} __attribute__((packed));

/*
 * @brief CCC Final_DATA (also UWB_Ranging_Data_Exchange) message payload
 * as described in 4.5.1 of CCC-WP-xxx-Digital-Key-R3_0.0.1
 */
struct ccc_finaldata {
	/* ID of the UWB ranging session.
	 * Range: 0 - (232 - 1) */
	uint32_t UWB_Session_ID;
	/* Index i of current ranging Block.
	 * Range: 0 - 65535 */
	uint16_t i_Block;
	/* Hop mode for next ranging Block i+1.
	 * Values:
	 * - 0: No Hopping
	 * - 1: Hopping */
	uint8_t Hop_Flag;
	/* The ranging round in which the  ranging cycle will be executed in the next ranging block
	 * i + 1.
	 * Range: 0 - 65535 */
	uint16_t s_Round;
	/* STS index of the preceding Final message (in this block i)-also referred to as
	 * Validity_Timestamp.
	 * Range:  0 - (231-1) */
	uint32_t Final_STS_Index;
	/* Time difference between POLL and Final POLL messages transmit times at the initiator.
	 * Range: (N * 1/128 * 1/499.2e6 sec Time Range = 0 to 67.21 ms) */
	uint32_t Final_Tx_Timestamp;
	/* Number of timestamps to follow in this message.
	 * Range: 0 - 255 */
	uint8_t NbRxResp;
/* 4 for Initiator, 10 max for responders */
#define CCC_FRAMES_MAX (4 + MAX_NB_RESP)
/* NbRxResp shall be in [0..MAX_NB_RESP] */
#define MAX_NB_RESP 10
	/* Repeat fields 8-11 for each node (total of Number_of_Ranging_Nodes) */
	struct _resp_ts {
		/* Index x of node whose timestamp data is referred to in the
		 * Ranging_Timestamp_Node_x, Ranging_Timestamp_Uncertainty_Node_x, and
		 * Ranging_Status_Node_x fields.
		 * Range: 0 - 255 */
		uint8_t Node_Index;
		/* Timestamp = N * 1/128 * 1/499.2e6 sec = 0 to 67.21 ms Time diff
		 * btw POLL and RESP of node x,  as received by the  initiator. */
		uint32_t Ranging_Timestamp_Node_x;
		/* See Section 16.7.3 in IEEE Standard 802.15.4z
		 * Range of values from 1.5 cm - 3.6 m at various confidences. */
		uint8_t Ranging_Timestamp_Uncertainty_Node_x;
		/* See Table 8-76 in IEEE Standard 802.15.4z. */
		uint8_t Ranging_Status_Node_x;
	} __attribute__((packed)) resp_ts[MAX_NB_RESP];
} __attribute__((packed));

struct ccc_prepoll_msg {
	struct ccc_mhr mhr;
	struct ccc_prepoll prepoll;
	uint8_t fcs[2];
} __attribute__((packed));

struct ccc_finaldata_msg {
	struct ccc_mhr mhr;
	struct ccc_finaldata finaldata;
	uint8_t fcs[2];
} __attribute__((packed));

/* Refer to Table 9-6 of 802.15.4-2015 section 9.4.1.1. */
#define MIC_LEN__ENC_MIC_64 8
/* Refer to Table 9-6 of 802.15.4-2015 section 9.4.1.1. */
#define SEC_LVL__ENC_MIC_64 6

/* Length of FinalData payload when nb Responders received is 0 (i.e no timestamp to report).
 * See Table 20-6 of 20.5.1 of CCC-WP-xxx-Digital-Key-R3_0.0.3a
 */
#define FINALDATA_PLD_MIN_LEN (4 + 2 + 1 + 2 + 4 + 4 + 1)
/* 7 bytes per Responder RX timestamp to report */
#define FINALDATA_LEN_PER_RESP 7

/* Max CCC frame size taking into account auth/encryption plus 2 bytes CRC */
#define CCC_MAX_FRAME_SIZE \
	(sizeof(struct ccc_mhr) + sizeof(struct ccc_finaldata) + MIC_LEN__ENC_MIC_64 + 2)
