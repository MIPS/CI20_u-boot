/*
 * CI20 configuration
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

#ifndef __CONFIG_CI20_H__
#define __CONFIG_CI20_H__

/* Ingenic JZ4780 based, mips32r2el ISA */
#define CONFIG_MIPS32
#define CONFIG_SYS_LITTLE_ENDIAN
#define CONFIG_JZSOC
#define CONFIG_JZ4780

#define CONFIG_SYS_CPU_SPEED	1200000000	/* CPU clock: 1.2 GHz */
#define CONFIG_SYS_EXTAL	48000000	/* EXTAL freq: 48 MHz */
#define CONFIG_SYS_HZ		(CONFIG_SYS_EXTAL / 16) /* incrementer freq */
#define CONFIG_SYS_MIPS_TIMER_FREQ	CONFIG_SYS_CPU_SPEED

#define CONFIG_SYS_DDR3_H5TQ2G83CFR
#define CONFIG_SYS_DDR3_K4B2G0846Q
#define CONFIG_SYS_MEM_SPEED CONFIG_SYS_CPU_SPEED
#define CONFIG_SYS_MEM_DIV 3

#define CONFIG_SYS_AUDIO_SPEED (768 * 1000000)

/* define this if you require JTAG support */
#define CONFIG_JTAG

/* NS16550-ish UARTs, uart[0134] are accessible */
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		CONFIG_SYS_EXTAL
#define CONFIG_SYS_NS16550_COM1		0xb0030000 /* uart0 */
#define CONFIG_SYS_NS16550_COM2		0xb0031000 /* uart1 */
#define CONFIG_SYS_NS16550_COM4		0xb0033000 /* uart3 */
#define CONFIG_SYS_NS16550_COM5		0xb0034000 /* uart4 */
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_CONSOLE_MUX
#define CONFIG_CONS_INDEX		5
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE \
	{ 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 115200, 230400 }

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_FLASH_BASE	0 /* init flash_base as 0 */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_MISC_INIT_R 1

#define CONFIG_JZ4780_EFUSE 1

#define CONFIG_BOOTP_MASK	(CONFIG_BOOTP_DEFAUL)

#define CONFIG_BOOTDELAY 1
#define CONFIG_SYS_BOOTM_LEN (64 << 20)
#define BOOTARGS_COMMON \
	"console=ttyS4,115200 console=tty0 mem=256M@0x0 mem=768M@0x30000000 " \
	"rootwait quiet rw"

#ifdef CONFIG_SPL_MMC_SUPPORT

/* SD/MMC card defaults */

#define CONFIG_BOOTARGS \
	BOOTARGS_COMMON " root=/dev/mmcblk0p1"
#define CONFIG_BOOTCOMMAND \
	"run ethargs; ext4load mmc 0:1 0x88000000 /boot/uImage; bootm 0x88000000"

#else /* !CONFIG_SPL_MMC_SUPPORT */

/* NAND defaults */

#define CONFIG_BOOTARGS \
	BOOTARGS_COMMON " ubi.mtd=3 ubi.mtd=4 ubi.fm_autoconvert=1 " \
	"root=ubi1:root rootfstype=ubifs"
#define CONFIG_BOOTCOMMAND \
	"run ethargs; mtdparts default; ubi part boot; ubifsmount ubi:boot; " \
	"ubifsload 0x88000000 uImage; bootm 0x88000000"

#endif /* !CONFIG_SPL_MMC_SUPPORT */

#define CONFIG_EXTRA_ENV_SETTINGS \
	"stdin=eserial0,eserial4\0" \
	"stdout=eserial0,eserial4\0" \
	"stderr=eserial0,eserial4\0" \
	"ethargs=env set bootargs ${bootargs} dm9000.mac_addr=${ethaddr}\0"

#define CONFIG_SYS_HUSH_PARSER

