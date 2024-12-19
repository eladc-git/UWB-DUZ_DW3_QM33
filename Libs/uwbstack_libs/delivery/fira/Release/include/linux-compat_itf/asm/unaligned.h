/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#ifndef COMPAT_ASM_UNALIGNED_H
#define COMPAT_ASM_UNALIGNED_H

#include <stdint.h>

static inline void put_unaligned_le16(uint16_t v, void *p)
{
	uint8_t *pc = (uint8_t *)p;
	*pc++ = v;
	*pc++ = v >> 8;
}

static inline void put_unaligned_le24(uint32_t v, void *p)
{
	uint8_t *pc = (uint8_t *)p;
	*pc++ = v;
	*pc++ = v >> 8;
	*pc++ = v >> 16;
}

static inline void put_unaligned_le32(uint32_t v, void *p)
{
	uint8_t *pc = (uint8_t *)p;
	*pc++ = v;
	*pc++ = v >> 8;
	*pc++ = v >> 16;
	*pc++ = v >> 24;
}

static inline void put_unaligned_le64(uint64_t v, void *p)
{
	uint8_t *pc = (uint8_t *)p;
	*pc++ = v;
	*pc++ = v >> 8;
	*pc++ = v >> 16;
	*pc++ = v >> 24;
	*pc++ = v >> 32;
	*pc++ = v >> 40;
	*pc++ = v >> 48;
	*pc++ = v >> 56;
}

static inline void put_unaligned_be16(uint16_t v, void *p)
{
	uint8_t *pc = (uint8_t *)p;
	*pc++ = v >> 8;
	*pc++ = v;
}

static inline void put_unaligned_be32(uint32_t v, void *p)
{
	uint8_t *pc = (uint8_t *)p;
	*pc++ = v >> 24;
	*pc++ = v >> 16;
	*pc++ = v >> 8;
	*pc++ = v;
}

static inline void put_unaligned_be64(uint64_t v, void *p)
{
	uint8_t *pc = (uint8_t *)p;
	*pc++ = v >> 56;
	*pc++ = v >> 48;
	*pc++ = v >> 40;
	*pc++ = v >> 32;
	*pc++ = v >> 24;
	*pc++ = v >> 16;
	*pc++ = v >> 8;
	*pc++ = v;
}

static inline uint16_t get_unaligned_le16(const void *p)
{
	uint8_t *pc = (uint8_t *)p;
	return (unsigned int)pc[0] | ((unsigned int)pc[1] << 8);
}

static inline uint32_t get_unaligned_le24(const void *p)
{
	uint8_t *pc = (uint8_t *)p;
	return (unsigned int)pc[0] | ((unsigned int)pc[1] << 8) | ((unsigned int)pc[2] << 16);
}

static inline uint32_t get_unaligned_le32(const void *p)
{
	uint8_t *pc = (uint8_t *)p;
	return (unsigned int)pc[0] | ((unsigned int)pc[1] << 8) | ((unsigned int)pc[2] << 16) |
	       ((unsigned int)pc[3] << 24);
}

static inline uint64_t get_unaligned_le64(const void *p)
{
	uint8_t *pc = (uint8_t *)p;
	return (uint64_t)get_unaligned_le32(pc + 4) << 32 | get_unaligned_le32(pc);
}

static inline uint32_t get_unaligned_be16(const void *p)
{
	uint8_t *pc = (uint8_t *)p;
	return (unsigned int)pc[1] | ((unsigned int)pc[0] << 8);
}

static inline uint32_t get_unaligned_be32(const void *p)
{
	uint8_t *pc = (uint8_t *)p;
	return (unsigned int)pc[3] | ((unsigned int)pc[2] << 8) | ((unsigned int)pc[1] << 16) |
	       ((unsigned int)pc[0] << 24);
}

static inline uint64_t get_unaligned_be64(const void *p)
{
	uint8_t *pc = (uint8_t *)p;
	return ((unsigned long long)pc[7] | ((unsigned long long)pc[6] << 8) |
		((unsigned long long)pc[5] << 16) | ((unsigned long long)pc[4] << 24) |
		((unsigned long long)pc[3] << 32) | ((unsigned long long)pc[2] << 40) |
		((unsigned long long)pc[1] << 48) | ((unsigned long long)pc[0] << 56));
}
#endif /* COMPAT_ASM_UNALIGNED_H */
