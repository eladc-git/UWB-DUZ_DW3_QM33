/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

/**
 * enum uci_oid_android - Android calls identifiers through UCI.
 *
 * @UCI_OID_ANDROID_GET_POWER_STATS_CMD_RSP:
 *	PowerStats request
 * @UCI_OID_ANDROID_SET_COUNTRY_CODE_CMD_RSP:
 *	Set country code, which may lead to a reset
 *
 * See https://source.android.com/docs/core/connect/uwb-hal-interface
 */
enum uci_oid_android {
	UCI_OID_ANDROID_GET_POWER_STATS_CMD_RSP = 0x00,
	UCI_OID_ANDROID_SET_COUNTRY_CODE_CMD_RSP = 0x01,
};

/**
 * enum uci_message_android_dpf - Message Data Packet Format (DPF) definition -
 * Android extension.
 *
 * @UCI_MESSAGE_RADAR_DATA:
 *     Format: Host receives radar data from UWBS
 */
enum uci_message_android_dpf {
	UCI_MESSAGE_RADAR_DATA = 0b1111,
};

/**
 * enum uci_device_android_capabilities - Device capability parameters -
 * Android extension.
 *
 * @UCI_CAP_ANDROID_RADAR_SUPPORT:
 *     Supported radar features
 */
enum uci_device_android_capabilities {
	UCI_CAP_ANDROID_RADAR_SUPPORT = 0xb0,
};
