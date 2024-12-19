/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/fbs_region_nl.h"
#include "qerr.h"

struct nlattr;
struct genl_info;
struct fira_session;

/**
 * fira_session_call() - FiRa session call procedure.
 * @session: FiRa session context.
 * @call_id: Identifier of the FiRa procedure.
 * @call_attrs: Call attributes.
 * @info: Request information.
 *
 * Return: QERR_SUCCESS or error.
 */
int fira_session_call(struct fira_session *session, enum fbs_call call_id,
		      struct nlattr **call_attrs, const struct genl_info *info);

/**
 * fira_session_setup_hrp() - Set FiRa session HRP parameters.
 * @session: FiRa session context.
 *
 * Return: QERR_SUCCESS or error.
 */
void fira_session_setup_hrp(struct fira_session *session);

/**
 * fira_session_set_measurement_sequence() - Retrieve the measurement
 * schedule from a NL message and store it in the session parameters.
 * @params: The parameters contained in the NL message.
 * @info: NL context info.
 * @meas_seq: The measurement sequence where to store the information.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_session_set_measurement_sequence(const struct nlattr *params,
						const struct genl_info *info,
						struct fira_measurement_sequence **meas_seq);

/**
 * fira_session_get_measurement_sequence() - Retrieve a
 * measurement sequence in a NL message.
 * @meas_seq: The measurement sequence to add to the message.
 * @msg_buf: NL message buffer.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_session_get_measurement_sequence(const struct fira_measurement_sequence *meas_seq,
						struct sk_buff *msg_buf);
