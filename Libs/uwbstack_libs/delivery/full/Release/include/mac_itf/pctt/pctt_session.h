/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/pctt_region_nl.h"
#include "net/pctt_region_params.h"
#include "qerr.h"

#include <stdbool.h>
#include <stdint.h>

struct nlattr;

#define PCTT_PAYLOAD_MAX_LEN 4096

struct pctt_session_params {
	enum pctt_device_role device_role;
	uint16_t short_addr;
	uint16_t dst_short_addr;
	uint8_t rx_antenna_selection;
	uint8_t tx_antenna_selection;
	int slot_duration_dtu;
	int channel_number;
	int preamble_code_index;
	enum pctt_rframe_config rframe_config;
	enum pctt_preamble_duration preamble_duration;
	enum pctt_sfd_id sfd_id;
	enum pctt_number_of_sts_segments number_of_sts_segments;
	enum pctt_psdu_data_rate psdu_data_rate;
	enum pctt_mac_fcs_type mac_fcs_type;
	enum pctt_prf_mode prf_mode;
	enum pctt_phr_data_rate phr_data_rate;
	uint32_t sts_index;
	enum pctt_sts_length sts_length;
	/* Test specific parameters */
	uint32_t num_packets;
	int gap_duration_dtu;
	uint32_t t_start_dtu;
	uint32_t t_win_dtu;
	uint8_t randomize_psdu;
	uint8_t phr_ranging_bit;
	uint32_t rmarker_tx_start;
	uint32_t rmarker_rx_start;
	uint8_t sts_index_auto_incr;
	uint16_t rssi_outliers;
	/* Data payload to put in TX test frame */
	struct sk_buff *data_payload;
};

/**
 * struct pctt_session - Session information.
 */
struct pctt_session {
	/**
	 * @params: Session parameters, mostly read only while the session is
	 * active.
	 */
	struct pctt_session_params params;
	/**
	 * @event_portid: Port identifier to use for notifications.
	 */
	uint32_t event_portid;
	/**
	 * @packet_idx: Next packet index.
	 */
	int32_t packet_idx;
	/**
	 * @rx_synchronized: True after the first successful reception.
	 */
	bool rx_synchronized;
	/**
	 * @stop_request: True to not start an another access.
	 */
	bool stop_request;
	/**
	 * @next_timestamp_dtu: next date for next frame.
	 */
	uint32_t next_timestamp_dtu;
	/**
	 * @frame_idx: Frame index (same type as num_packets parameter).
	 */
	uint32_t frame_idx;
	/**
	 * @state: State of the session.
	 */
	const struct pctt_session_fsm_state *state;
	/**
	 * @cmd_id: test identifier in progress.
	 */
	enum pctt_id_attrs cmd_id;
};

/* Forward declaration. */
struct pctt_local;

/**
 * pctt_session_is_ready() - Test whether a session is ready to be started.
 * @local: PCTT context.
 *
 * Return: True if the session can be started.
 */
bool pctt_session_is_ready(const struct pctt_local *local);

/**
 * pctt_session_fsm_common_check_parameters() - Validate session parameters
 * combination.
 * @local: PCTT context.
 * @attrs: Netlink attributes.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr pctt_session_fsm_common_check_parameters(struct pctt_local *local, struct nlattr **attrs);
