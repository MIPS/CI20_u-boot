/*
 * JZ4780 DDR initialization
 *
 * Copyright (c) 2013 Imagination Technologies
 * Author: Paul Burton <paul.burton@imgtec.com>
 *
 * Based on spl/common/{jz4780_ddr,jz_ddr3_init}.c from X-Boot
 * Copyright (c) 2006-2013 Ingenic Semiconductor
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

#ifdef __CONFIG_CI20_H__
/* Configured for Ci20 - we can get the board revision from here */
extern int ci20_revision(void);
#endif /* ci20 */

#ifdef CONFIG_SYS_DDR3_H5TQ2G83CFR
#include "sdram/H5TQ2G83CFR.h"
#endif
#ifdef CONFIG_SYS_DDR3_K4B2G0846Q
#include "sdram/K4B2G0846Q.h"
#endif

static const uint32_t get_mem_clk(void)
{
	uint32_t mpll_out;
	mpll_out = (uint64_t)CONFIG_SYS_EXTAL * JZ4780_MPLL_M / (JZ4780_MPLL_N * JZ4780_MPLL_OD);
	return mpll_out / CONFIG_SYS_MEM_DIV;
}

uint32_t sdram_size(int cs)
{
	uint32_t dw, banks, size = 0;

	dw = DDR_DW32 ? 4 : 2;
	banks = DDR_BANK8 ? 8 : 4;

	if ((cs == 0) && DDR_CS0EN) {
		size = (1 << (DDR_ROW + DDR_COL)) * dw * banks;
		if (DDR_CS1EN && (size > 0x20000000))
			size = 0x20000000;
	} else if ((cs == 1) && DDR_CS1EN) {
		size = (1 << (DDR_ROW + DDR_COL)) * dw * banks;
	}

	return size;
}

static void ddr_cfg_init(void)
{
	uint32_t ddrc_cfg, tmp;

	tmp = DDR_CL - 1;
	if (tmp < 0)
		tmp = 0;
	if (tmp > 4)
		tmp = 4;

	ddrc_cfg =
		DDRC_CFG_TYPE_DDR3 | DDRC_CFG_IMBA | DDR_DW32 | DDRC_CFG_MPRT |
		(tmp | 0x8) << 2 |
		(DDR_ROW - 12) << 11  | (DDR_COL - 8) << 8 |
		DDR_CS0EN << 6 | DDR_BANK8 << 1 |
		(DDR_ROW - 12) << 27 | (DDR_COL - 8) << 24 |
		DDR_CS1EN << 7 | DDR_BANK8 << 23;

	if (DDR_BL > 4)
		ddrc_cfg |= 1 << 21;

	writel(ddrc_cfg, DDRC_CFG);
}

#define DDRP_PTR0_tDLLSRST  	50		/* 50ns */
#define DDRP_tDLLLOCK 	    	5120 		/* 5.12us */
#define DDRP_PTR0_ITMSRST_8 	8		/* 8tck */
#define DDRP_PTR1_DINIT0_DDR3	500 * 1000 	/* 500us */
#define DDRP_PTR2_DINIT2_DDR3 	200 * 1000	/* 200us */
#define DDRP_PTR2_DINIT3_DDR3	512 		/* 512 tck */

