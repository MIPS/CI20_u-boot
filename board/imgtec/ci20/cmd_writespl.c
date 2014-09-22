/*
 * CI20 SPL write command
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
#include <command.h>
#include <malloc.h>
#include <nand.h>
#include <asm/arch/jz4780.h>
#include <asm/arch/nand.h>

#define SPL_SIZE (14 * 1024)
#define SPL_PAGES DIV_ROUND_UP(SPL_SIZE, CONFIG_SYS_NAND_PAGE_SIZE)
#define SPL_BLOCKSIZE 256
#define SPL_ECCSIZE 112
#define SPL_BLOCKSPERPAGE (CONFIG_SYS_NAND_PAGE_SIZE / SPL_BLOCKSIZE)

static void calculate_bch64(uint8_t * data, uint8_t * ecc)
{
	uint32_t reg;
	int i;

	/* clear & completion & error interrupts */
	reg = BCH_BHINT_ENCF | BCH_BHINT_DECF | BCH_BHINT_ERR | BCH_BHINT_UNCOR;
	writel(reg, BCH_BASE + BCH_BHINT);

	/* setup BCH count register */
	reg = SPL_BLOCKSIZE << BCH_BHCNT_BLOCKSIZE_SHIFT;
	reg |= SPL_ECCSIZE << BCH_BHCNT_PARITYSIZE_SHIFT;
	writel(reg, BCH_BASE + BCH_BHCNT);

	/* setup BCH control register */
	reg = BCH_BHCR_BCHE | BCH_BHCR_INIT | BCH_BHCR_ENCE;
	reg |= 64 << BCH_BHCR_BSEL_SHIFT;
	writel(reg, BCH_BASE + BCH_BHCR);

	/* write data */
	for (i = 0; i < SPL_BLOCKSIZE; i++)
		writeb(data[i], BCH_BASE + BCH_BHDR);

	/* wait for completion */
	while (!(readl(BCH_BASE + BCH_BHINT) & BCH_BHINT_ENCF)) ;

	/* clear interrupts */
	writel(readl(BCH_BASE + BCH_BHINT), BCH_BASE + BCH_BHINT);

	/* read back parity data */
	for (i = 0; i < SPL_ECCSIZE; i++)
		ecc[i] = readb(BCH_BASE + BCH_BHPAR0 + i);

	/* disable BCH */
	writel(BCH_BHCR_BCHE, BCH_BASE + BCH_BHCCR);
}

static int raw_nand_write(nand_info_t * nand, uint32_t off, uint8_t * data,
			  size_t size)
{
	mtd_oob_ops_t ops = {
		.datbuf = data,
		.oobbuf = NULL,
		.len = size,
		.ooblen = 0,
		.mode = MTD_OPS_RAW
	};

	return mtd_write_oob(nand, off, &ops);
}

static int do_writespl(cmd_tbl_t * cmdtp, int flag, int argc,
		       char *const argv[])
{
	nand_info_t *nand = &nand_info[nand_curr_device];
	int page, block, err, copies = 8, ret = CMD_RET_FAILURE;
	uint32_t nand_off = 0;
	uint8_t *input, *in_ptr, *ecc_buf;

	if (argc < 2)
		return CMD_RET_USAGE;

	input = (uint8_t *) simple_strtoul(argv[1], NULL, 16);

	if (argc > 2)
		copies = (int)simple_strtoul(argv[2], NULL, 10);

	ecc_buf = malloc(CONFIG_SYS_NAND_PAGE_SIZE);
	if (!ecc_buf) {
		puts("failed to allocate ECC buffer\n");
		goto out;
	}

	printf("writing %d copies of SPL from 0x%p\n", copies, input);

	for (in_ptr = input; copies--; in_ptr = input) {
		for (page = 0; page < SPL_PAGES; page++) {
			printf("writing code page %d to 0x%x from 0x%p\n",
			       page, nand_off, in_ptr);

			/* write code */
			jz4780_nand_set_pn(nand, CONFIG_SYS_NAND_PAGE_SIZE, 256,
					   page ? 0 : 1);
			err =
			    raw_nand_write(nand, nand_off, in_ptr,
					   CONFIG_SYS_NAND_PAGE_SIZE);
			if (err) {
				printf("error %d writing code\n", err);
				goto out;
			}
			nand_off += CONFIG_SYS_NAND_PAGE_SIZE;

			/* calculate ECC */
			for (block = 0; block < SPL_BLOCKSPERPAGE; block++) {
				calculate_bch64(in_ptr,
						&ecc_buf[block * SPL_ECCSIZE]);
				in_ptr += SPL_BLOCKSIZE;
			}
			memset(&ecc_buf[SPL_BLOCKSPERPAGE * SPL_ECCSIZE], 0,
			       CONFIG_SYS_NAND_PAGE_SIZE -
			       (SPL_BLOCKSPERPAGE * SPL_ECCSIZE));

			/* write ECC */
			jz4780_nand_set_pn(nand, CONFIG_SYS_NAND_PAGE_SIZE, 0,
					   0);
			err =
			    raw_nand_write(nand, nand_off, ecc_buf,
					   CONFIG_SYS_NAND_PAGE_SIZE);
			if (err) {
				printf("error %d writing ECC\n", err);
				goto out;
			}
			nand_off += CONFIG_SYS_NAND_PAGE_SIZE;
		}

		/* advance to the next 128 page boundary */
		nand_off += (128 - (SPL_PAGES * 2)) * CONFIG_SYS_NAND_PAGE_SIZE;
	}

	ret = CMD_RET_SUCCESS;
out:
	jz4780_nand_set_pn(nand, 0, 0, 0);
	free(ecc_buf);
	return ret;
}

U_BOOT_CMD(writespl, 3, 1, do_writespl,
	   "write a new SPL to NAND flash", "address [copies=2]");
