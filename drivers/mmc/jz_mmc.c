/*
 * Ingenic JZ MMC driver
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
#include <mmc.h>
#include <asm/arch/jzsoc.h>
#include <asm/jz_mmc.h>
#include <asm/unaligned.h>

struct jz_mmc_priv {
	uintptr_t base;
	uint32_t flags;
	int clock_div;
};

/* jz_mmc_priv flags */
#define JZ_MMC_BUS_WIDTH_MASK 0x3
#define JZ_MMC_BUS_WIDTH_1    0x0
#define JZ_MMC_BUS_WIDTH_4    0x2
#define JZ_MMC_BUS_WIDTH_8    0x3
#define JZ_MMC_SENT_INIT (1 << 2)

#ifdef CONFIG_SPL_BUILD
/* SPL will only use a single MMC device (CONFIG_JZ_MMC_SPLMSC) */
struct mmc mmc_dev[1];
struct jz_mmc_priv mmc_priv[1];
#else
struct mmc mmc_dev[3];
struct jz_mmc_priv mmc_priv[3];
#endif

static uint16_t jz_mmc_readw(struct jz_mmc_priv *priv, uintptr_t off)
{
	return readw(priv->base + off);
}

static uint32_t jz_mmc_readl(struct jz_mmc_priv *priv, uintptr_t off)
{
	return readl(priv->base + off);
}

static void jz_mmc_writel(uint32_t value, struct jz_mmc_priv *priv, uintptr_t off)
{
	writel(value, priv->base + off);
}

static int jz_mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd,
						struct mmc_data *data)
{
	struct jz_mmc_priv *priv = mmc->priv;
	uint32_t stat, mask, cmdat = 0;

	/* stop the clock */
	jz_mmc_writel(MSC_STRPCL_CLOCK_CONTROL_STOP, priv, MSC_STRPCL);
	while (jz_mmc_readl(priv, MSC_STAT) & MSC_STAT_CLK_EN);

	/* setup command */
	jz_mmc_writel(cmd->cmdidx, priv, MSC_CMD);
	jz_mmc_writel(cmd->cmdarg, priv, MSC_ARG);

	if (data) {
		/* setup data */
		cmdat |= MSC_CMDAT_DATA_EN;
		if (data->flags & MMC_DATA_WRITE)
			cmdat |= MSC_CMDAT_WRITE;

		jz_mmc_writel(data->blocks, priv, MSC_NOB);
		jz_mmc_writel(data->blocksize, priv, MSC_BLKLEN);
	} else {
		jz_mmc_writel(0, priv, MSC_NOB);
		jz_mmc_writel(0, priv, MSC_BLKLEN);
	}

	/* setup response */
	switch (cmd->resp_type) {
	case MMC_RSP_R1:
	case MMC_RSP_R1b:
		cmdat |= MSC_CMDAT_RESPONSE_R1;
		break;
	case MMC_RSP_R2:
		cmdat |= MSC_CMDAT_RESPONSE_R2;
		break;
	case MMC_RSP_R3:
		cmdat |= MSC_CMDAT_RESPONSE_R3;
		break;
	default:
		break;
	}

	if (cmd->resp_type & MMC_RSP_BUSY)
		cmdat |= MSC_CMDAT_BUSY;

	/* set init for the first command only */
	if (!(priv->flags & JZ_MMC_SENT_INIT)) {
		cmdat |= MSC_CMDAT_INIT;
		priv->flags |= JZ_MMC_SENT_INIT;
	}

	cmdat |= (priv->flags & JZ_MMC_BUS_WIDTH_MASK) << 9;

	/* write the data setup */
	jz_mmc_writel(cmdat, priv, MSC_CMDAT);

