/*
 * CI20 setup code
 *
 * Copyright (c) 2013 Imagination Technologies
 * Author: Paul Burton <paul.burton@imgtec.com>
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
#include <nand.h>
#include <net.h>
#include <netdev.h>
#include <asm/arch/efuse.h>
#include <asm/arch/jz4780.h>
#include <asm/arch/nand.h>
#include <asm/jz_mmc.h>

struct ci20_otp {
	uint32_t serial_number;
	uint32_t date;
	char manufacturer[2];
	uint8_t mac[6];
} __packed;

int board_early_init_f(void)
{
	/* SYS_POWER_IND high (LED blue, VBUS on) */
	gpio_direction_output(32 * 5 + 15, 1);

	return 0;
}

int misc_init_r(void)
{
	uint32_t cpccr, ahb2_div;
	struct ci20_otp otp;
	char manufacturer[3];

	/* read the board OTP data */
	cpccr = readl(CPM_CPCCR);
	ahb2_div = ((cpccr & CPM_CPCCR_H2DIV_MASK) >> CPM_CPCCR_H2DIV_BIT) + 1;
	jz4780_efuse_init(CONFIG_SYS_MEM_SPEED / ahb2_div);
	jz4780_efuse_read(0x18, 16, (uint8_t *)&otp);

	/* set MAC address */
	if (!is_valid_ether_addr(otp.mac)) {
		/* no MAC assigned, generate one from the unique chip ID */
		jz4780_efuse_read(0x8, 4, &otp.mac[0]);
		jz4780_efuse_read(0x12, 2, &otp.mac[4]);
		otp.mac[0] = (otp.mac[0] | 0x02) & ~0x01;
	}
	eth_setenv_enetaddr("ethaddr", otp.mac);

	/* put other board information into the environment */
	setenv_ulong("serial#", otp.serial_number);
	setenv_ulong("board_date", otp.date);
	memcpy(manufacturer, otp.manufacturer, 2);
	manufacturer[2] = 0;
	setenv("board_mfr", otp.manufacturer);

	return 0;
}

int board_nand_init(struct nand_chip *nand)
{
	/* setup pins */
	writel(0x002c00ff, GPIO_PXINTC(0));
	writel(0x002c00ff, GPIO_PXMASKC(0));
	writel(0x002c00ff, GPIO_PXPAT1C(0));
	writel(0x002c00ff, GPIO_PXPAT0C(0));
	writel(0x002c00ff, GPIO_PXPENS(0));
	writel(0x00000003, GPIO_PXINTC(1));
	writel(0x00000003, GPIO_PXMASKC(1));
	writel(0x00000003, GPIO_PXPAT1C(1));
	writel(0x00000003, GPIO_PXPAT0C(1));
	writel(0x00000003, GPIO_PXPENS(1));

	/* FRB0_N */
	gpio_direction_input(32 * 0 + 20);

	/* disable write protect */
	gpio_direction_output(32 * 5 + 22, 1);

	return jz4780_nand_init(nand);
}

#if !defined(CONFIG_SPL_BUILD) || defined(CONFIG_SPL_MMC_SUPPORT)

int board_mmc_init(bd_t *bd)
{
	uint32_t msc_cdr;

	/* setup MSC1 clock */
	msc_cdr = CONFIG_SYS_MEM_SPEED / 24000000 / 2 - 1;
	writel(msc_cdr | CPM_MSCCDR_CE, CPM_MSCCDR1);
	while (readl(CPM_MSCCDR1) & CPM_MSCCDR_MSC_BUSY);

	/* setup MSC1 pins */
	writel(0x30f00000, GPIO_PXINTC(4));
	writel(0x30f00000, GPIO_PXMASKC(4));
	writel(0x30f00000, GPIO_PXPAT1C(4));
	writel(0x30f00000, GPIO_PXPAT0S(4));

	jz_mmc_init((msc_cdr + 1) * 2);
	return 0;
}

#endif

#ifdef CONFIG_DRIVER_DM9000

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_NAND
	/* setup pins (some already setup for NAND) */
	writel(0x04030000, GPIO_PXINTC(0));
	writel(0x04030000, GPIO_PXMASKC(0));
	writel(0x04030000, GPIO_PXPAT1C(0));
	writel(0x04030000, GPIO_PXPAT0C(0));
	writel(0x04030000, GPIO_PXPENS(0));
#else
	/* setup pins (as above +NAND CS +RD/WE +SDx +SAx) */
	writel(0x0dff00ff, GPIO_PXINTC(0));
	writel(0x0dff00ff, GPIO_PXMASKC(0));
	writel(0x0dff00ff, GPIO_PXPAT1C(0));
	writel(0x0dff00ff, GPIO_PXPAT0C(0));
	writel(0x0dff00ff, GPIO_PXPENS(0));
	writel(0x00000003, GPIO_PXINTC(1));
	writel(0x00000003, GPIO_PXMASKC(1));
	writel(0x00000003, GPIO_PXPAT1C(1));
	writel(0x00000003, GPIO_PXPAT0C(1));
	writel(0x00000003, GPIO_PXPENS(1));
#endif

	/* enable clocks */
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_MAC, CPM_CLKGR0);
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_NEMC, CPM_CLKGR0);

	/* enable power (PB25) */
	gpio_direction_output(32 * 1 + 25, 1);

	/* reset (PF12) */
	gpio_direction_output(32 * 5 + 12, 0);
	udelay(10000);
	gpio_set(32 * 5 + 12, 1);
	udelay(10000);

	return dm9000_initialize(bis);
}

#endif /* CONFIG_DRIVER_DM9000 */

/* U-Boot common routines */
int checkboard(void)
{
	puts("Board: ci20 (Ingenic XBurst JZ4780 SoC)\n");
	return 0;
}

#ifdef CONFIG_SPL_BUILD

void spl_board_init(void)
{
}

#endif /* CONFIG_SPL_BUILD */
