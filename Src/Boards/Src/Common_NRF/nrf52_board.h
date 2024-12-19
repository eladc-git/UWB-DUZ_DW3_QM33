/**
 * @file      nrf52_board.h
 *
 * @brief     Common functionalities for nrf52 based boards.
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

/* Enable or disable APPROTECT (Access Port Protection) depending on a compilation flag. */
void handle_approtect(void);
