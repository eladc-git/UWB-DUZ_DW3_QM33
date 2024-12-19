/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "ccc_kdf.h"
#include "fbs/ccc/ccc.h"
#include "fbs/ccc/ccc_frames.h"
#include "fbs/fbs_region.h"
#include "fbs/fbs_session.h"
#include "net/ccc_region_nl.h"
#include "net/mcps802154.h"
#include "net/mcps802154_schedule.h"
#include "qerr.h"
#include "qtils.h"
#include "quwbs/fbs/defs.h"

#include <stdbool.h>
#include <stdint.h>

/*
 * @Brief - refer to CCC spec section 22.1 Cryptography - UAD UWB Address
 * Derivation In order to protect against tracking of the same UWB session
 * between recoveries, the following values shall be derived at session setup or
 * upon receiving the RR-RS message:
 * - Source Short Address (2 bytes)
 * - Source PAN ID (2 bytes)
 * - Destination Short Address (2 bytes)
 * - Source Long Address (8 bytes)
 */
struct ccc_uad {
	uint16_t Source_Short_Address;
	uint16_t Source_PAN_Id;
	uint16_t Dest_Short_Address;
	uint8_t Source_Long_Address[8];
};

struct ccc_crypto {
	/* UWB Addresss Derivation */
	struct ccc_uad uad;
	/* "mURSK", is used for dURSK and dUDSK derivation, can be computed once
	 * at session startup.
	 */
	bool valid;
	uint8_t mURSK[CCC_M_URSK_BYTE_SIZE];
	uint8_t mUPSK1[CCC_M_UPSK1_BYTE_SIZE];
	uint8_t mUPSK2[CCC_M_UPSK2_BYTE_SIZE];
	uint8_t bufUAD[CCC_UAD_BYTE_SIZE];
	uint8_t buf_Salt[CCC_SALT_BYTE_SIZE];
	uint8_t buf_SaltedHash[CCC_SALTED_HASH_BYTE_SIZE];
	/**
	 * STS AES key - stored in ccc_workspace (Ranging Round context) as is
	 * updated every ranging Round (unlike PrePoll key which is per-session)
	 */
	uint8_t dURSK[NB_KDF_BLK][16];
	/**
	 * SP0 FinalData AES key - stored in ccc_workspace (Ranging Round
	 * context) as is updated every ranging Round (unlike PrePoll key which
	 * is per-session)
	 */
	uint8_t dUDSK[NB_KDF_BLK][16];
};

/* Session configuration structure */
struct ccc_session_cfg {
	/* Not used in CCC */
	enum ccc_device_type device_type;
	uint64_t time0_us;
	/*
	 * Set to true for sts_index initialization with a random or provided value,
	 * set to false for incrementing the last used sts index.
	 */
	bool is_initial_sts_index0;
	uint32_t sts_index0;
	uint32_t hop_mode_key;
	uint16_t selected_protocol_version;
	uint16_t selected_uwb_config_id;
	/* Value: 0x05 or 0x09      */
	uint8_t selected_uwb_channel;
	/* 8 Bits bitmask defined by 'HOP_CONF_MSK_*' labels, as described in
	 * table 19-25 of [CCC].
	 */
	uint8_t hop_config_bitmask;
	/* Value: 0x01 to 0x20      */
	uint8_t sync_code_index;
	/* Block duration in ms (multiple of 96 ms) */
	uint32_t tBlock_ms;
	/* Duration of a slot in RSTU */
	uint16_t slot_duration_rstu;
	/* Value: 0x01 to 0xff      */
	uint8_t number_responders_nodes;
	/* Value: 0x04, 0x06, 0x08, 0x09, 0x0C, 0x10, 0x12, 0x18, 0x20, 0x24,
	 * 0x30, 0x48, 0x60.
	 */
	uint8_t number_slot_per_round;
	/* Must be one of 0x01 0x10 0x11 0x12 0x21 */
	uint8_t selected_pulse_shape_combo;
	uint16_t ursk_ttl;
	/* To activate/deactivate CCC notif */
	uint8_t session_info_ntf_config;
	/* Not used in CCC */
	uint8_t sts_config;
	/* Not used in CCC */
	uint8_t ranging_round_control;
	/* Not used in CCC */
	uint8_t device_role;
	/* Not used in CCC */
	uint8_t multi_node_mode;
	/* Not used in CCC */
	uint8_t key_rotation;
	uint8_t report_psdus;
	struct fira_measurement_sequence *meas_seq;
	uint32_t current_sts_index;
	bool mac_payload_encryption;
	/* ACWG specific. */
	bool mac_mode_present;
	uint8_t mac_mode;
	uint8_t *ursk;
};

