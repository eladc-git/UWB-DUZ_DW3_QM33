/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-Qorvo-2
 */

#include <nrfx_nvmc.h>
#include <qerr.h>
#include <qirq.h>

enum qerr qflash_write(uint32_t dst_addr, void *src_addr, uint32_t size)
{
	uint32_t size32 = size / sizeof(uint32_t);
	unsigned int key;

	/* Align on words */
	if (size32 % 2 != 0) {
		size32 += 1;
	}

	key = qirq_lock();
	nrfx_nvmc_page_erase(dst_addr);
	nrfx_nvmc_words_write(dst_addr, src_addr, size32);
	qirq_unlock(key);

	return QERR_SUCCESS;
}
