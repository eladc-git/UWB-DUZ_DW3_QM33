/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <qerr.h>

#define SE_ERROR_BASE 0x00001000ULL

/*
 * enum se_error_e - SE module related error codes.
 * @SE_ERROR_SUCCESS: successful transaction.
 * @SE_ERROR_UNKNOWN: unexpected error code.
 * @SE_ERROR_SPI_DEVICE_NOT_READY: SPI device initialization is not complete.
 * @SE_ERROR_DT_SPI_NO_CS_GPIO: UWB chip could not take control of the GPIO to
 * wake up the SE.
 * @SE_ERROR_SPI_INITIALIZATION_FAILED: unable to initialize the SPI to
 * communicate with eSE.
 * @SE_ERROR_NO_SW: AGPU response does not contain SW1SW2 bytes.
 * @SE_ERROR_SW_ERROR: AGPU response SW1SW2 is different from '90''00'.
 * @SE_ERROR_INVALID_PARAMETERS: invalid function parameters.
 * @SE_ERROR_URSK_NOT_FOUND: GET_RDS response does not contain URSK TAG.
 * @SE_ERROR_URSK_INVALID_SIZE: provided buffer length too small to contain URSK
 * value.
 * @SE_ERROR_NO_SUS_APPLET: SUS applet could not be selected in the SE.
 * @SE_ERROR_SE_VENDOR_CERT_FAILED: CERT.SE-VENDOR.ECDSA did not match
 * PK.FCA.ECDSA.
 * @SE_ERROR_SD_CERT_FAILED: signature of CERT.SD.ECDSA did not match pub key of
 * CERT.SE-VENDOR.
 * @SE_ERROR_PSO_FAILED: PSO failed.
 * @SE_ERROR_GET_SUS_APPLET_VERSION_FAILED: applet version fetching failed.
 * @SE_ERROR_SCP_MUT_AUTH_FAILED: mutual authentication failed.
 * @SE_ERROR_SUS_APPLET_VERSION_MISMATCH: version retrieved from unsecure
 * channel and the one from secure channel do not match.
 * @SE_ERROR_AES_SESSION_FAILED: ephemeral AES keys mismatch between UWB and SE.
 * @SE_ERROR_BINDING_ALREADY_DONE: SE binding was already done.
 * @SE_ERROR_ISO_T1_TRANSCEIVE_FAILED: ISO_T1 transfer to eSE failed.
 * @SE_ERROR_SE_WRAP_FAILED: eSE secure channel wrapping failed.
 * @SE_ERROR_SE_UNWRAP_FAILED: eSE secure channel unwrapping failed.
 * @SE_ERROR_OOM: SE stack failed to allocate memory.
 * @SE_ERROR_CMD_ONGOING: indicate that SE is busy, a command is already in progress.
 * @SE_ERROR_TLV_INVALID_TAG: invalid TLV tag.
 * @SE_ERROR_TLV_TAG_NOT_SUPPORTED: not supported type of TLV tag.
 * @SE_ERROR_TLV_TAG_NOT_FOUND: TLV tag not found.
 * @SE_ERROR_TLV_INVALID_LENGTH: invalid TLV length.
 * @SE_ERROR_TLV_INVALID_DATA: invalid TLV.
 */
enum se_error_e {
	SE_ERROR_SUCCESS = 0,
	SE_ERROR_UNKNOWN = SE_ERROR_BASE,
	SE_ERROR_SPI_DEVICE_NOT_READY,
	SE_ERROR_DT_SPI_NO_CS_GPIO,
	SE_ERROR_SPI_INITIALIZATION_FAILED,
	SE_ERROR_NO_SW,
	SE_ERROR_SW_ERROR,
	SE_ERROR_INVALID_PARAMETERS,
	SE_ERROR_URSK_NOT_FOUND,
	SE_ERROR_URSK_INVALID_SIZE,
	SE_ERROR_NO_SUS_APPLET,
	SE_ERROR_SE_VENDOR_CERT_FAILED,
	SE_ERROR_SD_CERT_FAILED,
	SE_ERROR_PSO_FAILED,
	SE_ERROR_GET_SUS_APPLET_VERSION_FAILED,
	SE_ERROR_SCP_MUT_AUTH_FAILED,
	SE_ERROR_SUS_APPLET_VERSION_MISMATCH,
	SE_ERROR_AES_SESSION_FAILED,
	SE_ERROR_BINDING_ALREADY_DONE,
	SE_ERROR_ISO_T1_TRANSCEIVE_FAILED,
	SE_ERROR_SE_WRAP_FAILED,
	SE_ERROR_SE_UNWRAP_FAILED,
	SE_ERROR_OOM,
	SE_ERROR_CMD_ONGOING,
	SE_ERROR_TLV_INVALID_TAG = SE_ERROR_BASE + 0x100,
	SE_ERROR_TLV_TAG_NOT_SUPPORTED,
	SE_ERROR_TLV_TAG_NOT_FOUND,
	SE_ERROR_TLV_INVALID_LENGTH,
	SE_ERROR_TLV_INVALID_DATA,
};
