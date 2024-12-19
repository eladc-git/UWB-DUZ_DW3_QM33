/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

/* NLA_POLICY_FULL_RANGE expect to have a range defined.
 * This define exist for backport linux compilations, as:
 *  - struct netlink_range_validation don't exist,
 *  - unused object are considered as error. */
#define ADD_NETLINK_RANGE_VALIDATION

#define MAP_uint8_t u8
#define MAP_int8_t s8
#define MAP_uint16_t u16
#define MAP_int16_t s16
#define MAP_uint32_t u32
#define MAP_int32_t s32
#define MAP_uint64_t u64
#define MAP_int64_t s64

#define MAP_NLA_TYPE(type) MAP_##type

#define MAP_NLA(prefix, type) prefix##_##type

#define MAP_NLA_DIR(direction, type) MAP_NLA(nla_##direction, type)

#define __NLA_GET(type) MAP_NLA_DIR(get, MAP_NLA_TYPE(type))
#define __NLA_PUT(type) MAP_NLA_DIR(put, MAP_NLA_TYPE(type))
