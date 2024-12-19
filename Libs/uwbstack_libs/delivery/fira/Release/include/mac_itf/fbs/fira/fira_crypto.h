/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "crypto/aes.h"
#include "net/fbs_region_params.h"
#include "net/fira_region_params.h"
#include "qerr.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct sk_buff;
struct fira_session;
struct fbs_se_key_request;

#define FIRA_CRYPTO_AEAD_AUTHSIZE 8

/**
 * struct fira_crypto_aead - Context for payload encryption/decryption.
 */
struct fira_crypto_aead {
	/**
	 * @ctx: The context to be used during aead encryption & decryption.
	 */
	void *ctx;
};

/**
 * struct fira_crypto_base - Context containing all crypto related information.
 *
 * NOTE: It is regularly used by the FiRa region core to produce the STS
 * parameters for a given a session and to encrypt/decrypt frames.
 */
struct fira_crypto_base {
	/**
	 * @key_size: Size of the key used in the AES derivation.
	 */
	uint8_t key_size;
	/**
	 * @config_digest: Digest of the configuration, used as input for keys
	 * derivation.
	 */
	uint8_t config_digest[AES_BLOCK_SIZE];
	/**
	 * @data_protection_key: Derived from the session key, the label
	 * "DataPrtK" and the config_digest. The precise size is given by the
	 * key_size.
	 */
	uint8_t data_protection_key[FIRA_KEY_SIZE_MAX];
	/**
	 * @derived_authentication_iv: Derived from data_protection_key, the
	 * label "DerAuthI", the current value of crypto_sts_index, and the
	 * config_digest. Used to compute the STS parameters for a slot.
	 */
	uint8_t derived_authentication_iv[AES_BLOCK_SIZE];
	/**
	 * @derived_authentication_key: Derived from data_protection_key, the
	 * label "DerAuthK", the current value of crypto_sts_index and the
	 * config_digest. Used to compute the STS parameters for a slot.
	 */
	uint8_t derived_authentication_key[FIRA_KEY_SIZE_MIN];
	/**
	 * @derived_payload_key: Derived from data_protection_key, the label
	 * "DerPaylK", the current value of crypto_sts_index and the
	 * config_digest. Used to encrypt/decrypt message PIE.
	 */
	uint8_t derived_payload_key[FIRA_KEY_SIZE_MIN];
	/**
	 * @phy_sts_index_init: Initial phy_sts_index deduced at context init.
	 */
	uint32_t phy_sts_index_init;
	/**
	 * @aead: AEAD Context for payload encryption/decryption.
	 */
	struct fira_crypto_aead aead;
};

struct fira_crypto {
	/**
	 * @id: Id of the session or sub-session using the fira_crypto.
	 */
	uint32_t id;
	/**
	 * @sts_config: The type of STS requested for this crypto.
	 */
	enum fbs_sts_mode sts_config;
	/**
	 * @base: Common parameters between all types of crypto contexts.
	 */
	struct fira_crypto_base base;
	/**
	 * @privacy_key: Derived from the session key, the label
	 * "PrivacyK" and the config_digest. Used to encrypt/decrypt message
	 * HIE.
	 */
	uint8_t privacy_key[FIRA_KEY_SIZE_MIN];
	/**
	 * @vupper64: The vupper 64 to use when static STS is used.
	 */
	uint8_t vupper64[FIRA_VUPPER64_SIZE];
	/**
	 * @mac_payload_encryption: Status of mac payload encryption.
	 */
	bool mac_payload_encryption;
};

/**
 * struct fira_crypto_params - Arguments grouping structure for the fira
 * crypto context initialization function.
 */
