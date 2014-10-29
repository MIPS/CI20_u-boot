/*
 * JZ4780 CPU
 *
 * Copyright (c) 2014 Imagination Technologies
 * Author: Chris Larsen <chris.larsen@imgtec.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
#include <asm/io.h>
#include <asm/arch/jz4780.h>

extern void tcu_writel(uint32_t, uint32_t);

static void wdt_writew(uint16_t val, uint32_t off)
{
	writew(val, (void __iomem *)WDT_BASE + off);
}

static void wdt_writeb(uint8_t val, uint32_t off)
{
	writeb(val, (void __iomem *)WDT_BASE + off);
}

void _machine_restart(void)
{
	debug("%s, line %d:  %s() entered\n", __FILE__, __LINE__, __func__);

	mdelay(100);

	/* Select the EXTAL as the timer clock input, and make each
	 * WDT clock tick equal 4 ticks of the system clock.
	 */
	wdt_writew(WDT_TCSR_PRESCALE4 | WDT_TCSR_EXT_EN, WDT_TCSR);

	/* Reset the WDT counter to zero. */
	wdt_writew(0, WDT_TCNT);

	/* reset after 4ms
	 *
	 *         1 sec    CONFIG_SYS_EXTAL ticks
	 *  4ms * ------- * ---------------------- = Number of WDT clock ticks
	 *        1000 ms           1 sec
	 *
	 * As noted above the number of system clock ticks have been
	 * effectively multiplied by 4.  All that's left here for the
	 * computation of WDT clock ticks is to divide by 1000
	 * (one thousand).
	 */
	wdt_writew(CONFIG_SYS_EXTAL / 1000, WDT_TDR);

	/* enable wdt clock */
	tcu_writel(TCU_TSSR_WDTSC, TCU_TSCR);

	/* wdt start */
	wdt_writeb(WDT_TCER_TCEN, WDT_TCER);

	while (1) ;
}
