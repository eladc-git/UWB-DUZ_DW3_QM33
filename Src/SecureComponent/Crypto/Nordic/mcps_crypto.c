/**
 * @file      mcps_crypto.c
 *
 * @brief     Implementaion of mcps crypto functionalities
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include <string.h>

#include <mbedtls/platform.h>
#include <mbedtls/ccm.h>
#include <mbedtls/cmac.h>
#include <mbedtls/aes.h>

#include "mcps_crypto_platform.h"
#include "qerr.h"

#define CMAC_KEY_SIZE_BYTES     (16)
#define CMAC_KEY_SIZE_BITS      (CMAC_KEY_SIZE_BYTES * 8)

#define CMAC_256KEY_SIZE_BYTES  (32)
#define CMAC_256KEY_SIZE_BITS   (CMAC_256KEY_SIZE_BYTES * 8)

#define CCM_STAR_KEY_SIZE_BYTES (16)
#define CCM_STAR_KEY_SIZE_BITS  (CCM_STAR_KEY_SIZE_BYTES * 8)
#define CCM_STAR_NONCE_LEN      (13)

#define ECB_KEY_SIZE_BYTES      (16)
#define ECB_KEY_SIZE_BITS       (ECB_KEY_SIZE_BYTES * 8)

static mbedtls_cipher_context_t gcmac_128_ctx;
static mbedtls_cipher_context_t gcmac_256_ctx;

static enum qerr mbedtls_error_to_qerr(int error)
{
    switch (error)
    {
        case 0:
            return QERR_SUCCESS;
        case MBEDTLS_ERR_CIPHER_FEATURE_UNAVAILABLE:
            return QERR_ENOTSUP;
        case MBEDTLS_ERR_CIPHER_BAD_INPUT_DATA:
        case MBEDTLS_ERR_CIPHER_INVALID_PADDING:
        case MBEDTLS_ERR_CIPHER_INVALID_CONTEXT:
        case MBEDTLS_ERR_CCM_BAD_INPUT:
            return QERR_EINVAL;
        case MBEDTLS_ERR_CIPHER_ALLOC_FAILED:
            return QERR_ENOMEM;
        case MBEDTLS_ERR_CIPHER_AUTH_FAILED:
        case MBEDTLS_ERR_CCM_AUTH_FAILED:
            return QERR_EFAULT;
        default:
            return QERR_EBADMSG;
    }
}

static int mcps_crypto_init_by_key_len(mbedtls_cipher_context_t *gcmac_ctx, unsigned int key_len)
{
    const mbedtls_cipher_info_t *gpcmac_info;

    mbedtls_cipher_init(gcmac_ctx);
    gpcmac_info = mbedtls_cipher_info_from_values(
        MBEDTLS_CIPHER_ID_AES, key_len, MBEDTLS_MODE_ECB
    );
    return mbedtls_cipher_setup(gcmac_ctx, gpcmac_info);
}

enum qerr mcps_crypto_init(void)
{
    enum qerr rc;

    rc = mcps_crypto_init_by_key_len(&gcmac_128_ctx, CMAC_KEY_SIZE_BITS);
    if (!rc)
        rc = mcps_crypto_init_by_key_len(&gcmac_256_ctx, CMAC_256KEY_SIZE_BITS);

    return mbedtls_error_to_qerr(rc);
}

void mcps_crypto_deinit(void)
{
    mbedtls_cipher_free(&gcmac_128_ctx);
    mbedtls_cipher_free(&gcmac_256_ctx);
}

enum qerr mcps_crypto_reinit(void)
{
    /* Nothing to reinit for now. */
    return QERR_SUCCESS;
}

/* TODO: Use qosal random generation to implement this function. */
uint32_t mcps_crypto_get_random(void)
{
    /* Dummy implementation. */
    static uint32_t random_seed;
    const int A = 1664525, B = 1013904223;
    random_seed = A * random_seed + B;

    return random_seed;
}

static int mcps_crypto_cmac_aes_digest(
    const uint8_t *key, unsigned int key_len,
    const uint8_t *data, unsigned int data_len,
    uint8_t *out, mbedtls_cipher_context_t *cmac_ctx
)
{
    int rc;
    const mbedtls_cipher_info_t *gpcmac_info;

    gpcmac_info = mbedtls_cipher_info_from_values(
        MBEDTLS_CIPHER_ID_AES, key_len, MBEDTLS_MODE_ECB
    );

    rc = mbedtls_cipher_setkey(cmac_ctx, key, key_len, MBEDTLS_ENCRYPT);
    if (rc)
        return rc;

    rc = mbedtls_cipher_cmac(gpcmac_info, key, key_len, data, data_len, out);

    return rc;
}

enum qerr mcps_crypto_cmac_aes_128_digest(
    const uint8_t *key, const uint8_t *data,
    unsigned int data_len, uint8_t *out
)
{
    int rc = mcps_crypto_cmac_aes_digest(key, CMAC_KEY_SIZE_BITS, data, data_len, out, &gcmac_128_ctx);
    return mbedtls_error_to_qerr(rc);
}

enum qerr mcps_crypto_cmac_aes_256_digest(
    const uint8_t *key, const uint8_t *data,
    unsigned int data_len, uint8_t *out
)
{
    int rc = mcps_crypto_cmac_aes_digest(key, CMAC_256KEY_SIZE_BITS, data, data_len, out, &gcmac_256_ctx);
    return mbedtls_error_to_qerr(rc);
}

void mcps_crypto_aead_aes_ccm_star_128_destroy(void *ctx)
{
    mbedtls_ccm_free(ctx);
    mbedtls_free(ctx);
}

