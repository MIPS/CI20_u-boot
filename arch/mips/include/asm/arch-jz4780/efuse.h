/*
 * JZ4780 EFUSE driver
 *
 * Copyright (c) 2014 Imagination Technologies
 * Author: Alex Smith <alex.smith@imgtec.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __JZ4780_EFUSE_H__
#define __JZ4780_EFUSE_H__

#define EFUSE_EFUCTRL			0xd0
#define EFUSE_EFUCFG			0xd4
#define EFUSE_EFUSTATE			0xd8
#define EFUSE_EFUDATA(n)		(0xdc + ((n) * 4))

#define EFUSE_EFUCTRL_RD_EN		(1 << 0)
#define EFUSE_EFUCTRL_LEN_BIT		16
#define EFUSE_EFUCTRL_LEN_MASK		0x1f
#define EFUSE_EFUCTRL_ADDR_BIT		21
#define EFUSE_EFUCTRL_ADDR_MASK		0x1ff
#define EFUSE_EFUCTRL_CS		(1 << 30)

#define EFUSE_EFUCFG_RD_STROBE_BIT	16
#define EFUSE_EFUCFG_RD_STROBE_MASK	0xf
#define EFUSE_EFUCFG_RD_ADJ_BIT		20
#define EFUSE_EFUCFG_RD_ADJ_MASK	0xf

#define EFUSE_EFUSTATE_RD_DONE		(1 << 0)

extern void jz4780_efuse_read(size_t addr, size_t count, uint8_t *buf);
extern void jz4780_efuse_init(uint32_t ahb2_rate);

#endif /* __JZ4780_EFUSE_H__ */
