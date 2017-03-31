/*
 * Fastboot mmc storage implementation
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

char* get_dev_part_str(char *par_name)
{
	/*
	 * TODO: Implement this
	 */
	return 0;
}

void storage_info_dump(void)
{
	/*
	 * TODO: Implement this
	 */
}

void cmd_flash_core(USB_STATUS * status, char *par_name,
	unsigned char *Bulk_Data_Buf, int image_size)
{
	/*
	 * TODO: Implement this
	 */
}

void cmd_erase_core(USB_STATUS * status, char *ptnparam, char *par_name)
{
	/*
	 * TODO: Implement this
	 */
}

void cmd_getvar_partition_type(char *buf, char *ptnparam) {
	/*
	 * TODO: Implement this
	 */
}