int mcps_crypto_aead_aes_ccm_star_128_create(void **ccm_star_ctx, const uint8_t *key)
{
    int rc;

    if (!*ccm_star_ctx)
    {
        *ccm_star_ctx = (mbedtls_ccm_context *)mbedtls_calloc(
            1, sizeof(mbedtls_ccm_context)
        );
        if (!*ccm_star_ctx)
            return QERR_ENOMEM;
    }
    mbedtls_ccm_init(*ccm_star_ctx);
    rc = mbedtls_ccm_setkey(*ccm_star_ctx, MBEDTLS_CIPHER_ID_AES, key, CCM_STAR_KEY_SIZE_BITS);
    if (rc)
    {
        mcps_crypto_aead_aes_ccm_star_128_destroy(*ccm_star_ctx);
        *ccm_star_ctx = NULL;
        return mbedtls_error_to_qerr(rc);
    }

    return 0;
}

int mcps_crypto_aead_aes_ccm_star_128_encrypt_inout(
    void *ctx, const uint8_t *nonce, const uint8_t *header,
    unsigned header_len, uint8_t *data, unsigned data_len,
    uint8_t *out, uint8_t *mac, unsigned mac_len
)
{
    int rc = mbedtls_ccm_star_encrypt_and_tag(ctx, data_len, nonce, CCM_STAR_NONCE_LEN, header, header_len, data, out, mac, mac_len);
    return mbedtls_error_to_qerr(rc);
}

int mcps_crypto_aead_aes_ccm_star_128_encrypt(
    void *ctx, const uint8_t *nonce, const uint8_t *header,
    unsigned int header_len, uint8_t *data, unsigned int data_len,
    uint8_t *mac, unsigned int mac_len
)
{
    return mcps_crypto_aead_aes_ccm_star_128_encrypt_inout(
        ctx, nonce, header, header_len, data, data_len, data, mac,
        mac_len
    );
}

int mcps_crypto_aead_aes_ccm_star_128_decrypt_inout(
    void *ctx, const uint8_t *nonce, const uint8_t *header,
    unsigned int header_len, uint8_t *data, unsigned int data_len,
    uint8_t *out, uint8_t *mac, unsigned int mac_len
)
{
    int rc = mbedtls_ccm_star_auth_decrypt(ctx, data_len, nonce, CCM_STAR_NONCE_LEN, header, header_len, data, out, mac, mac_len);
    return mbedtls_error_to_qerr(rc);
}

int mcps_crypto_aead_aes_ccm_star_128_decrypt(
    void *ctx, const uint8_t *nonce, const uint8_t *header,
    unsigned int header_len, uint8_t *data, unsigned int data_len,
    uint8_t *mac, unsigned int mac_len
)
{
    return mcps_crypto_aead_aes_ccm_star_128_decrypt_inout(
        ctx, nonce, header, header_len, data, data_len, data, mac,
        mac_len
    );
}

void mcps_crypto_aes_ecb_128_destroy(void *ctx)
{
    mbedtls_cipher_free(ctx);
    mbedtls_free(ctx);
}

static int mcps_crypto_aes_ecb_128_create(void **ecb_ctx, const uint8_t *key, const mbedtls_operation_t operation)
{
    if (!*ecb_ctx)
    {
        *ecb_ctx = (mbedtls_cipher_context_t *)mbedtls_calloc(
            1, sizeof(mbedtls_cipher_context_t)
        );
        if (!*ecb_ctx)
            return QERR_ENOMEM;
    }

    mbedtls_cipher_init(*ecb_ctx);

    const mbedtls_cipher_info_t *ecb_info = mbedtls_cipher_info_from_values(
        MBEDTLS_CIPHER_ID_AES, ECB_KEY_SIZE_BITS, MBEDTLS_MODE_ECB
    );

    int rc = mbedtls_cipher_setup(*ecb_ctx, ecb_info);
    if (rc)
    {
        mcps_crypto_aes_ecb_128_destroy(*ecb_ctx);
        *ecb_ctx = NULL;
        return mbedtls_error_to_qerr(rc);
    }

    rc = mbedtls_cipher_setkey(*ecb_ctx, key, ECB_KEY_SIZE_BITS, operation);
    if (rc)
    {
        mcps_crypto_aes_ecb_128_destroy(*ecb_ctx);
        *ecb_ctx = NULL;
        return mbedtls_error_to_qerr(rc);
    }

    return 0;
}

int mcps_crypto_aes_ecb_128_create_decrypt(void **ecb_ctx, const uint8_t *key)
{
    return mcps_crypto_aes_ecb_128_create(ecb_ctx, key, MBEDTLS_DECRYPT);
}

int mcps_crypto_aes_ecb_128_create_encrypt(void **ecb_ctx, const uint8_t *key)
{
    return mcps_crypto_aes_ecb_128_create(ecb_ctx, key, MBEDTLS_ENCRYPT);
}

int mcps_crypto_aes_ecb_128_encrypt_decrypt(void *ctx, const uint8_t *data, unsigned int data_len, uint8_t *out)
{
    int rc;
    unsigned int crypt_len = 0;
    unsigned int round = 0;
    uint8_t iv_buf[ECB_KEY_SIZE_BYTES];

    do
    {
        rc = mbedtls_cipher_crypt(ctx, iv_buf, sizeof(iv_buf), &data[ECB_KEY_SIZE_BYTES * round], ECB_KEY_SIZE_BYTES, &out[ECB_KEY_SIZE_BYTES * round], &crypt_len);
        round++;
    } while ((rc == 0) && ((ECB_KEY_SIZE_BYTES * round) < data_len));

    return mbedtls_error_to_qerr(rc);
}