/* NAND */
#define CONFIG_NAND			1
#define CONFIG_NAND_JZ4780		1
#define CONFIG_SYS_NAND_BASE		0xbb000000
#define CONFIG_SYS_NAND_ONFI_DETECTION	1
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_PAGE_SIZE	8192
#define CONFIG_SYS_NAND_BLOCK_SIZE	(2048 << 10)
#define CONFIG_SYS_NAND_OOBSIZE		448
#define CONFIG_SYS_NAND_ECCSIZE		1024
#define CONFIG_SYS_NAND_ECCSTRENGTH	24
#define CONFIG_SYS_NAND_ECCBYTES	((CONFIG_SYS_NAND_ECCSTRENGTH * 14) / 8)
#define CONFIG_SYS_NAND_ECC_POS		112
#define CONFIG_SYS_NAND_ECCPOS { \
	112, 113, 114, 115, 116, 117, 118, 119, \
	120, 121, 122, 123, 124, 125, 126, 127, \
	128, 129, 130, 131, 132, 133, 134, 135, \
	136, 137, 138, 139, 140, 141, 142, 143, \
	144, 145, 146, 147, 148, 149, 150, 151, \
	152, 153, 154, 155, 156, 157, 158, 159, \
	160, 161, 162, 163, 164, 165, 166, 167, \
	168, 169, 170, 171, 172, 173, 174, 175, \
	176, 177, 178, 179, 180, 181, 182, 183, \
	184, 185, 186, 187, 188, 189, 190, 191, \
	192, 193, 194, 195, 196, 197, 198, 199, \
	200, 201, 202, 203, 204, 205, 206, 207, \
	208, 209, 210, 211, 212, 213, 214, 215, \
	216, 217, 218, 219, 220, 221, 222, 223, \
	224, 225, 226, 227, 228, 229, 230, 231, \
	232, 233, 234, 235, 236, 237, 238, 239, \
	240, 241, 242, 243, 244, 245, 246, 247, \
	248, 249, 250, 251, 252, 253, 254, 255, \
	256, 257, 258, 259, 260, 261, 262, 263, \
	264, 265, 266, 267, 268, 269, 270, 271, \
	272, 273, 274, 275, 276, 277, 278, 279, \
	280, 281, 282, 283, 284, 285, 286, 287, \
	288, 289, 290, 291, 292, 293, 294, 295, \
	296, 297, 298, 299, 300, 301, 302, 303, \
	304, 305, 306, 307, 308, 309, 310, 311, \
	312, 313, 314, 315, 316, 317, 318, 319, \
	320, 321, 322, 323, 324, 325, 326, 327, \
	328, 329, 330, 331, 332, 333, 334, 335, \
	336, 337, 338, 339, 340, 341, 342, 343, \
	344, 345, 346, 347, 348, 349, 350, 351, \
	352, 353, 354, 355, 356, 357, 358, 359, \
	360, 361, 362, 363, 364, 365, 366, 367, \
	368, 369, 370, 371, 372, 373, 374, 375, \
	376, 377, 378, 379, 380, 381, 382, 383, \
	384, 385, 386, 387, 388, 389, 390, 391, \
	392, 393, 394, 395, 396, 397, 398, 399, \
	400, 401, 402, 403, 404, 405, 406, 407, \
	408, 409, 410, 411, 412, 413, 414, 415, \
	416, 417, 418, 419, 420, 421, 422, 423, \
	424, 425, 426, 427, 428, 429, 430, 431, \
	432, 433, 434, 435, 436, 437, 438, 439, \
	440, 441, 442, 443, 444, 445, 446, 447 }
#define CONFIG_SYS_NAND_HW_ECC_OOBFIRST	1
#define CONFIG_SYS_NAND_5_ADDR_CYCLE	1
#define CONFIG_SYS_NAND_PAGE_COUNT      (CONFIG_SYS_NAND_BLOCK_SIZE / CONFIG_SYS_NAND_PAGE_SIZE)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	0
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define MTDIDS_DEFAULT			"nand0=nand"
#define MTDPARTS_DEFAULT		"mtdparts=nand:8m(uboot-spl),2m(uboot),2m(uboot-env),64m(boot),-(system)"

/*
 * MMC
 */
