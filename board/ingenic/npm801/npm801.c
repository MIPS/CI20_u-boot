/*
 * NPM801 setup code
 *
 * Copyright (c) 2013 Imagination Technologies
 * Copyright (c) 2017 Mips Tech LLC
 * Author: Dragan Cecavac <dragan.cecavac@mips.com>
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

#include <common.h>
#include <asm/arch/efuse.h>
#include <asm/arch/jz4780.h>
#include <asm/jz_mmc.h>

struct npm801_otp {
	uint32_t serial_number;
	uint32_t date;
	char manufacturer[2];
	uint8_t mac[6];
} __packed;

int misc_init_r(void)
{
	uint32_t cpccr, ahb2_div;
	struct npm801_otp otp;
	char manufacturer[3];

	/* read the board OTP data */
	cpccr = readl(CPM_CPCCR);
	ahb2_div = ((cpccr & CPM_CPCCR_H2DIV_MASK) >> CPM_CPCCR_H2DIV_BIT) + 1;
	jz4780_efuse_init(CONFIG_SYS_MEM_SPEED / ahb2_div);
	jz4780_efuse_read(0x18, 16, (uint8_t *)&otp);

	/* put other board information into the environment */
	setenv_ulong("serial#", otp.serial_number);
	setenv_ulong("board_date", otp.date);
	memcpy(manufacturer, otp.manufacturer, 2);
	manufacturer[2] = 0;
	setenv("board_mfr", manufacturer);

	return 0;
}

int board_mmc_init(bd_t *bd)
{
	uint32_t msc_cdr;

	writel(0x30f00000, GPIO_PXINTC(4));
	writel(0x30f00000, GPIO_PXMASKC(4));
	writel(0x30f00000, GPIO_PXPAT1C(4));
	writel(0x30f00000, GPIO_PXPAT0C(4));

	msc_cdr = CONFIG_SYS_MEM_SPEED / 24000000 / 2 - 1;
	writel(msc_cdr | CPM_MSCCDR_CE | (1 << 31), CPM_MSCCDR);
	while (readl(CPM_MSCCDR) & CPM_MSCCDR_MSC_BUSY);

	jz_mmc_init((msc_cdr + 1) * 2);
	return 0;
}

/* U-Boot common routines */
int checkboard(void)
{
	return 0;
}

#ifdef CONFIG_SPL_BUILD

void spl_board_init(void)
{
}

#endif /* CONFIG_SPL_BUILD */