struct ccc_session {
	/** @base: FBS session context. */
	struct fbs_session base;
	/* Not session dependent probably not in the right place.  some content
	 * can probably be kept in application level parameters (valid for all
	 * session, all ranging round)
	 */
	/* Pre-calculated preamble duration, common to all CCC frame types.
	 * set to either PREAMB_SYNCCODE1_8_DUR_256rctu or
	 * PREAMB_SYNCCODE9_24_DUR_256rctu */
	uint16_t preamb_dur_256rctu;
	/* Configuration parameters as negotiated over BLE with Ranging Services
	 */
	struct ccc_session_cfg cfg;
#ifdef _HCI_UAD_WORKAROUND
	/* 32bits Key_src as read from built PrePoll frame MHR (by the SE), for
	 * later reference when building FinalData SP0 frame.
	 */
	uint32_t key_src;
#endif
	/* Time (in millisec) to next Ranging Round PrePoll - computed after
	 * completion of current ranging round (in rr_initiator_end()).
	 */
	int32_t time_to_next_rr_ms;
	/* Slot duration in DTU. */
	uint32_t tSlot_dtu;
	/* STS Index value at start of current ranging round. */
	uint32_t STS_Index_rr_start;
	/* See Table 20-10 - Counter that increments for each transmitted packet
	 * from a source. */
	uint32_t frame_counter;
	/* Counters for tracking RXERR regression. */
	uint32_t num_rxerr;
	/* Block index - uint16_t to match type length used in PrePoll and
	 * FinalData payload.
	 */
	uint16_t i_Block;
	/* Round index - uint16_t to match type length used in PrePoll and
	 * FinalData payload.
	 */
	uint16_t s_Round;
	/* Number of rounds per block. */
	uint16_t n_Round;
	/* Last round index used. */
	uint16_t s_last_Round;
	/* Index of next ranging round within its block, as calculated at the
	 * end of previous ranging round.
	 */
	uint16_t s_Round_next;
	enum { SUSPENDED, RUNNING } status;
	/** Duration of Ranging Round expressed in nb of chap = (1/3)ms. */
	uint16_t tRound_chap;
	/* Pre-calculated Rx timeout for Response Msg -- Not part of
	 * initialization.
	 */
	uint16_t responseRxTo_sy;
	/* Save the last_sts_index0 set. */
	uint32_t last_sts_index0;
#ifdef _HCI_SALTEDHASH_WORKAROUND
	/* Set to 1 once SaltedHash was deduced from STS-V reported by first
	 * call to ccc_run_rr_KDF_hook().
	 */
	uint8_t SaltedHash_set;
#endif
	uint8_t priority;
	/* Current hop flag (it will be set to next block hop flag at the end of
	 * round).
	 * If the session configuration hopping mode is not 0, then this
	 * flag will be updated before transmission of the Final Data message
	 * - 0: No Hopping
	 * - 1: Hopping based on Hopping scheme criterion
	 */
	uint8_t rr_HopFlag;
	/**
	 * @hrp_uwb_params: HRP UWB parameters, read only while the session is
	 * active.
	 */
	struct mcps802154_hrp_uwb_params hrp_uwb_params;
	/* Input and output material for per-session KDF
	 * used to store URSK and compute URSK, mURSK UPSK1 etc.
	 */
	struct ccc_crypto crypto;
	/**
	 * @se_key_req: pending dynamic key request.
	 */
	struct fbs_se_key_request *se_key_req;
	/* Contains key/IV material for the ranging round (used in place of
	 * sts_param[] for FiRa).
	 */
	struct ccc_rr_kdf rr_kdf;
	/**
	 * @sequence: Measurement sequence that define antenna to use.
	 */
	struct fira_measurement_sequence *sequence;
	/**
	 * @last_prepoll_status: Status of the last prepoll message.
	 */
	uint8_t last_prepoll_status;
	/**
	 * @is_time0_saved: False when a START is accepted, but the first
	 * ranging is not done yet.
	 */
	bool is_time0_saved;
	/**
	 * @nb_last_active_responder: Number of responders which answer during
	 * the last RR.
	 */
	uint8_t nb_last_active_responder;
};

