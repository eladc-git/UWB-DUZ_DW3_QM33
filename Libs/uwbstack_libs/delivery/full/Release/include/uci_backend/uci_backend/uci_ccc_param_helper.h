/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "uci/uci_message.h"

#include <uci/uci_spec_fira.h>
#include <uci_backend/uci_antenna_helper.h>
#include <uwbmac/ccc_helper.h>

/*
 * This is an internal lib to help uci_backend with command relating to session
 * configuration. The intention is to hide detail of holding the configuration
 * while providing a clean logic. Struct filed should not be accessed. Function
 * should be use instead and updated to ensure a coherent, clean and testable
 * logic in uc_backend.
 */
#define CCC_APP_PARAM_COUNT                                                             \
	UCI_APPLICATION_PARAMETER_CCC_NUM + UCI_APPLICATION_PARAMETER_VENDOR1_CCC_NUM + \
		UCI_APPLICATION_PARAMETER_VENDOR2_CCC_NUM
/* FIXME 18 wait for official spec AL reuse from fira */

// Defines used by AOSP in the UCI (r24 specification)
#define HOPPING_CONFIG_MODE_NONE 0x00
#define HOPPING_CONFIG_MODE_MODE_ADAPTIVE_DEFAULT 0x02
#define HOPPING_CONFIG_MODE_CONTINUOUS_DEFAULT 0x03
#define HOPPING_CONFIG_MODE_MODE_ADAPTIVE_AES 0x04
#define HOPPING_CONFIG_MODE_CONTINUOUS_AES 0x05
#define HOPPING_CONFIG_MODE_MAX (HOPPING_CONFIG_MODE_CONTINUOUS_AES + 1)
// Defines used by the CCC specification in the MAC
#define HOPPING_CONFIG_MODE_QORVO_NONE 0x80
#define HOPPING_CONFIG_MODE_QORVO_MODE_ADAPTIVE_DEFAULT 0x20
#define HOPPING_CONFIG_MODE_QORVO_CONTINUOUS_DEFAULT 0x40
#define HOPPING_CONFIG_MODE_QORVO_MODE_ADAPTIVE_AES 0x28
#define HOPPING_CONFIG_MODE_QORVO_CONTINUOUS_AES 0x48

extern const uint8_t hopping_config_mode_tab[];

/**
 * struct uci_ccc_app_param_item - Holds one configuration.
 *
 * NOTE: Don't use that directly as it might change.
 * Prefer using associated functions in uci_backend to have a future proof
 * logic. Current implementation allocates 53 times this struct on the heap wich
 * is 16*53=848 octets. One memory optimisation could be to use a growing link
 * list to allocate memory only for the given parameters.
 */
struct uci_ccc_app_param_item {
	/**
	 * @value: value as a union.
	 */
	union {
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
	} value;
	/**
	 * @len: type of the value.
	 */
	uint8_t len;
};

/**
 * struct uci_ccc_app_param - Holds the configuration of a session.
 *
 * NOTE: Don't use that directly as it might change.
 * Prefer using associated functions in uci_backend to have a future proof
 * logic.
 */
struct uci_ccc_app_param {
	/**
	 * @items: table of items
	 */
	struct uci_ccc_app_param_item items[CCC_APP_PARAM_COUNT];
	/**
	 * @ursk: UWB Ranging Secret Key.
	 */
	uint8_t ursk[FBS_KEY_SIZE_MAX];
};

bool uci_ccc_app_param_is_supported(uint8_t type);

bool uci_ccc_app_param_is_defined(uint8_t type);

bool uci_ccc_app_param_is_given(struct uci_ccc_app_param *param, uint8_t type);

/*bool uci_ccc_app_param_is_controlee(struct uci_ccc_app_param *params);*/

enum uci_status_code uci_ccc_app_param_check_and_set(enum quwbs_fbs_session_type stype,
						     struct uci_ccc_app_param *params,
						     struct uci_message_parser *parser,
						     uint8_t type, uint8_t len);

struct uci_ccc_app_param *uci_ccc_app_param_alloc();

void uci_ccc_app_param_free(struct uci_ccc_app_param *params);

enum qerr uci_ccc_app_param_build_message(struct uci_ccc_app_param *list,
					  struct fbs_helper_msg *msg);

void uci_ccc_app_param_clear(struct uci_ccc_app_param *params);

enum qerr uci_ccc_fill_meas_seq(struct fira_measurement_sequence *meas_seq,
				struct antenna_parameters *antennas, int channel);
