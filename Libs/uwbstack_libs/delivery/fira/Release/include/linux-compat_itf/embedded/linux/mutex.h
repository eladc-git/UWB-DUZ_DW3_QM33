/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#ifndef COMPAT_LINUX_MUTEX_H
#define COMPAT_LINUX_MUTEX_H

#include "qassert.h"
#include "qmutex.h"

#define DEFINE_MUTEX(name) struct mutex name = { .mtx = NULL }

struct mutex {
	struct qmutex *mtx;
};

static inline void mutex_init(struct mutex *lock)
{
	lock->mtx = qmutex_init();
	QASSERT(lock->mtx);
}

static inline void mutex_lock(struct mutex *lock)
{
	QASSERT(lock);
	if (lock->mtx == NULL)
		mutex_init(lock);
	QASSERT(lock->mtx != NULL);
	qmutex_lock(lock->mtx, QOSAL_WAIT_FOREVER);
}

static inline void mutex_unlock(struct mutex *lock)
{
	QASSERT(lock && lock->mtx != NULL);
	qmutex_unlock(lock->mtx);
}

static inline void mutex_destroy(struct mutex *lock)
{
	QASSERT(lock && lock->mtx != NULL);
	qmutex_deinit(lock->mtx);
	lock->mtx = NULL;
}

#endif /* COMPAT_LINUX_MUTEX_H */