static inline struct fbs_region *ccc_to_local(const struct ccc_session *ccc_session)
{
	return ccc_session->base.local;
}

static inline struct ccc_workspace *ccc_to_workspace(const struct ccc_session *ccc_session)
{
	return &ccc_to_local(ccc_session)->shared_ws.ccc;
}

void ccc_session_access_done(struct ccc_session *session, enum mcps802154_access_done_reason reason,
			     bool report_allowed);

#define CCC_SPI_RES_ERROR 4

#define DWT_DIAGNOSTIC_LOG_REV_5 (5)

/* RCTU time unit = 1/128 of 1/499.2e6 ~ 15.65 ps
 * RSTU time unit = 416/499.2E6 ~ 833ns = 416*128 RCTU = 208 * (256RCTU)
 * T_CHAP = 400RSTU  = (1/3) ms
 */
#define T_CHAP_RSTU 400
/* FIXME should be replaced/rationalized with anticp dtu */
#define MARGIN_TIME_RELEASE_ms 4

/* Default distance value. If such distance is reported, the RR did not complete succesfully. */
#define DEFAULT_DISTANCE 0x80000000

/* Wifi coex timer */
#define TIMER_DIV_FOR_WIFICOEX (DWT_XTAL_DIV128)
/* XTAL/TIMER_DIV_FOR_WIFICOEX = 38.4MHz/128 = 300kHz */
#define TIMER_FREQ_FOR_WIFICOEX_kHz (300)
/* The minimum timer period - we expect the WiFi coex to be set > 1ms prior to
 * TX of PrePoll
 */
#define CCC_MIN_TIMER_PERIOD_ms (1)
#define CCC_MIN_TIMER_PERIOD (CCC_MIN_TIMER_PERIOD_ms * TIMER_FREQ_FOR_WIFICOEX_kHz)
/* 1 hus = 100 us = 30 timer clock period when timer clock is 300 kHz */
#define hus_2_300kHz 30

/*
 * Reads a {uint2_t} big-endian encoded at specified XRAM `address`.
 * @param[in]   addr  Source address in RAM.
 *
 * @return a {uint2_t}.
 */
#define QPEEK_U2_BE(addr)                                     \
	((uint16_t)((((*(uint16_t *)(addr)) >> 8) & 0x00ff) | \
		    (((*(uint16_t *)(addr)) << 8) & 0xff00)))

/*
 * Reads a {uint4_t} big-endian encoded at specified XRAM `address`.
 * @param[in]   addr Source address in RAM.
 *
 * @return A {uint4_t}.
 */
#define QPEEK_U4_BE(addr)                                       \
	((uint32_t)((((*(uint32_t *)(addr)) >> 24) & 0xff) |    \
		    (((*(uint32_t *)(addr)) << 8) & 0xff0000) | \
		    (((*(uint32_t *)(addr)) >> 8) & 0xff00) |   \
		    (((*(uint32_t *)(addr)) << 24) & 0xff000000)))

/*
 * Reads a {uint8_t} big-endian encoded at specified XRAM `address`.
 * @param[in]   addr Source address in RAM.
 *
 * @return A {uint8_t}.
 */
#define QPEEK_U8_BE(addr)                                                \
	((uint64_t)((((*(uint64_t *)(addr)) >> 56) & 0xff) |             \
		    (((*(uint64_t *)(addr)) >> 40) & 0xff00) |           \
		    (((*(uint64_t *)(addr)) >> 24) & 0xff0000) |         \
		    (((*(uint64_t *)(addr)) >> 8) & 0xff000000) |        \
		    (((*(uint64_t *)(addr)) << 8) & 0xff00000000) |      \
		    (((*(uint64_t *)(addr)) << 24) & 0xff0000000000) |   \
		    (((*(uint64_t *)(addr)) << 40) & 0xff000000000000) | \
		    (((*(uint64_t *)(addr)) << 56) & 0xff00000000000000)))

/* "sy" = (us / 1.0256), 67213 = 1.0256 * (1 << 16). */
#define US_2_SY_RATIO_SH16 67213
#define US_2_SY(us) ((((uint32_t)us) << 16) / US_2_SY_RATIO_SH16)

/* Duration of SP0 PrePoll frame in usec. */
#define PREPOLL_DUR_us 154
/* Duration of SP3 (either Poll, Resp, or Final) frame in usec. */
#define SP3_DUR_us 139

