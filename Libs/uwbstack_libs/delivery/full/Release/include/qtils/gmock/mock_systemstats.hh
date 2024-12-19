/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <gmock/gmock.h>

extern "C" {
#include "systemstats.h"
}

class MockSystemstats {
    public:
	static MockSystemstats *instance;

	MockSystemstats()
	{
		instance = this;
		system_stats_start_mocked = true;
		system_stats_stop_mocked = true;
		system_stats_reset_mocked = true;
		system_stats_get_data_mocked = true;
	}
	~MockSystemstats()
	{
		instance = nullptr;
	}

	MOCK_METHOD(void, start, (enum system_pwr_state id));
	MOCK_METHOD(void, stop, (enum system_pwr_state id));
	MOCK_METHOD(enum qerr, reset, ());
	MOCK_METHOD(enum qerr, get_data, (struct system_power_stats_t * sys_power_stats));

	void set_mocked(bool state)
	{
		system_stats_start_mocked = state;
		system_stats_stop_mocked = state;
		system_stats_reset_mocked = state;
		system_stats_get_data_mocked = state;
	}

	/* Properties. */
    public:
	bool system_stats_start_mocked;
	bool system_stats_stop_mocked;
	bool system_stats_reset_mocked;
	bool system_stats_get_data_mocked;
};