struct fira_crypto_params {
	/**
	 * @se_key: Pointer on the key descriptor containing input/output from
	 * key-manager.
	 */
	struct mcps802154_se_key *se_key;
	/**
	 * @sts_config: The type of STS requested for the requested crypto
	 * context.
	 */
	enum fbs_sts_mode sts_config;
	/**
	 * @concat_params: The concatenated parameters of the session according
	 * to FiRa specifications.
	 */
	const uint8_t *concat_params;
	/**
	 * @concat_params_size: The size of the concatenated parameters.
	 */
	int concat_params_size;
	/**
	 * @vendor_id: The Vendor ID used to construct vUpper64 in static STS.
	 */
	const uint8_t *vendor_id;
	/**
	 * @static_sts_iv: The Static STS IV used to construct vUpper64 in
	 * static STS.
	 */
	const uint8_t *static_sts_iv;
	/**
	 * @prov_key: The key when provisioned STS is used.
	 */
	const uint8_t *prov_key;
	/**
	 * @mac_payload_encryption: Status of mac payload encryption.
	 */
	bool mac_payload_encryption;
};

/**
 * fira_crypto_get_sts_capabilities() - Get constant STS capabilities.
 *
 * Return: supported STS configurations:
 *    bit 0 : FBS_STS_MODE_STATIC supported
 *    bit 1 : FBS_STS_MODE_DYNAMIC supported
 *    bit 2 : FBS_STS_MODE_DYNAMIC_INDIVIDUAL_KEY supported
 *    bit 3 : FBS_STS_MODE_PROVISIONED supported
 *    bit 4 : FBS_STS_MODE_PROVISIONED_INDIVIDUAL_KEY supported
 *    other : not used
 */
uint32_t fira_crypto_get_sts_capabilities(void);

/**
 * fira_crypto_request_key() - Request for the key depending of the parameters.
 * @session: The session requesting key.
 * @params: Parameters to initialize the key request.
 * @se_key_req: output parameter for dynamic key request.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_request_key(struct fira_session *session, struct fira_crypto_params *params,
				  struct fbs_se_key_request **se_key_req);

/**
 * fira_crypto_new() - Allocate and initializes a crypto context.
 * @crypto_params: Parameters to initialize the crypto context.
 * @crypto: Pointer to update with the new crypto context.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_new(const struct fira_crypto_params *crypto_params,
			  struct fira_crypto **crypto);

/**
 * fira_crypto_delete() - Deinitialize and free the crypto context.
 * @crypto: The crypto context to delete.
 */
void fira_crypto_delete(struct fira_crypto *crypto);

