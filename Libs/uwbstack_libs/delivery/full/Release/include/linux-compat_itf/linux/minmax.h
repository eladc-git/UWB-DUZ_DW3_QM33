/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#define min(x, y)                              \
	({                                     \
		typeof(x) _min1 = (x);         \
		typeof(y) _min2 = (y);         \
		_min1 < _min2 ? _min1 : _min2; \
	})

#define max(x, y)                              \
	({                                     \
		typeof(x) _max1 = (x);         \
		typeof(y) _max2 = (y);         \
		_max1 > _max2 ? _max1 : _max2; \
	})