static void ddr_phy_init(const struct jz4780_ddr_config *ddr_config)
{
	register unsigned int tmp;
	unsigned int count = 0, i;

	writel(DDRP_DCR_TYPE_DDR3 | (DDR_BANK8 << 3), DDRP_DCR);

	writel(ddr_config->mr0, DDRP_MR0);
	writel(ddr_config->mr1, DDRP_MR1);
	writel(0, DDRP_ODTCR);
	writel(0, DDRP_MR2);

	writel(ddr_config->ptr0, DDRP_PTR0);
	writel(ddr_config->ptr1, DDRP_PTR1);
	writel(ddr_config->ptr2, DDRP_PTR2);

	writel(ddr_config->dtpr0, DDRP_DTPR0);
	writel(ddr_config->dtpr1, DDRP_DTPR1);
	writel(ddr_config->dtpr2, DDRP_DTPR2);

	writel(DDRP_PGCR_DQSCFG |
		7 << DDRP_PGCR_CKEN_BIT |
		2 << DDRP_PGCR_CKDV_BIT |
		(DDR_CS0EN | DDR_CS1EN << 1) << DDRP_PGCR_RANKEN_BIT |
		DDRP_PGCR_ZCKSEL_32 |
		DDRP_PGCR_PDDISDX, DDRP_PGCR);

	for (i = 0; i < 8; i++)
		writel(readl(DDRP_DXGCR(i)) & ~(0x3 << 9), DDRP_DXGCR(i));

	while (readl(DDRP_PGSR) != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE)) {
		if (readl(DDRP_PGSR) == 0x1f)
			break;
		if (count++ == 10000)
			hang();
	}

	/* DQS extension and early set to 1 */
	tmp = readl(DDRP_DSGCR);
	tmp &= ~(0x7E << 4);
	tmp |= 0x12 << 4;
	writel(tmp, DDRP_DSGCR);

	/* 500 pull up and 500 pull down */
	tmp = readl(DDRP_DXCCR);
	tmp &= ~(0xFF << 4);
	tmp |= 0xC4 << 4;
	writel(tmp, DDRP_DXCCR);

	/* Initialise phy */
	writel(DDRP_PIR_INIT | DDRP_PIR_DRAMINT | DDRP_PIR_DRAMRST, DDRP_PIR);

	count = 0;
	while (readl(DDRP_PGSR) != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE)) {
		if (readl(DDRP_PGSR) == 0x1f)
			break;
		if (count++ == 20000)
			hang();
	}

	count = 0;
	writel(DDRP_PIR_INIT | DDRP_PIR_QSTRN, DDRP_PIR);
	while (readl(DDRP_PGSR) != (DDRP_PGSR_IDONE | DDRP_PGSR_DLDONE | DDRP_PGSR_ZCDONE | DDRP_PGSR_DIDONE | DDRP_PGSR_DTDONE)) {
		if (count++ == 500000) {
			if (readl(DDRP_PGSR) & (DDRP_PGSR_DTDONE | DDRP_PGSR_DTERR | DDRP_PGSR_DTIERR))
				hang();
			count = 0;
		}
	}

	/* Override impedence */
	tmp = readl(DDRP_ZQXCR0(0));
	tmp &= ~0x3ff;
	tmp |= (CONFIG_SYS_DDR3PHY_PULLUP_IMPEDANCE & 0x1f) <<
		DDRP_ZQXCR_PULLUP_IMPE_BIT;
	tmp |= (CONFIG_SYS_DDR3PHY_PULLDOWN_IMPEDANCE & 0x1f) <<
		DDRP_ZQXCR_PULLDOWN_IMPE_BIT;
	tmp |= DDRP_ZQXCR_ZDEN;
	writel(tmp, DDRP_ZQXCR0(0));
}

#define BIT(bit) ((bit % 4) * 8)
#define MASK(bit) (0x1f << BIT(bit))

static void remap_swap(int a, int b)
{
	uint32_t remmap[2], tmp[2];

	remmap[0] = readl(DDRC_REMMAP(a / 4 + 1));
	remmap[1] = readl(DDRC_REMMAP(b / 4 + 1));

	tmp[0] = (remmap[0] & MASK(a)) >> BIT(a);
	tmp[1] = (remmap[1] & MASK(b)) >> BIT(b);

	remmap[0] &= ~MASK(a);
	remmap[1] &= ~MASK(b);

	writel(remmap[0] | (tmp[1] << BIT(a)), DDRC_REMMAP(a / 4 + 1));
	writel(remmap[1] | (tmp[0] << BIT(b)), DDRC_REMMAP(b / 4 + 1));
}

static void mem_remap(void)
{
	uint32_t start = 0;
	int num = 0;

	start += DDR_ROW + DDR_COL + (DDR_DW32 ? 4 : 2) / 2;
	start -= 12;

	if (DDR_BANK8)
		num += 3;
	else
		num += 2;

	if (DDR_CS0EN && DDR_CS1EN)
		num++;

	for (; num > 0; num--)
		remap_swap(0 + num - 1, start + num - 1);
}

