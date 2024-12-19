/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

/**
 * sat_fp() - Saturate the range of fixed-point
 * @x: fixed-point value on int32_t.
 *
 * Return: value saturate to int16_t range
 */
static inline int16_t sat_fp(int32_t x)
{
	if (x > INT16_MAX)
		return INT16_MAX;
	else if (x < INT16_MIN)
		return INT16_MIN;
	return x;
}

/**
 * map_q11_to_2pi() - Map a Fixed Point angle to an signed 16 bit integer
 * @x: angle as Q11 fixed_point value in range [-PI, PI]
 *
 * Return: the angle mapped to [INT16_MIN, INT16_MAX]
 */
static inline int16_t map_q11_to_2pi(int16_t x)
{
	/* Same as round(M_PI * (1 << 11)). */
	const int16_t pi = 6434;

	int32_t temp = (int32_t)x * INT16_MAX;
	temp /= pi;
	return sat_fp(temp);
}

/**
 * convert_q26_to_q10() - Convert from Q26 to Q10, saturate when exceeds int16_t.
 * @x: Value in Q26 format.
 *
 * Return: Value converted to Q10.
 */
int16_t convert_q26_to_q10(int16_t x);

/**
 * convert_q8_to_q1() - Convert from Q8.8 to Q7.1, round by adding 0.5 ulp.
 * @val_q8: value in format Q8.8.
 *
 * Return: value converted to format Q7.1.
 */
uint8_t convert_q8_to_q1(uint16_t val_q8);