	/* unmask interrupts */
	mask = 0xffffffff & ~(MSC_IMASK_END_CMD_RES | MSC_IMASK_TIME_OUT_RES);
	if (data) {
		mask &= ~MSC_IMASK_DATA_TRAN_DONE;
		if (data->flags & MMC_DATA_WRITE)
			mask &= ~MSC_IMASK_TXFIFO_WR_REQ;
		else
			mask &= ~(MSC_IMASK_RXFIFO_RD_REQ | MSC_IMASK_TIME_OUT_READ);
	}
	jz_mmc_writel(mask, priv, MSC_IMASK);

	/* clear interrupts */
	jz_mmc_writel(0xffffffff, priv, MSC_IREG);

	/* start the command (& the clock) */
	jz_mmc_writel(MSC_STRPCL_START_OP | MSC_STRPCL_CLOCK_CONTROL_START, priv, MSC_STRPCL);

	/* wait for completion */
	while (!(stat = (jz_mmc_readl(priv, MSC_IREG) & (MSC_IREG_END_CMD_RES | MSC_IREG_TIME_OUT_RES))))
		udelay(1000);
	jz_mmc_writel(stat, priv, MSC_IREG);
	if (stat & MSC_IREG_TIME_OUT_RES)
		return TIMEOUT;

	if (cmd->resp_type & MMC_RSP_PRESENT) {
		/* read the response */
		if (cmd->resp_type & MMC_RSP_136) {
			uint16_t a, b, c, i;
			a = jz_mmc_readw(priv, MSC_RES);
			for (i = 0; i < 4; i++) {
				b = jz_mmc_readw(priv, MSC_RES);
				c = jz_mmc_readw(priv, MSC_RES);
				cmd->response[i] = (a << 24) | (b << 8) | (c >> 8);
				a = c;
			}
		} else {
			cmd->response[0] = jz_mmc_readw(priv, MSC_RES) << 24;
			cmd->response[0] |= jz_mmc_readw(priv, MSC_RES) << 8;
			cmd->response[0] |= jz_mmc_readw(priv, MSC_RES) & 0xff;
		}
	}

	if (data && (data->flags & MMC_DATA_WRITE)) {
		/* write the data */
		int sz = DIV_ROUND_UP(data->blocks * data->blocksize, 4);
		const void *buf = data->src;
		while (sz--) {
			uint32_t val = get_unaligned_le32(buf);
			while (!(jz_mmc_readl(priv, MSC_IREG) & MSC_IREG_TXFIFO_WR_REQ));
			jz_mmc_writel(val, priv, MSC_TXFIFO);
			buf += 4;
		}
	} else if (data && (data->flags & MMC_DATA_READ)) {
		/* read the data */
		int sz = data->blocks * data->blocksize;
		void *buf = data->dest;
		do {
			stat = jz_mmc_readl(priv, MSC_STAT);
			if (stat & MSC_STAT_TIME_OUT_READ)
				return TIMEOUT;
			if (stat & MSC_STAT_CRC_READ_ERROR)
				return COMM_ERR;
			if (stat & MSC_STAT_DATA_FIFO_EMPTY) {
				udelay(100);
				continue;
			}
			do {
				uint32_t val = jz_mmc_readl(priv, MSC_RXFIFO);
				if (sz == 1)
					*(uint8_t *)buf = (uint8_t)val;
				else if (sz == 2)
					put_unaligned_le16(val, buf);
				else if (sz >= 4)
					put_unaligned_le32(val, buf);
				buf += 4;
				sz -= 4;
				stat = jz_mmc_readl(priv, MSC_STAT);
			} while (!(stat & MSC_STAT_DATA_FIFO_EMPTY));
		} while (!(stat & MSC_STAT_DATA_TRAN_DONE));
	}

	return 0;
}

