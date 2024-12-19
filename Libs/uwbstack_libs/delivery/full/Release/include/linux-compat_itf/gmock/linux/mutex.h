/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct mutex {
	int val;
	int init;
};

#define DEFINE_MUTEX(name) struct mutex name = { .val = 0, .init = 1 }

void mutex_init(struct mutex *lock);
void mutex_lock(struct mutex *lock);
void mutex_unlock(struct mutex *lock);
void mutex_destroy(struct mutex *lock);

#ifdef __cplusplus
}
#endif
