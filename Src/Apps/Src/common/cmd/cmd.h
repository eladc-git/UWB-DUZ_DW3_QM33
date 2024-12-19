/**
 * @file      cmd.h
 *
 * @brief     Interface for command.c
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#pragma once

#include "app.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CMD_LEN   9

#define STR_HELPER(x) #x
#define STR(x)        STR_HELPER(x)

/* Command driver states. */
typedef enum
{
    _NO_COMMAND = 0,
    _COMMAND_FOUND,
    _COMMAND_ALLOWED
} command_e;


/* For Json parsing. */
#define CMD_NAME   "CMD_NAME"
#define CMD_PARAMS "CMD_PARAMS"


/**
 * @brief Check if input text in known "COMMAND" or "PARAMETER=VALUE" format
 * and executes COMMAND or set the PARAMETER to the VALUE.
 */
void command_parser(usb_data_e res, char *text);

usb_data_e waitForCommand(uint8_t *pBuf, uint16_t len, uint16_t *read_offset, uint16_t cyclic_size);

#ifdef __cplusplus
}
#endif
