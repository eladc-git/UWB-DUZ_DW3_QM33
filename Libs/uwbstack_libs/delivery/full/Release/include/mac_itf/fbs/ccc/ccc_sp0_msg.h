/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "fbs/ccc/ccc.h"

#include <stdint.h>

struct ccc_session;
struct ccc_rr_slots;
struct ccc_finaldata_msg;
struct ccc_mhr;

void construct_finaldata(struct ccc_session *session, struct ccc_rr_slots *p_rr_slots,
			 struct ccc_rr_kdf *p_rr_kdf, struct ccc_finaldata_msg *p_finaldata_msg,
			 uint8_t *out_enc_buf, int16_t *p_out_enc_len);

void construct_finaldata_mhr(struct ccc_session *session, struct ccc_mhr *p_mhr);

void ccc_sp0_msg_encrypt(struct ccc_session *session, struct ccc_mhr *p_msg, uint8_t *out_enc_buf,
			 uint16_t *p_out_enc_len, enum ccc_key_type type, uint8_t i_kdf);
