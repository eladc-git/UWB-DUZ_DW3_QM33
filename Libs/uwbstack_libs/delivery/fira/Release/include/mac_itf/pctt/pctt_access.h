/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/mcps802154.h"
#include "net/pctt_region_params.h"
#include "qerr.h"

#include <stdbool.h>
struct llhw_vendor_cmd_pctt_get_frame_info;
struct pctt_aoa_info;
struct pctt_frame;
struct pctt_local;
struct pctt_session_params;
struct pctt_test_rssi_results;

/**
 * pctt_set_sts_params() - Set STS parameters from a PCTT session.
 * @sts_param: The sts parameters structure to set.
 * @session_params: The PCTT session parameters.
 */
void pctt_set_sts_params(struct mcps802154_sts_params *sts_param,
			 const struct pctt_session_params *session_params);

/**
 * pctt_access_setup_frame() - Fill an access frame from a PCTT slot.
 * @local: PCTT context.
 * @pf: PCTT frame information used to configure the frame in local.
 */
void pctt_access_setup_frame(struct pctt_local *local, const struct pctt_frame *pf);

/**
 * pctt_rx_error_to_result_status() - Convert Rx error into result error.
 * @error: Rx error type.
 *
 * Return: PCTT status.
 */
enum pctt_status_ranging pctt_rx_error_to_result_status(enum mcps802154_rx_error_type error);

/**
 * pctt_read_measurements() - Read measurements and fill results.
 * @local: PCTT context.
 * @rx_frame_info: Rx frame information.
 * @rssis: RSSI information.
 * @aoa_azimuth: Azimuth ranging AoA information.
 * @aoa_elevation: Elevation ranging AoA information.
 */
void pctt_read_measurements(struct pctt_local *local,
			    const struct mcps802154_rx_frame_info *rx_frame_info,
			    struct pctt_test_rssi_results *rssis, struct pctt_aoa_info *aoa_azimuth,
			    struct pctt_aoa_info *aoa_elevation);

/**
 * pctt_rx_sts_good() - Inspect the STS value in rx frame info.
 * @rx_frame_info: Rx frame information.
 *
 * Return: True if STS value is enough.
 */
bool pctt_rx_sts_good(const struct mcps802154_rx_frame_info *rx_frame_info);

/**
 * pctt_get_frame_info() - Retrieve last received frame information.
 * @local: PCTT context.
 * @frame_info: Last received frame information.
 *
 * Return: QERR_SUCCESS or an error otherwise.
 */
enum qerr pctt_get_frame_info(struct pctt_local *local,
			      struct llhw_vendor_cmd_pctt_get_frame_info *frame_info);
