/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/ccc/ccc.h"

/* Index of PrePoll   timestamp in ccc_rr_slots.tstamp[] table. */
#define PREPOLL_TS 0
/* Index of Poll      timestamp in ccc_rr_slots.tstamp[] table. */
#define POLL_TS 1
/* Index of 1st Resp  timestamp in ccc_rr_slots.tstamp[] table. */
#define RESP1_TS 2
/* Index of Final     timestamp in ccc_rr_slots.tstamp[] table. */
#define FINAL_TS (MAX_NB_SLOT - 2)
/* Index of FinalData timestamp in ccc_rr_slots.tstamp[] table. */
#define FINALDATA_TS (MAX_NB_SLOT - 1)

/* Used as default value at ranging round initialization. */
#define RR_SLOT_RESET 0
#define RR_SLOT_TXOK 1
#define RR_SLOT_TXLATE 2
#define RR_SLOT_RXOK 3
#define RR_SLOT_RXLATE 4
#define RR_SLOT_RXTO 5
#define RR_SLOT_RXERR 6
/* Fail to decrypt/decode SP0 payload (relevant for PrePoll or FinalData). */
#define RR_SLOT_RXSP0ERR 7
/* STS timestamp quality failed to pass criteria (thresh). */
#define RR_SLOT_RXSP3REJ 8
/* Frame Filter Rejection. The destination short address in SP0 frame does not
 * match expected.
 */
#define RR_SLOT_RXFFREJ 9

struct ccc_rr_slots {
	/* Store TX or RX timestamps as read from DW3000, along the complete
	 * Ranging Round.
	 */
	uint64_t tstamp[MAX_NB_SLOT];
	/* Store STS quality as read after reception event OK. */
	int16_t sts_qual[MAX_NB_SLOT];
	/* Store STS status for each of RX slots of the ranging round. */
	uint16_t stat_sts[MAX_NB_SLOT];
	/* Store status for each slots of the ranging round, can be any of
	 * following RR_SLOT_* defines.
	 */
	uint8_t stat[MAX_NB_SLOT];
#ifdef _RESP_RXDIAG_ENABLE
	/* For the Initiator, the total max nb of RX slots is:
	 * MAX_NB_SLOT - 4.
	 */
	dwt_rxdiag_t rxdiag[MAX_NB_SLOT - 4];
#endif
	/* Timer period value. */
	int32_t coex_timer_period;
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
};

/**
 * struct ccc_deferred - Deferred context.
 */
struct ccc_deferred {
	/**
	 * @slot_idx: Slot index of the PSDU.
	 */
	uint8_t slot_idx;
	/**
	 * @psdu: PSDU to store, can be null if no job to do.
	 */
	struct sk_buff *psdu;
};

/**
 * struct ccc_workspace - CCC workspace context.
 */
struct ccc_workspace {
	/**
	 * @rr_slots: Information on ranging for the current session. Index
	 *   in the table is determined by the order of the ranging messages.
	 */
	struct ccc_rr_slots rr_slots;
	/**
	 * @rr_kdf: Point to key/IV material for the ranging round (used in
	 * place of sts_param[] for FiRa)
	 */
	struct ccc_rr_kdf *rr_kdf;
	/**
	 * @deferred: Deferred context.
	 */
	struct ccc_deferred deferred;
	/**
	 * @psdus_report: Report containing all the PSDUs of the ranging round.
	 */
	struct sk_buff *psdus_report;
	/**
	 * @step: All informations needed for the step.
	 */
	struct {
		/**
		 * @next_index: Next step index.
		 */
		int16_t next_index;
		/**
		 * @n_slots: Number of slots.
		 */
		int16_t n_slots;
	} step;
};
