/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#include <uci/uci.h>
#include <uci/uci_spec_fira.h>
#include <uci_backend/uci_backend_coordinator.h>
#include <uci_backend/uci_backend_core.h>
#include <uci_backend/uci_backend_manager.h>
#include <uwbmac/radar_helper.h>

struct uwbmac_context;

/**
 * struct radar_stop_ntf_policy - Struct to handle STATUS_NTF delay during a
 * stop command
 */
struct radar_stop_ntf_policy {
	/**
	 * @session_handle: current session handle.
	 */
	int64_t session_handle;
	/**
	 * @stopped_reason: session stop reason.
	 */
	int16_t stopped_reason;
};

/**
 * struct uci_backend_radar_context - Definition of this backend context.
 *
 * NOTE: UCI spec defines several command to set the configuration for a test.
 * It is consumed in one helper call, hence the context hold the various part of
 * the configuration.
 */
struct uci_backend_radar_context {
	/**
	 * @radar_context: context for radar helper.
	 */
	struct radar_context radar_context;
	/**
	 * @uwbmac_context: UWB MAC context.
	 */
	struct uwbmac_context *uwbmac_context;
	/**
	 * @deinit_when_stop_notified: Stop notification shall deinit session.
	 */
	bool deinit_when_stop_notified;
	/**
	 * @uci: uci context used in callbacks from the helper.
	 */
	struct uci *uci;
	/**
	 * @coord: uci backend coordinator.
	 */
	struct uci_backend_coordinator *coord;
	/**
	 * @sess_man: common FiRa sub-modules sessions manager.
	 */
	struct uci_backend_manager *sess_man;
	/**
	 * @stop_ntf_policy: STATUS_NTF delay during a stop command.
	 */
	struct radar_stop_ntf_policy stop_ntf_policy;
	/**
	 * @session_ops: Ops for the radar backend.
	 */
	struct uci_session_controller_ops session_ops;
};

/**
 * uci_backend_radar_init() - Attach this backend to the uci context to bridge
 * uci communication to the MAC.
 * @context: Radar backend context.
 * @uci_backend_core_context: Core UCI backend context.
 * @uci: UCI context.
 * @uwbmac_context: UWB MAC context.
 * @coord: backend coordinator.
 * @sess_man: session manager.
 *
 * Return: 0 or an error code.
 */
enum qerr uci_backend_radar_init(struct uci_backend_radar_context *context,
				 struct uci_backend_core_context *uci_backend_core_context,
				 struct uci *uci, struct uwbmac_context *uwbmac_context,
				 struct uci_backend_coordinator *coord,
				 struct uci_backend_manager *sess_man);

/**
 * uci_backend_radar_release() - Free global resources used.
 * @context: This backend context.
 */
void uci_backend_radar_release(struct uci_backend_radar_context *context);
