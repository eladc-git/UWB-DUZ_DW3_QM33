/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#ifdef __KERNEL__
#include <asm/byteorder.h>
#include <linux/string.h>
#include <linux/types.h>
#else
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __KERNEL__
/*
 * CBOR is used in kernel to format/parse params in UCI messages when driver
 * is forwarding its Mac commands over UCI (to QM35)
 */
#define be16toh(x) be16_to_cpu(x)
#define be32toh(x) be32_to_cpu(x)
#define be64toh(x) be64_to_cpu(x)
#define htobe16(x) cpu_to_be16(x)
#define htobe32(x) cpu_to_be32(x)
#define htobe64(x) cpu_to_be64(x)

#elif defined(__linux__)
/*
 * CBOR is used on Linux/Android when Mac UCI commands are enabled
 * (allows to control the Mac from the test framework)
 */
#include <endian.h>

#elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) && defined(__GNUC__) && (__GNUC__ > 4)
/*
 * CBOR is used on Embedded to format/parse params in UCI messages used to
 * control the Mac
 */

#define be16toh(x) __builtin_bswap16(x)
#define be32toh(x) __builtin_bswap32(x)
#define be64toh(x) __builtin_bswap64(x)
#define htobe16(x) __builtin_bswap16(x)
#define htobe32(x) __builtin_bswap32(x)
#define htobe64(x) __builtin_bswap64(x)

#else
#error Platform is not supported, endianess operations need to be provided
#endif

#ifdef __cplusplus
}
#endif
