/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

#define CCC_SALT_BYTE_SIZE 16
#define CCC_PADDED_SALT_BYTE_SIZE 32
#define CCC_SALTED_HASH_BYTE_SIZE 16
#define CCC_M_URSK_BYTE_SIZE 32
#define CCC_M_UPSK1_BYTE_SIZE 16
#define CCC_M_UPSK2_BYTE_SIZE 32
#define CCC_UAD_BYTE_SIZE 16
#define CCC_UDSK_BYTE_SIZE 16

/*
 * @brief Derivation of mUPSK1 and mUPSK2
 *  mUPSK1 is used in order to encrypt the SP0 Pre-POLL.
 *  mUPSK2 is used to derive the UWB addresses.
 * @param ursk   : ptr to input  32 bytes (256bits) buffer holding input URSK
 * (session key)
 * @param mUPSK1 : ptr to output 16 bytes (128bits) buffer holding computed
 * mUPSK1
 * @param mUPSK2 : ptr to output 32 bytes (256bits) buffer holding computed
 * mUPSK2
 * @return : none
 * @note as per CCC 22.1.3.1 Cryptography:
 * mUPSK1[0:128]   = CMAC(URSK, 0x00000001 || "UPSK" || 0x00 || 0x000000 ||
 * 0x00000180) mUPSK2[128:128] = CMAC(URSK, 0x00000002 || "UPSK" || 0x00 ||
 * 0x000000 || 0x00000180) mUPSK2[0:128]   = CMAC(URSK, 0x00000003 || "UPSK" ||
 * 0x00 || 0x000000 || 0x00000180) CMAC= AES256-CMAC as PRF, r = 32, h=128
 */
void calc_mUPSK(uint8_t *ursk, uint8_t *mUPSK1, uint8_t *mUPSK2);

/*
 * @brief Derivation of mURSK
 * @param ursk  : ptr to input  32 bytes (256bits) buffer holding input URSK
 * (session key)
 * @param mURSK : ptr to output 32 bytes (256bits) buffer holding computed mURSK
 * @return : none
 * @note 22.1.3.3 mURSK Key Derivation
 * Purpose: Ranging Keys Seed derivation
 * Occurrence: Once per session
 * mURSK[128:128]= CMAC(URSK, 0x00000001 || "URSK" || 0x00 || 0x000000 ||
 * 0x00000100) mURSK[0:128]  = CMAC(URSK, 0x00000002 || "URSK" || 0x00 ||
 * 0x000000 || 0x00000100) CMAC = AES256-CMAC as PRF, r = 32, h=128
 */
void calc_mURSK(uint8_t *ursk, uint8_t *mURSK);

/**
 * calc_salt() - Derivation of Salt.
 * @ursk: ptr to the session key (URSK).
 * @salt: ptr to output computed salt.
 */
void calc_salt(const uint8_t *ursk, uint8_t *salt);

/**
 * calc_salted_hash() - Derivation of SaltedHash.
 * @selected_protocol_version: protocol version of the CCC session.
 * @selected_uwb_config_id: config id of the CCC session.
 * @uwb_session_id: session id of the CCC session.
 * @sts_index0: sts index0 of the CCC session.
 * @number_responders_nodes: number of responders associated to the CCC
 *                           session.
 * @tBlock_ms: block duration of the CCC session.
 * @number_slot_per_round: nb of slot per round of the CCC session.
 * @slot_duration_rstu: slot duration in rstu of the CCC session.
 * @selected_pulseshape_combo: pulse shape combe of the CCC session.
 * @salt: ptr to salt.
 * @saltedhash: ptr to output computed SaltedHash.
 */
void calc_salted_hash(uint16_t selected_protocol_version, uint16_t selected_uwb_config_id,
		      uint32_t uwb_session_id, uint32_t sts_index0, uint8_t number_responders_nodes,
		      uint32_t tBlock_ms, uint8_t number_slot_per_round,
		      uint16_t slot_duration_rstu, uint8_t selected_pulseshape_combo, uint8_t *salt,
		      uint8_t *saltedhash);

/*
 * @brief Derivation of UAD buffer - UAD = UWB Address Derivation, used for MAC
 * header src/dest address fields
 * @param ursk   : ptr to input  32 bytes (256bits) buffer holding input
 * (previously computed) mUPSK2
 * @param sts_index0 : Initial value of STS index, as randomly selected by the
 * Initiator, and transferred back to the Responder over BLE before start of UWB
 * Ranging session
 * @param uad : ptr to output 16 bytes (128bits) buffer holding computed UAD
 * @return : none
 * @note 22.1.4.1 UWB Addresses KDF
 * Purpose: Deriving UWB Addresses (UAD)
 * Occurrence: At ranging setup or at ranging recovery
 * UAD[0:128] = CMAC(mUPSK2, 0x00000001 || "UAD" || 0x00 ||STSIndex0[0:32] ||
 * 0x00000080) CMAC = AES256-CMAC as PRF, r = 32, h=128
 */
void calc_UAD(uint8_t *mupsk2, uint32_t sts_index0, uint8_t *uad);

/*
 * @brief Derivation of dUSRK and dUDSK (to be computed every Ranging Round)
 * @param ursk   : ptr to input 32 bytes (256bits) buffer holding input
 * (previously computed) mURSK
 * @param sts_index : current value of STS index for next PrePoll
 * @param salted_hash : ptr to input 16 bytes (128bits) buffer holding input
 * (previously computed) SaltedHash
 * @param dursk : ptr to output 16 bytes (128bits) buffer holding computed dURSK
 * @param dudsk : ptr to output 16 bytes (128bits) buffer holding computed dUDSK
 * @return : none
 * @note 22.1.3.4 dURSK/dUDSK Keys Derivation (KDF_cascade definition)
 * Purpose: Ranging Keys derivation
 * Occurrence: KDF executed once per ranging cycle (before "POLL")
 * URSK_KT[128:128] = CMAC(mURSK  , 0x00000001 || "URSK_KT" ||0x00 ||each bits
 * of STS index spread into LSBs of 32 bytes || 0x00000100) URSK_KT[0:128]   =
 * CMAC(mURSK  , 0x00000002 || "URSK_KT" ||0x00 ||each bits of STS index spread
 * into LSBs of 32 bytes || 0x00000100) dURSK[0:128]     = CMAC(URSK_KT,
 * 0x00000001 || "URSK" || 0x00 || SaltedHash ||0x000000 || 0x00000080)
 * dUDSK[0:128]     = CMAC(URSK_KT, 0x00000001 || "UDSK" || 0x00 || SaltedHash
 * ||0x000000 || 0x00000080) CMAC = AES256-CMAC as PRF, r = 32, h=128
 */
void calc_dURSK_dUDSK(uint8_t *mursk, uint32_t sts_index, uint8_t *salted_hash, uint8_t *dursk,
		      uint8_t *dudsk);
