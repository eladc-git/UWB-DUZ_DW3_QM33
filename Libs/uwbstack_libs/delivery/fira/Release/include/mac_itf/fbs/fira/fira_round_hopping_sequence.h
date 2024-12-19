/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

struct fira_session;

/**
 * fira_round_hopping_sequence_init() - Initialize round hopping context.
 * @session: Session.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_round_hopping_sequence_init(struct fira_session *session);

/**
 * fira_round_hopping_sequence_destroy() - Destroy round hopping context.
 * @session: Session.
 */
void fira_round_hopping_sequence_destroy(struct fira_session *session);

/**
 * fira_round_hopping_sequence_get() - Get round index for block index.
 * @session: Session.
 * @block_index: Block index.
 *
 * Return: Round index.
 */
int fira_round_hopping_sequence_get(const struct fira_session *session, int block_index);
