/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/ccc/ccc_frames.h"
#include "fbs/ccc/ccc_params.h"
#include "net/ccc_region_nl.h"

#include <stdint.h>

#define MAX_NB_TX_SLOT 4
/* 4 slots for PrePoll, Poll, Final, FinalData, and the rest for Responses
 * from Responders.
 */
#define MAX_NB_SLOT (MAX_NB_TX_SLOT + MAX_NB_RESP)

/*
 * Note:
 * KDF operations (on the Initiator side) are run (if enough time between rr
 * block k-1 and rr block k), for 2 next Blocks k and k + 1, to anticipate
 * situations where both blocks have their ranging rounds consecutive in time
 * (because of hopping):
 *   RRndIdx(k) = Nrnd - 1 (where Nrnd = num of rounds per block)
 *   RRndIdx(k+1) = 0
 */
#define NEXT_KDF_BLK 0
#define PRECALC_KDF_BLK 1
#define NB_KDF_BLK 2

enum ccc_key_type {
	CCC_KEY_DUDSK,
	CCC_KEY_UPSK1,
};

struct ccc_rr_kdf {
	/**
	 * Common space used for building of SP0 data frames
	 * PrePoll and FinalData
	 */
	union {
		/* Plaintext PrePoll   MHR+payload */
		struct ccc_prepoll_msg prepoll_msg;
		/* Plaintext FinalData MHR+payload */
		struct ccc_finaldata_msg finaldata_msg;
	} sp0_frame;
	/* Index modulo 2 (NEXT_KDF_BLK or PRECALC_KDF_BLK) of KDF material
	 * (PrePoll frame, dURSK, and dUDSK) to be used for next ranging round
	 */
	uint8_t i_kdf;
	/* Indicates whether KDF material is already precomputed (from last
	 * ranging round).
	 */
	uint8_t b_kdf_rdy;
	/* Buffers used to hold Prepoll encrypted payload. */
	uint16_t enc_len[NB_KDF_BLK];
	uint8_t enc_buf[NB_KDF_BLK][sizeof(struct ccc_mhr) + sizeof(struct ccc_finaldata) +
				    MIC_LEN__ENC_MIC_64 + 2];
};
