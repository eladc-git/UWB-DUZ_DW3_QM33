/**
 * @file      common_fira.h
 *
 * @brief     Interface for common FiRa functionalities
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include "uwbmac/fira_helper.h"
#include "fira_app_config.h"

#define DATA_TRANSFER 2

/* Comment below to see the debug information in a real-time, diag_printf is a buffered I/O. */
/* #define DEBUG_SP3_MSG(...) */

#ifndef DEBUG_SP3_MSG
#include "deca_dbg.h"
#define DEBUG_SP3_MSG diag_printf
#endif

#ifndef TWR_ENTER_CRITICAL
#define TWR_ENTER_CRITICAL taskENTER_CRITICAL
#endif

#ifndef TWR_EXIT_CRITICAL
#define TWR_EXIT_CRITICAL taskEXIT_CRITICAL
#endif

/** @brief Data measurement structure. */
struct string_measurement
{
    /* Buffer of string measurement to allocate. */
    char *str;
    /* Length of measurement string. */
    uint16_t len;
};

bool fira_set_user_params(char *text, bool controller);
void fira_set_default_params(bool controller);
void fira_show_params();
enum qerr fira_prepare_measurement_sequence(struct uwbmac_context *uwbmac_ctx, struct session_parameters *session);
enum qerr fira_set_session_parameters(struct fira_context *fira_context, uint32_t session_handle, struct session_parameters *session);

void fira_uwb_mcps_init(fira_param_t *fira_param);
void fira_uwb_mcps_deinit(void);
bool fira_uwb_is_diag_enabled(void);
int fira_uwb_add_diag(const struct fira_twr_ranging_results *results, char *str, int len, int max_len);

#ifdef __cplusplus
}
#endif
