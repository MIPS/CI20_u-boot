/*
 * JZ4780 NAND (NEMC/BCH) driver
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
#include <asm/io.h>
#include <asm/arch/jzsoc.h>
#include <asm/arch/nand.h>

#define OFFSET_DATA 0x00000000
#define OFFSET_CMD  0x00400000
#define OFFSET_ADDR 0x00800000

static struct nand_ecclayout jz4780_nand_ecclayout = {
	.eccbytes = 336,
	.eccpos = CONFIG_SYS_NAND_ECCPOS,
	.oobavail = CONFIG_SYS_NAND_OOBSIZE - 338,
	.oobfree = {
		{ .offset = 2, .length = CONFIG_SYS_NAND_OOBSIZE - 338 },
	},
};

struct jz_nand_priv {
	int reading;
	int pn_bytes;
	int pn_size;
	int pn_skip;
};

static struct jz_nand_priv nand_privs[CONFIG_SYS_MAX_NAND_DEVICE];

static void jz_nand_clear_ints(void)
{
	uint32_t ints;

	/* clear & completion & error interrupts */
	ints = BCH_BHINT_ENCF | BCH_BHINT_DECF |
		   BCH_BHINT_ERR | BCH_BHINT_UNCOR;
	writel(ints, BCH_BASE + BCH_BHINT);
}

static void jz_nand_set_pn(int enable, bool force)
{
	uint32_t reg = readl(NEMC_BASE + NEMC_PNCR);

	/* check whether PNEN should change */
	if (!force && !((reg & NEMC_PNCR_PNEN) ^ !!enable))
		return;

	/* ...it should, either enable & reset or disable */
	if (enable)
		reg |= NEMC_PNCR_PNEN | NEMC_PNCR_PNRST;
	else
		reg &= ~NEMC_PNCR_PNEN;

	writel(reg, NEMC_BASE + NEMC_PNCR);
}

static int jz_nand_device_ready(struct mtd_info *mtd)
{
	udelay(20);
	return !!(readl(GPIO_PXPIN(0)) & 0x00100000);
}

static void jz_nand_cmd_ctrl(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *this = mtd->priv;
	uint32_t reg;

	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_ALE)
			this->IO_ADDR_W = (void __iomem *)(CONFIG_SYS_NAND_BASE + OFFSET_ADDR);
		else if (ctrl & NAND_CLE)
			this->IO_ADDR_W = (void __iomem *)(CONFIG_SYS_NAND_BASE + OFFSET_CMD);
		else
			this->IO_ADDR_W = (void __iomem *)(CONFIG_SYS_NAND_BASE + OFFSET_DATA);

		reg = readl(NEMC_BASE + NEMC_NFCSR);
		if (ctrl & NAND_NCE)
			reg |= NEMC_NFCSR_FCEn(1);
		else
			reg &= ~NEMC_NFCSR_FCEn(1);
		writel(reg, NEMC_BASE + NEMC_NFCSR);
	}

	if (cmd != NAND_CMD_NONE)
		writeb(cmd, this->IO_ADDR_W);
}

static void jz_nand_hwctl(struct mtd_info *mtd, int mode)
{
	struct nand_chip *this = mtd->priv;
	struct jz_nand_priv *priv = this->priv;
	uint32_t reg;

	jz_nand_clear_ints();

	/* check whether we're reading */
	priv->reading = mode == NAND_ECC_READ;

	/* setup BCH count register */
	reg = this->ecc.size << BCH_BHCNT_BLOCKSIZE_SHIFT;
	reg |= this->ecc.bytes << BCH_BHCNT_PARITYSIZE_SHIFT;
	writel(reg, BCH_BASE + BCH_BHCNT);

	/* setup BCH control register */
	reg = BCH_BHCR_BCHE | BCH_BHCR_INIT;
	reg |= CONFIG_SYS_NAND_ECCSTRENGTH << BCH_BHCR_BSEL_SHIFT;
	if (mode == NAND_ECC_WRITE)
		reg |= BCH_BHCR_ENCE;
	writel(reg, BCH_BASE + BCH_BHCR);
}

