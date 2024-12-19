/**
 * @file      uci_parser.c
 *
 * @brief     UCI parser implementation
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include <ctype.h>
#include <string.h>

#include "uci_parser.h"
#include "controlTask.h"

static const char dwSTOP[] = "STOP";
static const char dwSAVE[] = "SAVE";
static char last[5] = {0};

/**
 * @brief Check if the stop command has been received
 * @return DATA_STOP if stop command was received
 *         DATA_READY if byte stream does not contain stop
 */
static usb_data_e check_for_stop(uint8_t *p, uint16_t len)
{
    for (int i = 0; i < len; i++)
    {
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = toupper(p[i]);
        if (strcmp(last, dwSTOP) == 0)
        {
            return DATA_STOP;
        }
        if (strcmp(last, dwSAVE) == 0)
        {
            return DATA_SAVE;
        }
    }
    return DATA_READY;
}

/**
 * @brief Serialize circular buffer before calling the function processing the buffer data
 *
 * @return data_ready : the data is ready for future processing in usb2spi or uci application
 * data can be found in app.local_buff : app.local_buff_len;
 * stop : stop command was detected and will exit UCI mode;
 * no_data : no valid data yet.
 */
usb_data_e cir_to_ser_buf(uint8_t *pbuf, uint16_t len, uint16_t *read_offset, uint16_t cyclic_size)
{
    usb_data_e ret;
    uint16_t cnt;

    ret = NO_DATA;

    /* Wait for valid usb2spi message from pbuf. */
    if ((len + local_buff_length) < sizeof(local_buff) - 1)
    {
        for (cnt = 0; cnt < len; cnt++)
        {
            local_buff[local_buff_length + cnt] = pbuf[*read_offset];
            *read_offset = (*read_offset + 1) & cyclic_size;
        }
        local_buff_length += cnt;
        ret = check_for_stop(local_buff, cnt);
    }
    else
    { /* Overflow in usb2spi protocol : flush everything. */
        local_buff_length = 0;
    }

    return (ret);
}
