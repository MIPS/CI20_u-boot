/*
 * Fastboot mmc storage implementation
 *
 * Copyright (c) 2017 Imagination Technologies
 * Portions Copyright 2014 Broadcom Corporation
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

char dev_part_str[5];

static char* get_dev_part_str(char *par_name)
{
	int i;
	char *partition_names[] = {CONFIG_PARTITION_NAMES};

	memset(dev_part_str, 0, sizeof(dev_part_str));

	for (i = 0; i < ARRAY_SIZE(partition_names); i++) {
		/*
		 * boot.img and recovery.img are stored
		 * to the same ext4 partition named "boot",
		 * unlike other images which are being
		 * flashed directly to the mmc partition.
		 */
		int is_recovery = !strcmp(par_name, FB_PARTITION_RECOVERY) &&
			    !strcmp(FB_PARTITION_BOOT, partition_names[i]);

		if (is_recovery || !strcmp(par_name, partition_names[i])) {
			sprintf(dev_part_str, "0:%d", i + 1);
			break;
		}
	}

	return dev_part_str;
}

void storage_info_dump(void)
{
	char *argv[2];

	argv[0] = "mmc";
	argv[1] = "part";

	do_mmcops(NULL,0,2,argv);
}

static lbaint_t write_to_mmc_partition(struct sparse_storage *info,
		lbaint_t blk, lbaint_t blkcnt, const void *buffer)
{
	char *argv[6];
	char bufferAddr[16];
	char bufferStart[16];
	char bufferSize[16];

	sprintf(bufferAddr, "0x%08X", buffer);
	sprintf(bufferStart, "0x%08X", blk);
	sprintf(bufferSize, "0x%08X", blkcnt);

	argv[0] = "mmc";
	argv[1] = "write";
	argv[2] = bufferAddr;
	argv[3] = bufferStart;
	argv[4] = bufferSize;

	do_mmcops(NULL,0,5,argv);
	return blkcnt;
}

static int update_boot_image(char *par_name, const void *buffer, int image_size)
{
	char bufferAddr[16];
	char bufferSize[16];
	char *dev_part_str = get_dev_part_str(par_name);

	char *argv[6];
	sprintf(bufferAddr, "0x%08X", buffer);
	sprintf(bufferSize, "0x%08X", image_size);

	argv[0] = "ext4write";
	argv[1] = "mmc";
	argv[2] = dev_part_str;
	argv[3] = bufferAddr;

	if (!strcmp(par_name, FB_PARTITION_BOOT)) {
		argv[4] = "/boot.img";
	} else if (!strcmp(par_name, FB_PARTITION_RECOVERY)) {
		argv[4] = "/recovery.img";
	} else {
		return -1;
	}

	argv[5] = bufferSize;

	return do_ext4_write(NULL, 0, 6,argv);
}

void cmd_flash_core(USB_STATUS * status, char *par_name,
	unsigned char *Bulk_Data_Buf, int image_size)
{
	int error = 1;

	if (!strcmp(par_name, FB_PARTITION_BOOT) ||
		!strcmp(par_name, FB_PARTITION_RECOVERY)) {
		if(!check_boot_magic(status, par_name, Bulk_Data_Buf)) {
			debug("%s, line %d: valid boot image being written to \"%s\" "
				"partition\n", __FILE__, __LINE__, par_name);
			error = update_boot_image(par_name, Bulk_Data_Buf, image_size);
		}
	}

	if (!strcmp(par_name, FB_PARTITION_CACHE) ||
		!strcmp(par_name, FB_PARTITION_SYSTEM) ||
		!strcmp(par_name, FB_PARTITION_USERDATA) ||
		!strcmp(par_name, FB_PARTITION_VENDOR)) {

		disk_partition_t info;
		char *dev_part_str = get_dev_part_str(par_name);
		struct block_dev_desc_t	*dev_desc;
		get_device_and_partition("mmc", dev_part_str, &dev_desc, &info, 1);

		if (is_sparse_image(Bulk_Data_Buf)) {
			struct sparse_storage sparse;

			sparse.blksz = info.blksz;
			sparse.start = info.start;
			sparse.size = info.size;
			sparse.write = write_to_mmc_partition;
			sparse.reserve = sparse_reserve;
			sparse.priv = &dev_desc;

			printf("Flashing sparse image at offset " LBAFU "\n",
				sparse.start);
			write_sparse_image(&sparse, par_name, Bulk_Data_Buf,
				image_size);
		} else {
				printf("Flashing raw image\n");
				write_to_mmc_partition(0, info.start, info.size, Bulk_Data_Buf);
		}
		error = 0;
	}

	if(error) {
		printf("Flashing error detected\n");
		tx_status(status, FASTBOOT_REPLY_FAIL);
	} else {
		tx_status(status, FASTBOOT_REPLY_OKAY);
	}
}

void cmd_erase_core(USB_STATUS * status, char *ptnparam, char *par_name)
{
	/*
	 * For now, this command doesn't do any work. Erasing
	 * a partition isn't a prerequisite for rewriting it
	 *
	 * flash_erase(...);
	 */
	if (get_dev_part_str(par_name)) {
		tx_status(status, FASTBOOT_REPLY_OKAY);
	} else {
		tx_status(status, FASTBOOT_REPLY_FAIL "Partition %s not found",
			par_name);
	}
}

void cmd_getvar_partition_type(char *buf, char *ptnparam) {
	if (!strcmp(FB_PARTITION_BOOT, ptnparam) ||
		!strcmp(FB_PARTITION_RECOVERY, ptnparam)) {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
			"raw");
	} else if (!strcmp(FB_PARTITION_SYSTEM, ptnparam) ||
		!strcmp(FB_PARTITION_VENDOR, ptnparam)) {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
			"ext4");
	} else if (!strcmp(FB_PARTITION_CACHE, ptnparam) ||
		!strcmp(FB_PARTITION_USERDATA, ptnparam)) {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
			"f2fs");
	} else {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
			"UNKNOWN");
	}
}