void sdram_init(void)
{
	int board_revision;
	uint32_t mem_clk, ps, tmp;
	uint32_t ddrc_timing1 = 0, ddrc_timing2 = 0, ddrc_timing3 = 0;
	uint32_t ddrc_timing4 = 0, ddrc_timing5 = 0, ddrc_timing6 = 0;
	uint32_t ddrc_refcnt = 0;
	uint32_t mem_base0, mem_base1;
	uint32_t mem_mask0, mem_mask1;
	uint32_t mem_size0, mem_size1;
	const struct jz4780_ddr_config *ddr_config;

	board_revision = ci20_revision();

	if (board_revision == 2)
#ifdef CONFIG_SYS_DDR3_K4B2G0846Q
#if CONFIG_SYS_CPU_SPEED == 1200000000	&& CONFIG_SYS_EXTAL == 48000000
		ddr_config = &K4B2G0846Q_48_config;
#else
#error No DDR configuration for CPU speed
#endif	
#else
#error K4B2G0846Q support disabled
#endif
	else /* Fall back to H5TQ2G83CFR RAM */
#ifdef CONFIG_SYS_DDR3_H5TQ2G83CFR
#if CONFIG_SYS_CPU_SPEED == 1200000000	&& CONFIG_SYS_EXTAL == 48000000
		ddr_config = &H5TQ2G83CFR_48_config;
#else
#error No DDR configuration for CPU speed
#endif
#else
#error H5TQ2G83CFR support disabled
#endif

#if !defined(CONFIG_SPL_BUILD) || defined(CONFIG_SPL_LIBCOMMON_SUPPORT)
	puts("SDRAM ");
	puts(ddr_config->name);
	puts(" initialization...\n");
#endif

	/* Reset DLL in DDR PHY */
	writel(0x3, 0xb00000d0);
	udelay(400000);
	writel(0x1, 0xb00000d0);
	udelay(400000);

	/* enter reset */
	writel(0xf << 20, DDRC_CTRL);

	mem_clk = get_mem_clk();
	ps = 1000000000 / (mem_clk / 1000); /* ns per tck ns <= real value , ns * 1000*/

	ddrc_timing1 = ddr_config->timing1;
	ddrc_timing2 = ddr_config->timing2;
	ddrc_timing3 = ddr_config->timing3;
	ddrc_timing4 = ddr_config->timing4;
	ddrc_timing5 = ddr_config->timing5;
	ddrc_timing6 = ddr_config->timing6;

	ddrc_refcnt = DDR_CLK_DIV << 1 | DDRC_REFCNT_REF_EN;
	tmp = (1000000000 % mem_clk == 0) ? (1000000000 / mem_clk) : (1000000000 / mem_clk + 1);
	tmp = DDR_tREFI / tmp;
	tmp = tmp / (16 * (1 << DDR_CLK_DIV)) - 1;
	if (tmp > 0xff)
		tmp = 0xff;
	if (tmp < 1)
		tmp = 1;

	ddrc_refcnt |= tmp << DDRC_REFCNT_CON_BIT;

	writel(0x0, DDRC_CTRL);

	writel(0x150000, DDRP_DTAR);
	ddr_phy_init(ddr_config);

	writel(DDRC_CTRL_CKE | DDRC_CTRL_ALH, DDRC_CTRL);
	writel(0x0, DDRC_CTRL);

	ddr_cfg_init();

	writel(ddrc_timing1, DDRC_TIMING(1));
	writel(ddrc_timing2, DDRC_TIMING(2));
	writel(ddrc_timing3, DDRC_TIMING(3));
	writel(ddrc_timing4, DDRC_TIMING(4));
	writel(ddrc_timing5, DDRC_TIMING(5));
	writel(ddrc_timing6, DDRC_TIMING(6));

	mem_size0 = sdram_size(0);
	mem_size1 = sdram_size(1);

	if (!mem_size1 && mem_size0 > 0x20000000) {
		mem_base0 = 0x0;
		mem_mask0 = ~(((mem_size0 * 2) >> 24) - 1) & DDRC_MMAP_MASK_MASK;
	} else {
		mem_base0 = (DDR_MEM_PHY_BASE >> 24) & 0xff;
		mem_mask0 = ~((mem_size0 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
	}

	if (mem_size1) {
		mem_mask1 = ~((mem_size1 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
		mem_base1 = ((DDR_MEM_PHY_BASE + mem_size0) >> 24) & 0xff;
	} else {
		mem_mask1 = 0;
		mem_base1 = 0xff;
	}

	writel(mem_base0 << DDRC_MMAP_BASE_BIT | mem_mask0, DDRC_MMAP0);
	writel(mem_base1 << DDRC_MMAP_BASE_BIT | mem_mask1, DDRC_MMAP1);
	writel(DDRC_CTRL_CKE | DDRC_CTRL_ALH, DDRC_CTRL);
	writel(ddrc_refcnt, DDRC_REFCNT);
	writel(1 << 15 | 4 << 12 | 1 << 11 | 1 << 8 | 0 << 6 | 1 << 4 | 1 << 3 | 1 << 2 | 1 << 1, DDRC_CTRL);
	mem_remap();
	writel(readl(DDRC_ST) & ~0x40, DDRC_ST);

#if !defined(CONFIG_SPL_BUILD) || defined(CONFIG_SPL_LIBCOMMON_SUPPORT)
	puts(" done\n");
#endif
}
