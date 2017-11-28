/*
 * NPM801 configuration
 *
 * Copyright (c) 2013 Imagination Technologies
 * Copyright (c) 2017 Mips Tech LLC
 *
 * Author: Paul Burton <paul.burton@mips.com>
 * Author: Dragan Cecavac <dragan.cecavac@mips.com>
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

#ifndef __CONFIG_NPM801_H__
#define __CONFIG_NPM801_H__

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
#define CONFIG_SYS_MEM_SPEED CONFIG_SYS_CPU_SPEED
#define CONFIG_SYS_MEM_DIV 3

#define CONFIG_SYS_AUDIO_SPEED (768 * 1000000)

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
#define CONFIG_CONS_INDEX		4
#define CONFIG_BAUDRATE			57600
#define CONFIG_SYS_BAUDRATE_TABLE \
	{ 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400 }

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

#define CONFIG_PARTITION_NAMES	"boot", "system", "vendor", "extended", "cache", "userdata"
#define CONFIG_PARTITION_NUM	6
#define CONFIG_DOS_BLKDEVPARTS

#define CONFIG_BOOTCOMMAND \
	"run blkdevpartscmd; run dtbcmd; run abootcmd;"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"stdin=eserial0,eserial3\0" \
	"stdout=eserial0,eserial3\0" \
	"stderr=eserial0,eserial3\0" \
	"blkdevpartscmd=mmc part; setenv bootargs blkdevparts=mmcblk0:${blkdevparts};\0" \
	"dtbcmd=env set bootargs \"${bootargs} dtb_addr=0x87000000\"; ext4load mmc 0:1 0x87000000 jz4780.dtb;\0" \
	"abootcmd=ext4load mmc 0:1 0x88000000 boot.img; boota 0x88000000;\0"


#define CONFIG_SYS_HUSH_PARSER

/*
 * MMC
 */
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_JZ_MMC
#define CONFIG_JZ_MMC_MSC0	1
#define CONFIG_JZ_MMC_SPLMSC	0

/*
 * Command line configuration.
 */
#define CONFIG_CMD_BDI		/* bdinfo                       */
#define CONFIG_CMD_BOOTA	/* boota                        */
#define CONFIG_CMD_BOOTD	/* bootd                        */
#define CONFIG_CMD_CONSOLE	/* coninfo                      */
#define CONFIG_CMD_DHCP		/* DHCP support                 */
#define CONFIG_CMD_ECHO		/* echo arguments               */
#define CONFIG_CMD_EXT4		/* ext4 support                 */
#define CONFIG_EXT4_WRITE
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FASTBOOT	/* Fastboot support             */
#define CONFIG_CMD_FAT		/* FAT support                  */
#define CONFIG_CMD_LOADB	/* loadb                        */
#define CONFIG_CMD_LOADS	/* loads                        */
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_MISC		/* Misc functions like sleep etc*/
#define CONFIG_CMD_MMC		/* MMC/SD support			*/
#define CONFIG_CMD_NET		/* networking support			*/
#define CONFIG_CMD_PING
#define CONFIG_CMD_RUN		/* run command in env variable	*/
#define CONFIG_CMD_SAVEENV	/* saveenv			*/
#define CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
#define CONFIG_CMD_SOURCE	/* "source" command support	*/

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
#define CONFIG_SYS_PROMPT "npm801# "
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
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		((14 + 512) << 10)

#define CONFIG_ENV_IS_NOWHERE

#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_MMC_FASTBOOT
#define CONFIG_FASTBOOT_BUF_SIZE	0x8000000
#define CONFIG_FASTBOOT_NAME_PREFIX	"npm801"

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

#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_LIBGENERIC_SUPPORT

#define CONFIG_SPL_TEXT_BASE			0xf4000800
#define CONFIG_SPL_MAX_SIZE			((14 * 1024) - 0x800)

/*
 * SDRAM Info.
 */
#define CONFIG_NR_DRAM_BANKS	1
#define CONFIG_EMC_LOW_SDRAM_SPACE_SIZE 	0x10000000 /* 256M */

/* SPL is stored on NAND and U BOOT image is loaded from a SD card */
#define CONFIG_COMBINED_NAND_MMC
#define CONFIG_SYS_NAND_PAGE_SIZE		4096

#define CONFIG_GPIO_BOOT_SEL0			(32 * 3 + 17)	/* GPD17 */
#define CONFIG_GPIO_BOOT_SEL1			(32 * 3 + 18)	/* GPD18 */
#define CONFIG_GPIO_KEY_WAKEUP			(32*0 + 30)	/*GPA30 WAKEUP*/
#define CONFIG_GPIO_KEY_MAIN			(32*4 + 24)	/*GPE24 MAIN*/
#define CONFIG_GPIO_KEY_VOLUME_INC		(32*3 + 17)	/* vol+ K4 */

#define CONFIG_GPIO_KEY_VOLUME_DEC		GPIO_BOOT_SEL1	/*vol- K3 */
#define CONFIG_GPIO_KEY_BACK			(32*1 + 5)
#define CONFIG_GPIO_KEY_MENU			(GPIO_KEY_MAIN)	/*GPE26 MSC0_D6/MSC1_D6/MSC2_D6*/

#define CONFIG_JZ4780_LCD
#define CONFIG_LCD_RESET_PIN			(32*1 + 22)	/*PB22*/
#define CONFIG_LCD_BPP				LCD_COLOR8

#define CONFIG_HIBERNATE_CONDITION

#endif /* __CONFIG_NPM801_H__ */