static void jz_mmc_set_ios(struct mmc *mmc)
{
	struct jz_mmc_priv *priv = mmc->priv;
	uint32_t real_rate = CONFIG_SYS_MEM_SPEED / priv->clock_div;
	uint8_t clk_div = 0;

	/* calculate clock divide */
	while ((real_rate > mmc->clock) && (clk_div < 7)) {
		real_rate >>= 1;
		clk_div++;
	}
	jz_mmc_writel(clk_div, priv, MSC_CLKRT);

	/* set the bus width for the next command */
	priv->flags &= ~JZ_MMC_BUS_WIDTH_MASK;
	if (mmc->bus_width == 8)
		priv->flags |= JZ_MMC_BUS_WIDTH_8;
	else if (mmc->bus_width == 4)
		priv->flags |= JZ_MMC_BUS_WIDTH_4;
	else
		priv->flags |= JZ_MMC_BUS_WIDTH_1;
}

static int jz_mmc_core_init(struct mmc *mmc)
{
	struct jz_mmc_priv *priv = mmc->priv;

	/* reset */
	jz_mmc_writel(MSC_STRPCL_RESET, priv, MSC_STRPCL);
	while (jz_mmc_readl(priv, MSC_STAT) & MSC_STAT_IS_RESETTING);

	/* maximum timeouts */
	jz_mmc_writel(0xffff, priv, MSC_RESTO);
	jz_mmc_writel(0xffffffff, priv, MSC_RDTO);

	/* enable low power mode */
	jz_mmc_writel(0x1, priv, MSC_LPM);

	return 0;
}

static void jz_mmc_init_one(int idx, int controller, uintptr_t base, int clock_div)
{
	struct mmc *mmc = &mmc_dev[idx];
	struct jz_mmc_priv *priv = &mmc_priv[idx];

	/* fill in the name */
	strcpy(mmc->name, "jz_mmc msc");
	mmc->name[10] = '0' + controller;
	mmc->name[11] = 0;

	/* setup priv */
	priv->base = base;
	priv->clock_div = clock_div;
	priv->flags = 0;

	/* setup mmc */
	mmc->priv = priv;
	mmc->send_cmd = jz_mmc_send_cmd;
	mmc->set_ios = jz_mmc_set_ios;
	mmc->init = jz_mmc_core_init;
	mmc->getcd = NULL;
	mmc->getwp = NULL;

	mmc->voltages = MMC_VDD_27_28 |
		MMC_VDD_28_29 | MMC_VDD_29_30 | MMC_VDD_30_31 | MMC_VDD_31_32 |
		MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_34_35 | MMC_VDD_35_36;
	mmc->host_caps = MMC_MODE_4BIT | MMC_MODE_HS_52MHz | MMC_MODE_HS | MMC_MODE_HC;

	/*
	 * min freq is for card identification, and is the highest
	 *  low-speed SDIO card frequency (actually 400KHz)
	 * max freq is highest HS eMMC clock as per the SD/MMC spec
	 *  (actually 52MHz)
	 */
	mmc->f_min = 375000;
	mmc->f_max = 48000000;

	mmc_register(mmc);
}

void jz_mmc_init(int clock_div)
{
	int i = 0;

#if defined(CONFIG_JZ_MMC_MSC0) && (!defined(CONFIG_SPL_BUILD) || (CONFIG_JZ_MMC_SPLMSC == 0))
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_MSC0, CPM_CLKGR0);
	jz_mmc_init_one(i++, 0, MSC0_BASE, clock_div);
#endif
#if defined(CONFIG_JZ_MMC_MSC1) && (!defined(CONFIG_SPL_BUILD) || (CONFIG_JZ_MMC_SPLMSC == 1))
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_MSC1, CPM_CLKGR0);
	jz_mmc_init_one(i++, 1, MSC1_BASE, clock_div);
#endif
#if defined(CONFIG_JZ_MMC_MSC2) && (!defined(CONFIG_SPL_BUILD) || (CONFIG_JZ_MMC_SPLMSC == 2))
	writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_MSC2, CPM_CLKGR0);
	jz_mmc_init_one(i++, 2, MSC2_BASE, clock_div);
#endif
}