static int jz_nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat,
				u_char *ecc_code)
{
	struct nand_chip *this = mtd->priv;
	struct jz_nand_priv *priv = this->priv;
	int i;

	if (priv->reading)
		return 0;

	/* write data */
	for (i = 0; i < this->ecc.size; i++)
		writeb(dat[i], BCH_BASE + BCH_BHDR);

	/* wait for completion */
	while (!(readl(BCH_BASE + BCH_BHINT) & BCH_BHINT_ENCF));

	/* clear interrupts */
	writel(readl(BCH_BASE + BCH_BHINT), BCH_BASE + BCH_BHINT);

	/* read back parity data */
	for (i = 0; i < this->ecc.bytes; i++)
		ecc_code[i] = readb(BCH_BASE + BCH_BHPAR0 + i);

	/* disable BCH */
	writel(BCH_BHCR_BCHE, BCH_BASE + BCH_BHCCR);

	return 0;
}

static int jz_nand_correct_data(struct mtd_info *mtd, u_char *dat,
				   u_char *read_ecc, u_char *calc_ecc)
{
	struct nand_chip *this = mtd->priv;
	uint32_t status;
	int i, ret = -1;

	/* write data */
	for (i = 0; i < this->ecc.size; i++)
		writeb(dat[i], BCH_BASE + BCH_BHDR);

	/* write ECC */
	for (i = 0; i < this->ecc.bytes; i++)
		writeb(read_ecc[i], BCH_BASE + BCH_BHDR);

	/* wait for completion */
	do {
		udelay(1);
		status = readl(BCH_BASE + BCH_BHINT);
	} while (!(status & BCH_BHINT_DECF));

	/* clear interrupts */
	writel(status, BCH_BASE + BCH_BHINT);

	if (!(status & BCH_BHINT_DECF))
		goto out;

	/* check status */
	if (status & BCH_BHINT_UNCOR) {
		debug("uncorrectable ECC error\n");
		goto out;
	}

	/* correct any detected errors */
	if (status & BCH_BHINT_ERR) {
		int err_count = (status & BCH_BHINT_ERRC_MASK) >> BCH_BHINT_ERRC_SHIFT;
		for (i = 0; i < err_count; i++) {
			uint32_t err_reg = readl(BCH_BASE + BCH_BHERR0 + (i * 4));
			uint32_t mask = (err_reg & BCH_BHERRn_MASK_MASK) >> BCH_BHERRn_MASK_SHIFT;
			uint32_t index = (err_reg & BCH_BHERRn_INDEX_MASK) >> BCH_BHERRn_INDEX_SHIFT;
			dat[(index * 2) + 0] ^= mask;
			dat[(index * 2) + 1] ^= mask >> 8;
		}
	}

	ret = 0;
out:
	/* disable BCH */
	writel(BCH_BHCR_BCHE, BCH_BASE + BCH_BHCCR);

	return ret;
}

static void jz_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	struct jz_nand_priv *priv = this->priv;
	uint32_t off, pn_len;
	int i;

	if (priv->pn_bytes && priv->pn_size) {
		/* apply pseudo-random noise to each pn_size block */
		for (i = 0; i < (len / priv->pn_size); i++) {
			jz_nand_set_pn(priv->pn_bytes && !priv->pn_skip, true);
			if (priv->pn_skip)
				priv->pn_skip--;
			off = i * priv->pn_size;
			pn_len = MIN(priv->pn_size, len - off);
			nand_read_buf(mtd, &buf[off], pn_len);
			priv->pn_bytes = MAX(0, priv->pn_bytes - pn_len);
		}
	} else {
		jz_nand_set_pn(!!priv->pn_bytes, false);
		nand_read_buf(mtd, buf, len);
		priv->pn_bytes = MAX(0, priv->pn_bytes - len);
	}

	jz_nand_set_pn(!!priv->pn_bytes, false);
}

