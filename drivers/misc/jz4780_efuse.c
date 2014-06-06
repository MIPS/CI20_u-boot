/*
 * JZ4780 EFUSE driver
 *
 * Copyright (c) 2014 Imagination Technologies
 * Author: Alex Smith <alex.smith@imgtec.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/arch/efuse.h>
#include <asm/arch/jz4780.h>
#include <asm/unaligned.h>

static void jz4780_efuse_read_chunk(size_t addr, size_t count, uint8_t *buf)
{
	uint32_t val;
	size_t i;

	val = EFUSE_EFUCTRL_RD_EN |
	      (count - 1) << EFUSE_EFUCTRL_LEN_BIT |
	      addr << EFUSE_EFUCTRL_ADDR_BIT |
	      ((addr > 0x200) ? EFUSE_EFUCTRL_CS : 0);
	writel(val, NEMC_BASE + EFUSE_EFUCTRL);

	while (!(readl(NEMC_BASE + EFUSE_EFUSTATE) & EFUSE_EFUSTATE_RD_DONE))
		;

	if ((count % 4) == 0) {
		for (i = 0; i < count / 4; i++) {
			val = readl(NEMC_BASE + EFUSE_EFUDATA(i));
			put_unaligned(val, (uint32_t *)(buf + (i * 4)));
		}
	} else {
		val = readl(NEMC_BASE + EFUSE_EFUDATA(0));
		if (count > 2) buf[2] = (val >> 16) & 0xff;
		if (count > 1) buf[1] = (val >> 8) & 0xff;
		buf[0] = val & 0xff;
	}
}

static inline int jz4780_efuse_chunk_size(size_t count)
{
	if (count >= 32)
		return 32;
	else if ((count / 4) > 0)
		return (count / 4) * 4;
	else
		return count % 4;
}

void jz4780_efuse_read(size_t addr, size_t count, uint8_t *buf)
{
	size_t chunk;

	while (count > 0) {
		chunk = jz4780_efuse_chunk_size(count);
		jz4780_efuse_read_chunk(addr, chunk, buf);
		addr += chunk;
		buf += chunk;
		count -= chunk;
	}
}

void jz4780_efuse_init(uint32_t ahb2_rate)
{
	uint32_t rd_adj, rd_strobe, tmp;

	rd_adj = (((6500 * (ahb2_rate / 1000000)) / 1000000) + 0xf) / 2;
	tmp = (((35000 * (ahb2_rate / 1000000)) / 1000000) - 4) - rd_adj;
	rd_strobe = ((tmp + 0xf) / 2 < 7) ? 7 : (tmp + 0xf) / 2;

	tmp = (rd_adj << EFUSE_EFUCFG_RD_ADJ_BIT) |
	      (rd_strobe << EFUSE_EFUCFG_RD_STROBE_BIT);
	writel(tmp, NEMC_BASE + EFUSE_EFUCFG);
}
