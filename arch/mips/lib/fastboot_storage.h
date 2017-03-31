/*
 * Fastboot storage interface
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

#ifndef FASTBOOT_STORAGE_H
#define FASTBOOT_STORAGE_H


#include <common.h>
#include <version.h>
#include <fastboot.h>
#include <asm/arch/jz4780.h>
#include <image-sparse.h>

#define TYPE_DEVICE          1
#define TYPE_CONFIGURATION   2
#define TYPE_STRING          3
#define TYPE_INTERFACE       4
#define TYPE_ENDPOINT        5

#include "usb_descriptors.h"
#include <asm/arch/jz4780_otg.h>

void storage_info_dump(void);

lbaint_t sparse_reserve(struct sparse_storage *info,
		lbaint_t blk, lbaint_t blkcnt);

void cmd_flash_core(USB_STATUS * status, char *par_name,
	unsigned char *Bulk_Data_Buf, int image_size);

void cmd_erase_core(USB_STATUS * status, char *ptnparam, char *par_name);

void cmd_getvar_partition_type(char *buf, char *ptnparam);

#endif
