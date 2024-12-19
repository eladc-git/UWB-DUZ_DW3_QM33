/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "ccc/ccc.h"
#include "ccc/ccc_session.h"

/*
 * @brief  AES ECB Encryption port function
 * @param  InputMessage: pointer to input message to be encrypted.
 * @param  InputMessageLength: input data message length in byte.
 * @param  AES128_Key: pointer to the AES key to be used in the operation
 * @param  OutputMessage: pointer to output parameter that will handle the
 * encrypted message
 * @param  OutputMessageLength: pointer to encrypted message length.
 * @retval error status: _NO_ERR if successful, _ERR otherwise
 */
int port_aes_ecb_encrypt(uint8_t *InputMessage, uint32_t InputMessageLength, uint8_t *AES128_Key,
			 uint8_t *OutputMessage, uint32_t *OutputMessageLength);

/*
 * @brief  AES CCM Authenticated Encryption port function
 * @param  session: pointer to the CCC session context.
 * @param  HeaderMessage: pointer to the header message. It will be
 * authenticated but not encrypted.
 * @param  HeaderMessageLength: header message length in byte.
 * @param  Plaintext: pointer to input message to be encrypted.
 * @param  PlaintextLength: input data message length in byte.
 * @param  AES128_Key: pointer to the AES key to be used in the operation
 * @param  InitializationVector: pointer to the Initialization Vector (IV)
 * @param  IvLength: IV length in bytes.
 * @param  OutputMessage: pointer to output parameter that will handle the
 * encrypted message and TAG
 * @param  OutputMessageLength: pointer to encrypted message length.
 * @param  AuthenticationTAGLength: authentication TAG length.
 * @retval error status: _NO_ERR if successful, _ERR otherwise
 */
int port_aes_ccm_encrypt(struct ccc_session *session, struct ccc_mhr *HeaderMessage,
			 uint32_t HeaderMessageLength, uint8_t *Plaintext, uint32_t PlaintextLength,
			 uint8_t *InitializationVector, uint32_t IvLength, uint8_t *OutputMessage,
			 int32_t *OutputMessageLength, int32_t mic_len,
			 int32_t *AuthenticationTAGLength, enum ccc_key_type type, uint8_t i_kdf);