extern const uint8_t FinalData_duration_us[1 + MAX_NB_RESP];

/* (8+64) * 0.99359 us, converted to (256rctu) = 1ms/249600. */
#define PREAMB_SYNCCODE1_8_DUR_256rctu 17856
/* (8+64) * 0.99359 us, converted to (256rctu) = 1ms/249600 */
#define PREAMB_SYNCCODE9_24_DUR_256rctu 18288

#define RREV_START 0xa0
#define RREV_TXDONE 1
#define RREV_RXOK 2
#define RREV_RXERR 3
#define RREV_RXTO 4
#define RREV_SPIRDY 5
#define RREV_SPI1MAVAIL 6
#define RREV_SPI2MAVAIL 7
/* Used for responder only. */
#define RREV_ABORT 8

static inline struct ccc_session *fbs_to_ccc(const struct fbs_session *fbs_session)
{
	/* Only the argument is const to avoid to have 2 functions. */
	return qparent_of(fbs_session, struct ccc_session, base);
}

/**
 * ccc_session_start() - Start CCC session.
 * @session: CCC session context.
 *
 * Return: QERR_SUCCESS on success or error otherwise.
 */
enum qerr ccc_session_start(struct ccc_session *session);

void prepare_next_RangingRound(struct ccc_session *session, uint32_t next_timestamp_dtu);

/*
 * CCC minimum num of SP3 Resp rxok = 1
 * We should receive at least 1 Response OK else we trigger Hoping
 */
#define MIN_THRESH_RR_RESP_OK 1
bool evaluate_Hop_criterion(uint8_t goodRxCnt);

/*
 * @brief Implements the Default Hopping Sequence as described in Appendix F.2
 * of CCC-WP-xxx-Digital-Key-R3_0.0.3a_Amendment_redline
 */
uint16_t calc_Hop_Si(uint32_t Hop_key_rw, uint16_t Nround, uint16_t i_Block);
uint16_t calc_AES_Hop_Si(uint32_t Hop_key_rw, uint16_t Nround, uint16_t i_Block);

uint32_t read_serialized_big_endian_u32(uint8_t *p_buf);

enum qerr ccc_update_sts_iv(struct ccc_session *session, int frame_idx);

void ccc_crypto_build_nonce(struct ccc_session *session, uint8_t *nonce_buf,
			    uint32_t frame_counter);

uint8_t ccc_crypto_get_idx_kdf(struct ccc_session *session);

/**
 * ccc_session_update_crypto() - Calculates the SaltedHash in case of
 * recovering the CCC session because the RAN or/and the STS Index0
 * could be changed.
 * @session: CCC session context.
 */
void ccc_session_update_crypto(struct ccc_session *session);

/**
 * ccc_session_receive_key() - Session receives value from key manager.
 * Used to calculate/derive the per session crypto operations
 * (UAD (UWB Address Derivation) and mUPSK1)
 * This function is meant to be called prior to each session start or recover
 * Refer to 22.1 of CCC-WP-xxx-Digital-Key-R3_0.0.3a_Amendment_redline
 * UAD structure is stored outside of session context, to make no-copy easier
 * @session: CCC session context.
 * @se_key_req: The key-request from the key manager.
 * @r: The error status from the key manager.
 *
 * Return: the reason code.
 */
enum quwbs_fbs_reason_code ccc_session_receive_key(struct ccc_session *session,
						   struct fbs_se_key_request *se_key_req,
						   enum qerr r);

/*
 * @Brief User-implemented hook function used to calculate the derived dURSK key
 * (STS generation) + dUDSK (FinalData payload encryption)
 * This hook function is meant to be called prior to each ranging Round
 */
void ccc_run_rr_KDF_hook(struct ccc_session *session);

/**
 * ccc_session_receive_dynamic_key() - Receives value from key manager.
 * @fbs_session: CCC session context.
 * @se_key_req: Dynamic key request descriptor.
 * @r: The error status from the key manager.
 */
void ccc_session_receive_dynamic_key(struct fbs_session *fbs_session,
				     struct fbs_se_key_request *se_key_req, enum qerr r);

void ccc_session_report_psdus(struct ccc_session *session);

void ccc_session_active_leave(struct ccc_session *session, enum quwbs_fbs_reason_code rc);

uint8_t calc_round_index_on_rr_fail(const struct ccc_session *session, uint16_t blk_i,
				    uint16_t *p_s_Round_next);

void ccc_report(struct ccc_session *session);
