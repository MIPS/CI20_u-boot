/*
 * JZ4780 PLL setup
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

enum PLLS {
	EXTCLK = 0,
	APLL,
	MPLL,
	EPLL,
	VPLL,
};

/* XPLL control register */
#define XLOCK		(1 << 6)
#define XPLL_ON		(1 << 4)
#define XF_MODE		(1 << 3)
#define XPLLBP		(1 << 1)
#define XPLLEN		(1 << 0)

#define M_N_OD(m,n,od) (((m-1) << 19) | ((n-1) << 13) | ((od-1) << 9))

struct cgu_pll_select {
	uintptr_t reg;
	uint8_t pll;
	uint8_t pll_shift;
};

static void pll_init_one(int pll, int m, int n, int od)
{
	uintptr_t pll_reg = CPM_CPAPCR + ((pll - 1) * 4);

	writel(readl(pll_reg) | M_N_OD(m, n, od) | XPLLEN, pll_reg);

	while (!(readl(pll_reg) & XPLL_ON));
}

static void cpu_mux_select(int pll)
{
	unsigned int clk_ctrl;
	unsigned int selectplls[] = {
		CPM_PLL_SEL_STOP,
		CPM_PLL_SEL_SRC,
		CPM_PLL_SEL_MPLL,
		CPM_PLL_SEL_EPLL
	};

	/* init CPU, L2CACHE, AHB0, AHB2, APB clock */
	clk_ctrl = readl(CPM_CPCCR) & (0xff << 24);
	clk_ctrl |= (CPM_CPCCR_CE_CPU | CPM_CPCCR_CE_AHB0 | CPM_CPCCR_CE_AHB2);
#if CONFIG_SYS_CPU_SPEED >= (1000 * 1000000)
	clk_ctrl |= (12 - 1) << CPM_CPCCR_PDIV_BIT;
#else
	clk_ctrl |= (6 - 1) << CPM_CPCCR_PDIV_BIT;
#endif
	clk_ctrl |= (6 - 1) << CPM_CPCCR_H2DIV_BIT;
	clk_ctrl |= (3 - 1) << CPM_CPCCR_H0DIV_BIT;
	clk_ctrl |= (2 - 1) << CPM_CPCCR_L2DIV_BIT;
	clk_ctrl |= (1 - 1) << CPM_CPCCR_CDIV_BIT;
	writel(clk_ctrl, CPM_CPCCR);

	while (readl(CPM_CPCSR) & (CPM_CPCSR_CDIV_BUSY | CPM_CPCSR_H0DIV_BUSY | CPM_CPCSR_H2DIV_BUSY));

	clk_ctrl = readl(CPM_CPCCR);
	clk_ctrl &= ~(0xff << 24);
	if (pll == APLL)
		clk_ctrl |= CPM_PLL_SEL_SRC << CPM_CPCCR_SEL_SRC_BIT;
	else
		clk_ctrl |= CPM_SRC_SEL_EXCLK << CPM_CPCCR_SEL_SRC_BIT;
	clk_ctrl |= selectplls[pll] << CPM_CPCCR_SEL_CPLL_BIT;
	clk_ctrl |= selectplls[MPLL] << CPM_CPCCR_SEL_H0PLL_BIT;
	clk_ctrl |= selectplls[MPLL] << CPM_CPCCR_SEL_H2PLL_BIT;
	writel(clk_ctrl, CPM_CPCCR);
}

static void ddr_mux_select(int pll)
{
	int selectplls[] = { CPM_DDRCDR_DCS_STOP,
			     CPM_DDRCDR_DCS_SRC,
			     CPM_DDRCDR_DCS_MPLL};

	writel(selectplls[pll] | CPM_DDRCDR_CE_DDR | (CONFIG_SYS_MEM_DIV - 1), CPM_DDCDR);
	while (readl(CPM_DDCDR) & CPM_DDRCDR_DDR_BUSY);

	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_DDR0, CPM_CLKGR0);

	udelay(200000);
}

static void cgu_mux_init(struct cgu_pll_select *cgu, unsigned int num)
{
	int i;
	unsigned int selectplls[] = {0, 1, 2, 3, 2, 6};

	for (i = 0; i < num; i++)
		writel(selectplls[cgu[i].pll] << cgu[i].pll_shift, cgu[i].reg);
}

void pll_init(void)
{
	uint32_t cppcr;
	struct cgu_pll_select cgu_mux[] = {
		{ CPM_MSCCDR,  MPLL, 30 },
		{ CPM_LPCDR,   VPLL, 30 },
		{ CPM_LPCDR1,  VPLL, 30 },
		{ CPM_GPUCDR,  MPLL, 30 },
		{ CPM_HDMICDR, VPLL, 30 },
		{ CPM_I2SCDR,  EPLL, 30 },
		{ CPM_BCHCDR,  MPLL, 30 },
		{ CPM_VPUCDR,  0x1,  30 },
		{ CPM_UHCCDR,  0x3,  30 },
		{ CPM_CIMCDR,  0x1,  31 },
		{ CPM_PCMCDR,  0x5,  29 },
		{ CPM_SSICDR,  0x3,  30 },
	};

	cppcr = readl(CPM_CPPCR);
	cppcr &= ~0xfffff;
	cppcr |= 16 << 8 | 0x20;  //pll stable time set to default--1ms
	writel(cppcr, CPM_CPPCR);

	pll_init_one(APLL, JZ4780_APLL_M, JZ4780_APLL_N, JZ4780_APLL_OD);
	pll_init_one(MPLL, JZ4780_MPLL_M, JZ4780_MPLL_N, JZ4780_MPLL_OD);
	pll_init_one(VPLL, JZ4780_VPLL_M, JZ4780_VPLL_N, JZ4780_VPLL_OD);
	pll_init_one(EPLL, JZ4780_EPLL_M, JZ4780_EPLL_N, JZ4780_EPLL_OD);

	cpu_mux_select(CONFIG_SYS_CPU_PLL);
	ddr_mux_select(CONFIG_SYS_MEM_PLL);
	cgu_mux_init(cgu_mux, ARRAY_SIZE(cgu_mux));
}
