/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "net/netlink.h"

struct genl_info {
	struct netlink_ext_ack *extack;
	uint32_t snd_seq;
	uint32_t snd_portid;
};