/**
 * fira_crypto_kdf() - Key derivation function for FiRa.
 *
 * @input_key: AES input key.
 * @input_key_len: Length of AES input key.
 * @label: KDF label (8 bytes).
 * @ctxt: KDF context (16 bytes).
 * @output_key: AES output key.
 * @output_key_len: Length of AES output key.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_kdf(const uint8_t *input_key, unsigned int input_key_len, const char *label,
			  const uint8_t *ctxt, uint8_t *output_key, unsigned int output_key_len);

/**
 * fira_crypto_rotate_elements() - Rotate the crypto elements contained in the
 * crypto context.
 *
 * NOTE: After calling this function, all active crypto elements will be the
 * latest rotated ones.
 *
 * @crypto: The context containing the elements to rotate.
 * @crypto_sts_index: The crypto STS index to use to rotate the elements.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_rotate_elements(struct fira_crypto *crypto, const uint32_t crypto_sts_index);

/**
 * fira_crypto_build_phy_sts_index_init() - Build the phy STS index init value
 * related to the given crypto context.
 *
 * @crypto: The context to use to compute the phy STS index init value.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_build_phy_sts_index_init(struct fira_crypto *crypto);

/**
 * fira_crypto_get_sts_params() - Build and get the STS parameters according to
 * a specific crypto context.
 *
 * NOTE: The elements built are the STS value and the STS key. Their
 * construction depends on the STS config and is described in the FiRa MAC
 * specification.
 *
 * @crypto: The context to use to build the STS parameters.
 * @crypto_sts_index: The crypto STS index to use to build the STS parameters.
 * @sts_v: The output buffer for STS V.
 * @sts_v_size: The size of the output buffer for STS V.
 * @sts_key: The output buffer for STS key.
 * @sts_key_size: The size of the output buffer for STS key.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_get_sts_params(struct fira_crypto *crypto, uint32_t crypto_sts_index,
				     uint8_t *sts_v, uint32_t sts_v_size, uint8_t *sts_key,
				     uint32_t sts_key_size);

/**
 * fira_crypto_prepare_decrypt() - Prepare skb for header decryption and
 * verification.
 * @crypto: The crypto context used to decrypt the frame.
 * @skb: Buffer containing the frame to decrypt.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_prepare_decrypt(struct fira_crypto *crypto, struct sk_buff *skb);

/**
 * fira_crypto_encrypt_frame() - Encrypt a 802154 frame using a given context.
 *
 * NOTE: The src address is given as an argument as it is a part of the nonce
 * needed to encrypt the frame and it is not present in the 802154 frame. The
 * length of the header is given because only the payload is encrypted even if
 * the encryption algorithm needs the whole 802154 frame.
 * Encryption is done in-place.
 * When called this function shall increase the size of the skb of
 * FIRA_CRYPTO_AEAD_AUTHSIZE and set the correct bits in the 802154 frame SCF.
 *
 * @crypto: The context to use to encrypt the frame.
 * @skb: The buffer containing the whole frame, skb->data points to the start of
 * the 802154 frame header.
 * @header_len: The length of the 802154 frame header. Can be used to find the
 * position of the 802154 frame payload relative to skb->data.
 * @src_short_addr: The short source address attached to the frame.
 * @crypto_sts_index: The crypto STS index attached to the frame.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_encrypt_frame(struct fira_crypto *crypto, struct sk_buff *skb, int header_len,
				    uint16_t src_short_addr, uint32_t crypto_sts_index);

/**
 * fira_crypto_decrypt_frame() - Decrypt a 802154 frame using a given context.
 *
 * NOTE: The src address is given as an argument as it is a part of the nonce
 * needed to decrypt the frame and it is not present in the 802154 frame. The
 * length of the header is given because only the payload is encrypted even if
 * the encryption algorithm needs the whole 802154 frame.
 * Decryption is done in-place.
 * When called, this function shall reduce the
 * size of the skb of FIRA_CRYPTO_AEAD_AUTHSIZE and verify the correct bits in
 * the 802154 frame SCF.
 *
 * @crypto: The crypto to use to decrypt the frame.
 * @skb: The buffer containing the whole frame, skb->data points to the start of
 * the 802154 frame payload.
 * @header_len: The length of the 802154 frame header. Can be used to find the
 * start of the 802154 frame payload relative to skb->data.
 * @src_short_addr: The short source address attached to the frame.
 * @crypto_sts_index: The crypto STS index attached to the frame.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_decrypt_frame(struct fira_crypto *crypto, struct sk_buff *skb, int header_len,
				    uint16_t src_short_addr, uint32_t crypto_sts_index);

/**
 * fira_crypto_encrypt_hie() - Encrypt a 802154 header using a given context.
 *
 * @crypto: The crypto to use to encrypt the frame.
 * @skb: The buffer containing the whole frame, skb->data points to the start of
 * the 802154 frame header.
 * @hie_offset: Offset of the FiRa HIE relative to skb->data.
 * @hie_len: The length of the FiRa HIE.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_encrypt_hie(struct fira_crypto *crypto, struct sk_buff *skb, int hie_offset,
				  size_t hie_len);

/**
 * fira_crypto_decrypt_hie() - Decrypt a 802154 header using a given context.
 *
 * @crypto: The crypto to use to encrypt the frame.
 * @skb: The buffer containing the whole frame, skb->data points to the start of
 * the 802154 frame payload.
 * @hie_offset: Offset of the FiRa HIE relative to skb->data.
 * @hie_len: The length of 802154 header.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_decrypt_hie(struct fira_crypto *crypto, struct sk_buff *skb, int hie_offset,
				  size_t hie_len);

/**
 * fira_crypto_kdf() - Key derivation function for FiRa.
 *
 * @input_key: AES input key.
 * @input_key_len: Length of AES input key.
 * @label: KDF label (8 bytes).
 * @ctxt: KDF context (16 bytes).
 * @output_key: AES output key.
 * @output_key_len: Length of AES output key.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_crypto_kdf(const uint8_t *input_key, unsigned int input_key_len, const char *label,
			  const uint8_t *ctxt, uint8_t *output_key, unsigned int output_key_len);
