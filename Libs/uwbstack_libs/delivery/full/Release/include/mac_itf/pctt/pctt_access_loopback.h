/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

struct pctt_local;

void pctt_get_access_loopback(struct pctt_local *local, uint32_t next_timestamp_dtu);
