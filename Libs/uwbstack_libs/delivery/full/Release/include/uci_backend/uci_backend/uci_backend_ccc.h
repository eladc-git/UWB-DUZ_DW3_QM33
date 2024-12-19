/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "uci_ccc_param_helper.h"

#include <uci/uci.h>
#include <uci_backend/uci_antenna_helper.h>
#include <uci_backend/uci_backend_coordinator.h>
#include <uci_backend/uci_backend_manager.h>
#include <uwbmac/ccc_helper.h>

struct uwbmac_context;

// Struct to handle STATUS_NTF delay during a stop command
struct ccc_stop_ntf_policy {
	// session being stop, or negative if no session are being stopped.
	int64_t session_handle;
	// if >= 0 indicating that the session was stopped with this reason.
	int16_t stopped_reason;
};

/**
 * struct ccc_device_params - Definition of device specific parameters
 */
struct ccc_device_params {
	/**
	 * @default_channel: the device default channel.
	 */
	uint8_t default_channel;
};

/**
 * struct uci_backend_ccc_context - Definition of this backend context.
 *
 * NOTE: UCI spec defines several command to set the configuration for a test.
 * It is consumed in one helper call, hence the context hold the various part of
 * the configuration.
 */
struct uci_backend_ccc_context {
	/**
	 * @ccc_context: context for ccc helper.
	 */
	struct ccc_context ccc_context;
	/**
	 * @uwbmac_context: UWB MAC context.
	 */
	struct uwbmac_context *uwbmac_context;
	/**
	 * @device_param: devices parameters.
	 */
	struct ccc_device_params device_param;
	/**
	 * @uci: uci context used in callbacks from the helper.
	 */
	struct uci *uci;
	/**
	 * @uci_backend_core_context: UCI core backend context.
	 */
	struct uci_backend_core_context *uci_backend_core_context;
	/**
	 * @coord: uci backend coordinator.
	 */
	struct uci_backend_coordinator *coord;
	/**
	 * @sess_man: common FiRa sub-modules sessions manager.
	 */
	struct uci_backend_manager *sess_man;
	/**
	 * @antennas: antennas parameters.
	 */
	struct antenna_parameters *antennas;
	/**
	 * @session_ops: Ops for the CCC backend.
	 */
	struct uci_session_controller_ops session_ops;
};

/**
 * uci_backend_ccc_init() - Attach this backend to the uci context to bridge uci
 * communication to the MAC.
 *
 * @context: CCC backend context.
 * @uci: UCI context.
 * @uci_backend_core_context: Core backend context.
 * @uwbmac_context: UWB MAC context.
 * @coord: backend coordinator.
 * @sess_man: session manager.
 *
 * Return: 0 or an error code.
 */
enum qerr uci_backend_ccc_init(struct uci_backend_ccc_context *context, struct uci *uci,
			       struct uci_backend_core_context *uci_backend_core_context,
			       struct uwbmac_context *uwbmac_context,
			       struct uci_backend_coordinator *coord,
			       struct uci_backend_manager *sess_man);

enum qerr uci_ccc_session_init(struct uci *uci, uint16_t mt_gid_oid, uint32_t session_id,
			       void *user_data);

enum qerr uci_ccc_session_deinit(struct uci *uci, uint16_t mt_gid_oid,
				 const struct uci_blk *payload, void *user_data);

enum qerr uci_ccc_session_set_app_config(struct uci *uci, uint16_t mt_gid_oid,
					 struct uci_session *session_info,
					 const struct uci_blk *payload, void *user_data);

enum qerr uci_ccc_session_get_app_config(struct uci *uci, uint16_t mt_gid_oid,
					 const struct uci_blk *payload, void *user_data);

enum qerr uci_ccc_session_get_count(struct uci *uci, uint16_t mt_gid_oid,
				    const struct uci_blk *payload, void *user_data);

enum qerr uci_ccc_session_get_state(struct uci *uci, uint16_t mt_gid_oid,
				    const struct uci_blk *payload, void *user_data);

/**
 * uci_backend_ccc_release() - Free global ressources used.
 * @context: CCC backend context.
 */
void uci_backend_ccc_release(struct uci_backend_ccc_context *context);

/**
 * uci_backend_ccc_set_antenna_conf() - Give backend the antenna conf to
 * configure session according to aoa requests.
 * @context: This backend context.
 * @antennas_params: The antenna configuration.
 */
void uci_backend_ccc_set_antenna_conf(struct uci_backend_ccc_context *context,
				      struct antenna_parameters *antennas_params);
