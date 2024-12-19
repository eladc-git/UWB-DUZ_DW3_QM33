/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdbool.h> /* IWYU pragma: keep */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct tests_override_kmalloc {
	int allow;
};

extern struct tests_override_kmalloc *tests_override_kmalloc;

bool tests_override_kmalloc_nomem(void);
void tests_override_reset(void);

struct tests_override_ktime {
	uint64_t boottime_ns;
};

extern struct tests_override_ktime *tests_override_ktime;

uint64_t tests_override_ktime_get_boottime(void);

void tests_assert_failed(bool expression, const char *fmt, ...);

#ifdef __cplusplus
}
#endif
