/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#ifndef MOCK_QPROBE_HH
#define MOCK_QPROBE_HH

#include <gmock/gmock.h>

extern "C" {
#include "qprobe.h"
}

class MockQprobe {
    public:
	static MockQprobe *instance;

	MockQprobe()
	{
		instance = this;
		qprobe_start_mocked = true;
		qprobe_stop_mocked = true;
		qprobe_reset_mocked = true;
		qprobe_get_data_mocked = true;
	}
	~MockQprobe()
	{
		instance = nullptr;
	}

	MOCK_METHOD(void, start, (enum qprobe_id id));
	MOCK_METHOD(void, stop, (enum qprobe_id id));
	MOCK_METHOD(enum qerr, reset, ());
	MOCK_METHOD(enum qerr, get_data,
		    (struct qprobe_runtime_t * runtime, struct qprobe_config_t *config));

	void set_mocked(bool state)
	{
		qprobe_start_mocked = state;
		qprobe_stop_mocked = state;
		qprobe_reset_mocked = state;
		qprobe_get_data_mocked = state;
	}

	/* Properties. */
    public:
	bool qprobe_start_mocked;
	bool qprobe_stop_mocked;
	bool qprobe_reset_mocked;
	bool qprobe_get_data_mocked;
};

#endif // MOCK_QPROBE_HH