static void jz_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	struct jz_nand_priv *priv = this->priv;
	uint32_t off, pn_len;
	int i;

	if (priv->pn_bytes && priv->pn_size) {
		/* apply pseudo-random noise to each pn_size block */
		for (i = 0; i < (len / priv->pn_size); i++) {
			jz_nand_set_pn(priv->pn_bytes && !priv->pn_skip, true);
			if (priv->pn_skip)
				priv->pn_skip--;
			off = i * priv->pn_size;
			pn_len = MIN(priv->pn_size, len - off);
			nand_write_buf(mtd, &buf[off], pn_len);
			priv->pn_bytes = MAX(0, priv->pn_bytes - pn_len);
		}
	} else {
		jz_nand_set_pn(!!priv->pn_bytes, false);
		nand_write_buf(mtd, buf, len);
		priv->pn_bytes = MAX(0, priv->pn_bytes - len);
	}

	jz_nand_set_pn(!!priv->pn_bytes, false);
}

void jz4780_nand_set_pn(nand_info_t *nand, int bytes, int size, int skip)
{
	struct nand_chip *chip = nand->priv;
	struct jz_nand_priv *priv = chip->priv;

	/*
	 * Enable pseudo-random noise for <bytes> bytes.
	 *
	 * If size is non-zero, reset the seed after each <size> length block.
	 *
	 * If size & skip are both non-zero, disable PN for the first <skip>
	 * <size> length blocks.
	 */
	priv->pn_bytes = bytes;
	priv->pn_size = size;
	priv->pn_skip = skip;
}

int jz4780_nand_init(struct nand_chip *nand)
{
	struct jz_nand_priv *priv;
	uint32_t reg;

	/* set BCH clock divide */
	reg = readl(CPM_BCHCDR) & ~(CPM_BCHCDR_BCHCDR_MASK | CPM_BCHCDR_BPCS_MASK);
	reg |= ((CONFIG_SYS_CPU_SPEED / 200000000) - 1) & CPM_BCHCDR_BCHCDR_MASK;
	reg |= CPM_BCHCDR_BPCS_MPLL;
	writel(reg | CPM_BCHCDR_CE_BCH, CPM_BCHCDR);
	writel(reg | CPM_BCHCDR_CE_BCH, CPM_BCHCDR);
	while (readl(CPM_BCHCDR) & CPM_BCHCDR_BCH_BUSY);
	writel(reg, CPM_BCHCDR);

	/* ungate NEMC & BCH clocks */
	writel(readl(CPM_CLKGR0) & ~(CPM_CLKGR0_NEMC | CPM_CLKGR0_BCH), CPM_CLKGR0);

	nand->cmd_ctrl		= jz_nand_cmd_ctrl;
	nand->dev_ready		= jz_nand_device_ready;
	nand->read_buf		= jz_nand_read_buf;
	nand->write_buf		= jz_nand_write_buf;
	nand->ecc.mode		= NAND_ECC_HW_OOB_FIRST;
	nand->ecc.hwctl		= jz_nand_hwctl;
	nand->ecc.correct	= jz_nand_correct_data;
	nand->ecc.calculate	= jz_nand_calculate_ecc;
	nand->ecc.size		= CONFIG_SYS_NAND_ECCSIZE;
	nand->ecc.bytes		= CONFIG_SYS_NAND_ECCBYTES;
	nand->ecc.strength	= CONFIG_SYS_NAND_ECCSTRENGTH;
	nand->ecc.layout	= &jz4780_nand_ecclayout;
	nand->chip_delay	= 50;
	nand->options		= NAND_NO_SUBPAGE_WRITE;
	nand->bbt_options	= NAND_BBT_USE_FLASH;
	nand->priv = priv	= &nand_privs[0];

	priv->reading = 0;
	priv->pn_bytes = priv->pn_size = priv->pn_skip = 0;

	/* optimise timing */
	writel(0x18664400, NEMC_BASE + NEMC_SMCR1);

	/* enable flash chip */
	writel(readl(NEMC_BASE + NEMC_NFCSR) | NEMC_NFCSR_NFEn(1), NEMC_BASE + NEMC_NFCSR);

	/* begin without pseudo-random noise */
	jz_nand_set_pn(0, false);

	return 0;
}
