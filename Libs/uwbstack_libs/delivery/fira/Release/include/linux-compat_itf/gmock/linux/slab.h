/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

enum gfp_e {
	GFP_KERNEL,
};

void kmemzero_explicit(void *s, size_t count);
void *kmalloc(size_t size, int flags);
void *kzalloc(size_t size, int flags);
void kfree(void *ptr);
void kfree_sensitive(void *ptr);
void *krealloc(void *ptr, size_t size, int flags);

#ifdef __cplusplus
}
#endif
