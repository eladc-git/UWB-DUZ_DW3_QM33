/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "llhw.h"
#include "llhw_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * llhw_get_dtu_time() - get current DTU time.
 * @uwb: the UWB device.
 *
 * This function compute the current DTU time, based on "UWB" time,
 * at 15.6MHz rate.
 *
 * Return: the current DTU time.
 */
uint32_t llhw_get_dtu_time(const struct uwb_chip *uwb);

/**
 * llhw_get_time_us() - get current time in microseconds.
 *
 * Return: the current time in microseconds.
 */
int64_t llhw_get_time_us(void);

/**
 * llhw_systime_to_dtu() - compute current DTU time from SysTime.
 * @uwb: the UWB device.
 * @systime: the UWB transceiver SysTime register value to convert to DTU.
 * @dtu_near: a DTU time which must be in the past relative to systime, at less
 * than half the SysTime rollover period.
 *
 * Return: the corresponding DTU time.
 */
uint32_t llhw_systime_to_dtu(const struct uwb_chip *uwb, uint32_t systime, uint32_t dtu_near);

/**
 * llhw_systime_rctu_to_dtu() - compute current DTU time from RCTU.
 * @uwb: the UWB device.
 * @timestamp_rctu: The UWB transceiver RX_STAMP register value in RCTU to
 * convert to DTU.
 * The RCTU, Ranging Counter Time Unit, is approximately 15.65 picoseconds long.
 *
 * Return: The corresponding DTU time.
 */
uint32_t llhw_systime_rctu_to_dtu(const struct uwb_chip *uwb, uint64_t timestamp_rctu);

/**
 * llhw_resync_dtu_systime() - resync DTU time and SysTime
 * @uwb: the UWB device.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr llhw_resync_dtu_systime(struct uwb_chip *uwb);

/**
 * llhw_time_us_to_dtu() - convert UWB time in microseconds to DTU time
 * @runtime: the Low-level hardware runtime instance
 * @timestamp_us: time in us
 *
 * Formula:
 * dtu = (time - time0) * N / D + dtu0
 * Where:
 * N = DTU_FREQ = 15600000
 * D = 1000000
 *
 * N/D = 15600000/1000000 = 156/10
 * dtu0 always 0.
 *
 * Return: driver DTU time
 */
static inline uint32_t llhw_time_us_to_dtu(const struct uwb_runtime *runtime, int64_t timestamp_us)
{
	timestamp_us -= runtime->time_zero_us;
	return (uint32_t)(timestamp_us * (DTU_FREQ / 100000) / 10);
}

/**
 * llhw_dtu_to_systime() - compute UWB SysTime from DTU time
 * @runtime: the Low-level hardware runtime instance
 * @dtu: the DTU timestamp to convert to SysTime
 *
 * Return: the value to write to SysTime register
 */
static inline uint32_t llhw_dtu_to_systime(const struct uwb_runtime *runtime, uint32_t dtu)
{
	const int N = DTU_PER_SYS_POWER;
	uint32_t dtu_sync = runtime->dtu_sync;
	uint32_t systime_sync = runtime->systime_sync;
	return ((dtu - dtu_sync) << N) + systime_sync;
}

/**
 * llhw_dtu_to_pac() - convert a time in DTU (Device Time Unit) into PAC
 * (Preamble Acquisition Chunk).
 * @uwb: the UWB device.
 * @time_dtu: time in DTU.
 *
 * Return: time in PAC.
 */
int llhw_dtu_to_pac(const struct uwb_chip *uwb, int time_dtu);

/**
 * llhw_pac_to_dtu() - convert a time in PAC (Preamble Acquisition Chunk) into
 * DTU (Device Time Unit).
 * @uwb: the UWB device.
 * @time_pac: time in PAC.
 *
 * Return: time in DTU.
 */
int llhw_pac_to_dtu(const struct uwb_chip *uwb, int time_pac);

#ifdef __cplusplus
}
#endif