#define CONFIG_GENERIC_MMC		1
#define CONFIG_MMC			1
#define CONFIG_JZ_MMC 1
#define CONFIG_JZ_MMC_MSC1 1
#define CONFIG_JZ_MMC_SPLMSC 1

/* Ethernet:  davicom DM9000 */
#define CONFIG_DRIVER_DM9000		1
#define CONFIG_DM9000_BASE		0xb6000000
#define DM9000_IO			CONFIG_DM9000_BASE
#define DM9000_DATA			(CONFIG_DM9000_BASE + 2)

/*
 * Command line configuration.
 */
#define CONFIG_CMD_BDI		/* bdinfo			*/
#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_BOOTA	/* boota			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_DHCP 	/* DHCP support			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/
#define CONFIG_CMD_EXT4 	/* ext4 support			*/
#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_LOADB	/* loadb			*/
#define CONFIG_CMD_LOADS	/* loads			*/
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#define CONFIG_CMD_MMC 	    /* MMC/SD support			*/
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_CMD_NET 	    /* networking support			*/
#define CONFIG_CMD_PING
#define CONFIG_CMD_RUN		/* run command in env variable	*/
#define CONFIG_CMD_SAVEENV	/* saveenv			*/
#define CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
#define CONFIG_CMD_SOURCE	/* "source" command support	*/
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS

#define CONFIG_DOS_PARTITION

#define CONFIG_LZO
#define CONFIG_RBTREE

/*
 * Serial download configuration
 */
#define CONFIG_LOADS_ECHO	1	/* echo on for serial download */

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_MAXARGS 16
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_PROMPT "ci20# "
#define CONFIG_SYS_CBSIZE 1024 /* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(64 * 1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 * 1024)

#define CONFIG_SYS_SDRAM_BASE		0x80000000 /* cached (KSEG0) address */
#define CONFIG_SYS_SDRAM_MAX_TOP	0x90000000 /* don't run into IO space */
#define CONFIG_SYS_INIT_SP_OFFSET	0x400000
#define CONFIG_SYS_LOAD_ADDR		0x88000000
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x88000000

#define CONFIG_SYS_TEXT_BASE		0x80000000
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

/*
 * Environment
 */
#ifdef CONFIG_ENV_IS_IN_MMC

#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		((14 + 512) << 10)

#else

#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		(CONFIG_SYS_NAND_BLOCK_SIZE * 5)

#endif

/*
 * SDRAM Info.
 */
#define CONFIG_NR_DRAM_BANKS	1

/*
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		32768
#define CONFIG_SYS_ICACHE_SIZE		32768
#define CONFIG_SYS_CACHELINE_SIZE	32

/* SPL */
#define CONFIG_SPL
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_STACK		0xf4004000 /* only max. 2KB spare! */

#define CONFIG_SPL_BSS_START_ADDR	0x8f000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x00080000 /* 512KB, arbitrary */

#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/jz4780/u-boot-spl.lds"

#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x1c /* 14KB offset */
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS	0x400 /* 512 KB */
#define CONFIG_SYS_NAND_U_BOOT_OFFS	(CONFIG_SYS_NAND_BLOCK_SIZE * 4)
#define CONFIG_SYS_NAND_U_BOOT_DST	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_NAND_U_BOOT_DST
#define CONFIG_SYS_NAND_U_BOOT_SIZE	(512 * 1024)

#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_LIBGENERIC_SUPPORT

#ifdef CONFIG_SPL_MMC_SUPPORT

#define CONFIG_SPL_TEXT_BASE		0xf4000a00
#define CONFIG_SPL_MAX_SIZE		((14 * 1024) - 0xa00)

#else /* !CONFIG_SPL_MMC_SUPPORT */

#define CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_NAND_BASE
#define CONFIG_SPL_NAND_DRIVERS
#define CONFIG_SPL_NAND_SIMPLE
#define CONFIG_SPL_NAND_LOAD

#define CONFIG_SPL_TEXT_BASE		0xf4000800
#define CONFIG_SPL_MAX_SIZE		((14 * 1024) - 0x800)

#endif /* !CONFIG_SPL_MMC_SUPPORT */

#endif /* __CONFIG_CI20_H__ */
