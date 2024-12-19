/**
 * @file      uci_transport.c
 *
 * @brief     uci transport functions
 *
 * @author    Qorvo Applications
 *
 * @copyright SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *            SPDX-License-Identifier: LicenseRef-QORVO-2
 *
 */

#include <stdlib.h>

#include "reporter.h"
#include "HAL_timer.h"
#include "uci_transport.h"
/* nordic_common.h is not protecting MIN / MAX definition. */
#include "minmax.h"

#define UCI_GARBAGE_TIMEOUT_MS 100

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - __builtin_offsetof(type, member)))

static struct uci_blk *rx = NULL;
static uint16_t rx_offset = 0;

void uci_tp_attach(struct uci_transport *tr, struct uci *uci)
{
    struct uci_tp *s = container_of(tr, struct uci_tp, tr);

    s->uci = uci;
}

void uci_tp_detach(struct uci_transport *tr)
{
    struct uci_tp *s = container_of(tr, struct uci_tp, tr);

    if (rx)
    {
        bool found = false;
        if (s->uci)
        {
            struct uci_blk *block = s->uci->rx;
            struct uci_blk *next;
            while (block)
            {
                next = block->next;
                if (rx == block)
                {
                    found = true;
                    break;
                }
                block = next;
            }
        }

        if (!found)
        {
            uci_blk_free_all(s->uci, rx);
        }
        rx = NULL;
    }

    rx_offset = 0;

    s->uci = NULL;
}

void uci_tp_usb_packet_send_ready(struct uci_transport *tr)
{
    struct uci_tp *s = container_of(tr, struct uci_tp, tr);
    struct uci_blk *p;

    while ((p = uci_packet_send_get_ready(s->uci)))
    {
        struct uci_blk *current = p;

        while (current)
        {
            reporter_instance.print((char *)current->data, current->len);
            current = current->next;
        }

        uci_packet_send_done(s->uci, p, 0);
    }
}

/**
 * @brief Platform agnostic read entry point for the UCI transport read.
 * This function process the buffer provided in entry and
 * check if it contains a valid UCI packet.
 * If it does -> call UCI-CORE
 * If it does not -> return NO_PACKET
 * @param tr Transport structure containing buffer to read from, buffer len and uci structure.
 * @return Number of byte read from the input buffer.
 */

int uci_tp_read(struct uci_tp *tr)
{
    uint16_t n = 0;
    uint16_t data_size = *(tr->p_read_buf_size);
    uint16_t available = data_size;
    char *buf = tr->read_buf;
    static bool msg_pending = false;
    static uint32_t tmr_rd;

    while (available > 0)
    {
        if (!rx)
        {
            rx = uci_blk_alloc(tr->uci, UCI_MAX_PACKET_SIZE, 0);
            if (!rx)
                return QERR_ENOMEM;
        }

        if (rx_offset < UCI_PACKET_HEADER_SIZE)
            n = MIN(UCI_PACKET_HEADER_SIZE - rx_offset, available);
        else
            n = MIN(UCI_PACKET_HEADER_SIZE + rx->data[3] - rx_offset, available);

        memcpy(rx->data + rx_offset, buf, n);

        buf += n;
        rx_offset += n;
        available -= n;

        /* If received data has same len as declared in header then assume
         * it's a complete UCI message and pass it to the stack. */
        if (rx_offset == (UCI_PACKET_HEADER_SIZE + rx->data[3]))
        {
            rx->len = rx_offset;
            uci_packet_recv(tr->uci, rx);
            rx = NULL;
            rx_offset = 0;
            msg_pending = false;
        }
        /* Not a complete message. */
        else if (rx_offset > 0)
        {
            if (msg_pending)
            {
                /* If message is pending completion for too long, flush the buffer. */
                if (Timer.check(tmr_rd, UCI_GARBAGE_TIMEOUT_MS))
                {
                    uci_tp_flush(tr);
                    msg_pending = false;
                    return QERR_EINVAL;
                }
            }
            else
            {
                Timer.start(&tmr_rd);
            }
            msg_pending = true;
        }
    }

    return data_size;
}

void uci_tp_flush(struct uci_tp *tr)
{
    uci_blk_free_all(tr->uci, rx);
    rx = NULL;
    rx_offset = 0;
}
