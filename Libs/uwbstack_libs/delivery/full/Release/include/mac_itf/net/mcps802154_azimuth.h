/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * mcps802154_azimuth_compute() - Compute the azimuth angle.
 * @aoa_x_2pi: AoA angle in 2PI on x axis.
 * @aoa_y_2pi: AoA angle in 2PI on y axis which represents the elevation.
 * @aoa_azimuth_2pi: Pointer to azimuth angle to update.
 *
 *           y
 *           ^    M            O, x, y, z: right handed orthonormal basis
 *           |   /:            M: source of signal
 *           |  / :            H: orthogonal projection of M on Oxz plane
 *           | /  :            e: elevation angle
 *           |/\e :            a: azimuth angle
 *         O +--)-:-> x
 *          / \/  :
 *         /,_,\  :
 *        /  a  \ :
 *       /       \:
 *      v         +
 *     z           H
 *
 * AoA on axis x is 0 when the signal source is in the Oyz plane, and positive
 * when on the side where x coordinates are positive.
 *
 * AoA on axis y is 0 when the signal source is in the Oxz plane, and positive
 * when on the side where y coordinates are positive.
 *
 * Elevation is defined as the HOM oriented angle, with H the orthogonal
 * projection of M on Oxz plane. This directly corresponds to the AoA on axis
 * y.
 *
 * Azimuth is defined as the zOH oriented angle.
 *
 * When M is on the y axis, the azimuth angle is arbitrary.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr mcps802154_azimuth_compute(int16_t aoa_x_2pi, int16_t aoa_y_2pi,
				     int16_t *aoa_azimuth_2pi);

#ifdef __cplusplus
}
#endif
