/*
 * JZ4780 common routines
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

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/jz4780.h>
#include <spl.h>

#ifdef CONFIG_SPL_BUILD

/* Pointer to as well as the global data structure for SPL */
DECLARE_GLOBAL_DATA_PTR;
gd_t gdata __attribute__ ((section(".data")));

extern void pll_init(void);
extern void sdram_init(void);
extern void enable_caches(void);

void board_init_f(ulong dummy)
{
#ifdef CONFIG_JTAG
	/* enable JTAG */
	writel(3 << 30, GPIO_PXINTC(0));
	writel(3 << 30, GPIO_PXMASKC(0));
	writel(3 << 30, GPIO_PXPAT1C(0));
	writel(3 << 30, GPIO_PXPAT0C(0));
#endif

	/* Set global data pointer */
	gd = &gdata;

	timer_init();
	pll_init();

	/* UART0 init */
	writel(0x9, GPIO_PXINTC(5));
	writel(0x9, GPIO_PXMASKC(5));
	writel(0x9, GPIO_PXPAT1C(5));
	writel(0x9, GPIO_PXPAT0C(5));
	writel(0x9, GPIO_PXPENC(5));
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_UART0, CPM_CLKGR0);

	/* UART1 init */
	/* TODO: pins */
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_UART1, CPM_CLKGR0);

	/* UART2 init */
	/* TODO: pins */
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_UART2, CPM_CLKGR0);

#ifndef CONFIG_JTAG
	/* UART3 init */
	writel(1 << 12, GPIO_PXINTC(3));
	writel(1 << 12, GPIO_PXMASKS(3));
	writel(1 << 12, GPIO_PXPAT1S(3));
	writel(1 << 12, GPIO_PXPAT0C(3));
	writel(3 << 30, GPIO_PXINTC(0));
	writel(3 << 30, GPIO_PXMASKC(0));
	writel(3 << 30, GPIO_PXPAT1C(0));
	writel(1 << 30, GPIO_PXPAT0C(0));
	writel(1 << 31, GPIO_PXPAT0S(0));
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_UART3, CPM_CLKGR0);
#endif

	/* UART4 init */
	writel(0x100400, GPIO_PXINTC(2));
	writel(0x100400, GPIO_PXMASKC(2));
	writel(0x100400, GPIO_PXPAT1S(2));
	writel(0x100400, GPIO_PXPAT0C(2));
	writel(0x100400, GPIO_PXPENC(2));
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR1_UART4, CPM_CLKGR1);

#ifdef CONFIG_SPL_SERIAL_SUPPORT
	preloader_console_init();
#endif
	sdram_init();
	enable_caches();

	/* Clear the BSS */
	memset(__bss_start, 0, (char *)&__bss_end - __bss_start);

	board_init_r(NULL, 0);
}

extern void flush_cache_all(void);

void __noreturn jump_to_image_no_args(struct spl_image_info *spl_image)
{
	typedef void __noreturn(*image_entry_noargs_t) (void);

	image_entry_noargs_t image_entry =
	    (image_entry_noargs_t) spl_image->entry_point;

	flush_cache_all();

	debug("image entry point: 0x%X\n", spl_image->entry_point);
	image_entry();
}

#endif /* CONFIG_SPL_BUILD */

/*
 * U-Boot common functions
 */

void enable_interrupts(void)
{
}

int disable_interrupts(void)
{
	return 0;
}

phys_size_t initdram(int board_type)
{
	return sdram_size(0) + sdram_size(1);
}
