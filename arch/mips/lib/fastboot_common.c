/*
 * Fastboot storage interface common implementation
 *
 * Copyright (c) 2017 Imagination Technologies
 * Author: Dragan Cecavac <dragan.cecavac@imgtec.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include "fastboot_storage.h"

lbaint_t sparse_reserve(struct sparse_storage *info,
		lbaint_t blk, lbaint_t blkcnt)
{
	return blkcnt;
}

int check_boot_magic(USB_STATUS * status, char *par_name, unsigned char *buffer)
{
	if (!strcmp(par_name, FB_KERNEL_DTB))
		return 0;

	if (!strcmp(par_name, FB_PARTITION_BOOT) ||
		!strcmp(par_name, FB_PARTITION_RECOVERY)) {

		if (memcmp((void *)buffer, FASTBOOT_BOOT_MAGIC,
			FASTBOOT_BOOT_MAGIC_SIZE)) {
			tx_status(status, FASTBOOT_REPLY_FAIL
				"image is not a boot image");
			return -1;
		}

		return 0;
	}

	return -1;
}

