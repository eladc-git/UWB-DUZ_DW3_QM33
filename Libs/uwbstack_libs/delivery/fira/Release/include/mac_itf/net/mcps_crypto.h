/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "crypto/aes.h"
#include "qerr.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fira_round_hopping_sequence {
	uint8_t key[AES_KEYSIZE_128];
	void *ctx;
};

#define MCPS_CRYPTO_AES_CCM_STAR_NONCE_LEN 13

/**
 * mcps_crypto_reinit() - Reinitialize internal crypto context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_reinit(void);

/**
 * mcps_crypto_cmac_aes_128_digest() - Compute a cmac AES 128.
 * @key: AES key.
 * @data: Input data.
 * @data_len: Input data length in bytes.
 * @out: Output hash, with length AES_BLOCK_SIZE.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_cmac_aes_128_digest(const uint8_t *key, const uint8_t *data,
					  unsigned int data_len, uint8_t *out);

/**
 * mcps_crypto_cmac_aes_256_digest() - Compute a cmac AES 256.
 * @key: AES key.
 * @data: Input data.
 * @data_len: Input data length in bytes.
 * @out: Output hash, with length AES_KEYSIZE_256.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_cmac_aes_256_digest(const uint8_t *key, const uint8_t *data,
					  unsigned int data_len, uint8_t *out);

/**
 * mcps_crypto_aead_aes_ccm_star_128_create() - Create a context using
 * Authenticated Encryption Associated Data with AES CCM* 128.
 * @ctx: context to be created or reused.
 * @key: AES key.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_aead_aes_ccm_star_128_create(void **ctx, const uint8_t *key);

/**
 * mcps_crypto_aead_aes_ccm_star_128_destroy() - Destroy the Authenticated
 * Encryption Associated Data with AES CCM* 128 context.
 * @ctx: Context.
 *
 * NOTE: This API should be implemented by platform.
 */
void mcps_crypto_aead_aes_ccm_star_128_destroy(void *ctx);

/**
 * mcps_crypto_aead_aes_ccm_star_128_encrypt_inout() - Encrypt using
 * Authenticated Encryption Associated Data with AES CCM* 128.
 * @ctx: Context.
 * @nonce: Nonce, with length MCPS_CRYPTO_AES_CCM_STAR_NONCE_LEN.
 * @header: Header data.
 * @header_len: Header length in bytes.
 * @data: Data to encrypt.
 * @data_len: Data length in bytes.
 * @out: Data encrypted.
 * @mac: AES CCM* MAC.
 * @mac_len: AES CCM* MAC size in bytes.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_aead_aes_ccm_star_128_encrypt_inout(void *ctx, const uint8_t *nonce,
							  const uint8_t *header,
							  unsigned int header_len, uint8_t *data,
							  unsigned int data_len, uint8_t *out,
							  uint8_t *mac, unsigned int mac_len);

/**
 * mcps_crypto_aead_aes_ccm_star_128_encrypt() - Encrypt using Authenticated
 * Encryption Associated Data with AES CCM* 128.
 * @ctx: Context.
 * @nonce: Nonce, with length MCPS_CRYPTO_AES_CCM_STAR_NONCE_LEN.
 * @header: Header data.
 * @header_len: Header length in bytes.
 * @data: Data to encrypt, will be replaced with encrypted data.
 * @data_len: Data length in bytes.
 * @mac: AES CCM* MAC.
 * @mac_len: AES CCM* MAC size in bytes.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_aead_aes_ccm_star_128_encrypt(void *ctx, const uint8_t *nonce,
						    const uint8_t *header, unsigned int header_len,
						    uint8_t *data, unsigned int data_len,
						    uint8_t *mac, unsigned int mac_len);

/**
 * mcps_crypto_aead_aes_ccm_star_128_decrypt_inout() - Decrypt using
 * Authenticated Encryption Associated Data with AES CCM* 128.
 * @ctx: Context.
 * @nonce: Nonce, with length MCPS_CRYPTO_AES_CCM_STAR_NONCE_LEN.
 * @header: Header data.
 * @header_len: Header length in bytes.
 * @data: Data to decrypt.
 * @data_len: Data length in bytes.
 * @out: Data decrypted.
 * @mac: AES CCM* MAC.
 * @mac_len: AES CCM* MAC size in bytes.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_aead_aes_ccm_star_128_decrypt_inout(void *ctx, const uint8_t *nonce,
							  const uint8_t *header,
							  unsigned int header_len, uint8_t *data,
							  unsigned int data_len, uint8_t *out,
							  uint8_t *mac, unsigned int mac_len);

/**
 * mcps_crypto_aead_aes_ccm_star_128_decrypt() - Decrypt using Authenticated
 * Encryption Associated Data with AES CCM* 128.
 * @ctx: Context.
 * @nonce: Nonce, with length MCPS_CRYPTO_AES_CCM_STAR_NONCE_LEN.
 * @header: Header data.
 * @header_len: Header length in bytes.
 * @data: Data to decrypt, will be replaced with decrypted data.
 * @data_len: Data length in bytes.
 * @mac: AES CCM* MAC.
 * @mac_len: AES CCM* MAC size in bytes.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_aead_aes_ccm_star_128_decrypt(void *ctx, const uint8_t *nonce,
						    const uint8_t *header, unsigned int header_len,
						    uint8_t *data, unsigned int data_len,
						    uint8_t *mac, unsigned int mac_len);

/**
 * mcps_crypto_aes_ecb_128_create_encrypt() - Create AES ECB 128 encryption
 * context.
 * @ctx: context to be created or reused.
 * @key: AES key.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_aes_ecb_128_create_encrypt(void **ctx, const uint8_t *key);

/**
 * mcps_crypto_aes_ecb_128_create_decrypt() - Create AES ECB 128 decryption
 * context.
 * @ctx: context to be created or reused.
 * @key: AES key.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_aes_ecb_128_create_decrypt(void **ctx, const uint8_t *key);

/**
 * mcps_crypto_aes_ecb_128_destroy() - Destroy the AES ECB 128 context.
 * @ctx: Context.
 *
 * NOTE: This API should be implemented by platform.
 */
void mcps_crypto_aes_ecb_128_destroy(void *ctx);

/**
 * mcps_crypto_aes_ecb_128_encrypt_decrypt() - Encrypt or Decrypt using AES ECB
 * 128 context.
 * @ctx: Context.
 * @data: Data to encrypt.
 * @data_len: Data length in bytes, should be a multiple of AES_BLOCK_SIZE.
 * @out: Ciphered data with same length as data.
 *
 * NOTE: This API should be implemented by platform.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps_crypto_aes_ecb_128_encrypt_decrypt(void *ctx, const uint8_t *data,
						  unsigned int data_len, uint8_t *out);

/**
 * mcps_crypto_get_random() - Get a random number.
 *
 * Return: a random number.
 */
uint32_t mcps_crypto_get_random(void);

#ifdef __cplusplus
}
#endif
