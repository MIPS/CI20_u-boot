/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <asm/addrspace.h>

DECLARE_GLOBAL_DATA_PTR;

#define	LINUX_MAX_ENVS		256
#define	LINUX_MAX_ARGS		256

#if defined(CONFIG_QEMU_MALTA)
#define mips_boot_qemu_malta	1
#else
#define mips_boot_qemu_malta	0
#endif

static int linux_argc;
static char **linux_argv;
static char *linux_argp;

static char **linux_env;
static char *linux_env_p;
static int linux_env_idx;

static ulong arch_get_sp(void)
{
	ulong ret;

	__asm__ __volatile__("move %0, $sp" : "=r"(ret) : );

	return ret;
}

void arch_lmb_reserve(struct lmb *lmb)
{
	ulong sp;

	sp = arch_get_sp();
	debug("## Current stack ends at 0x%08lx\n", sp);

	/* adjust sp by 4K to be safe */
	sp -= 4096;
	lmb_reserve(lmb, sp, CONFIG_SYS_SDRAM_BASE + gd->ram_size - sp);
}

static void linux_cmdline_init(void)
{
	linux_argc = 1;
	linux_argv = (char **)UNCACHED_SDRAM(gd->bd->bi_boot_params);
	linux_argv[0] = 0;
	linux_argp = (char *)(linux_argv + LINUX_MAX_ARGS);
}

static void linux_cmdline_set(const char *value, size_t len)
{
	linux_argv[linux_argc] = linux_argp;
	memcpy(linux_argp, value, len);
	linux_argp[len] = 0;

	linux_argp += len + 1;
	linux_argc++;
}

static void linux_cmdline_dump(void)
{
	int i;

	debug("## cmdline argv at 0x%p, argp at 0x%p\n",
	      linux_argv, linux_argp);

	for (i = 1; i < linux_argc; i++)
		debug("   arg %03d: %s\n", i, linux_argv[i]);
}

static void boot_cmdline_linux(bootm_headers_t *images)
{
	const char *bootargs, *next, *quote;

	linux_cmdline_init();

	bootargs = getenv("bootargs");
	if (!bootargs)
		return;

	next = bootargs;

	while (bootargs && *bootargs && linux_argc < LINUX_MAX_ARGS) {
		quote = strchr(bootargs, '"');
		next = strchr(bootargs, ' ');

		while (next && quote && quote < next) {
			/*
			 * we found a left quote before the next blank
			 * now we have to find the matching right quote
			 */
			next = strchr(quote + 1, '"');
			if (next) {
				quote = strchr(next + 1, '"');
				next = strchr(next + 1, ' ');
			}
		}

		if (!next)
			next = bootargs + strlen(bootargs);

		linux_cmdline_set(bootargs, next - bootargs);

		if (*next)
			next++;

		bootargs = next;
	}

	linux_cmdline_dump();
}

static void linux_env_init(void)
{
	linux_env = (char **)(((ulong) linux_argp + 15) & ~15);
	linux_env[0] = 0;
	linux_env_p = (char *)(linux_env + LINUX_MAX_ENVS);
	linux_env_idx = 0;
}

static void linux_env_set(const char *env_name, const char *env_val)
{
	if (linux_env_idx < LINUX_MAX_ENVS - 1) {
		linux_env[linux_env_idx] = linux_env_p;

		strcpy(linux_env_p, env_name);
		linux_env_p += strlen(env_name);

		if (mips_boot_qemu_malta) {
			linux_env_p++;
			linux_env[++linux_env_idx] = linux_env_p;
		} else {
			*linux_env_p++ = '=';
		}

		strcpy(linux_env_p, env_val);
		linux_env_p += strlen(env_val);

		linux_env_p++;
		linux_env[++linux_env_idx] = 0;
	}
}

static void boot_prep_linux(bootm_headers_t *images)
{
	char env_buf[12];
	const char *cp;
	ulong rd_start, rd_size;

#ifdef CONFIG_MEMSIZE_IN_BYTES
	sprintf(env_buf, "%lu", (ulong)gd->ram_size);
	debug("## Giving linux memsize in bytes, %lu\n", (ulong)gd->ram_size);
#else
	sprintf(env_buf, "%lu", (ulong)(gd->ram_size >> 20));
	debug("## Giving linux memsize in MB, %lu\n",
	      (ulong)(gd->ram_size >> 20));
#endif /* CONFIG_MEMSIZE_IN_BYTES */

	rd_start = UNCACHED_SDRAM(images->initrd_start);
	rd_size = images->initrd_end - images->initrd_start;

	linux_env_init();

	linux_env_set("memsize", env_buf);

	sprintf(env_buf, "0x%08lX", rd_start);
	linux_env_set("initrd_start", env_buf);

	sprintf(env_buf, "0x%lX", rd_size);
	linux_env_set("initrd_size", env_buf);

	sprintf(env_buf, "0x%08X", (uint) (gd->bd->bi_flashstart));
	linux_env_set("flash_start", env_buf);

	sprintf(env_buf, "0x%X", (uint) (gd->bd->bi_flashsize));
	linux_env_set("flash_size", env_buf);

	cp = getenv("ethaddr");
	if (cp)
		linux_env_set("ethaddr", cp);

	cp = getenv("eth1addr");
	if (cp)
		linux_env_set("eth1addr", cp);

	if (mips_boot_qemu_malta)
		linux_env_set("modetty0", "38400n8r");
}

static void boot_jump_linux(bootm_headers_t *images)
{
	typedef void __noreturn (*kernel_entry_t)(int, ulong, ulong, ulong);
	kernel_entry_t kernel = (kernel_entry_t) images->ep;
	ulong linux_extra = 0;

	debug("## Transferring control to Linux (at address %p) ...\n", kernel);

	bootstage_mark(BOOTSTAGE_ID_RUN_OS);

	if (mips_boot_qemu_malta)
		linux_extra = gd->ram_size;

	/* we assume that the kernel is in place */
	printf("\nStarting kernel ...\n\n");

	kernel(linux_argc, (ulong)linux_argv, (ulong)linux_env, linux_extra);
}

int do_bootm_linux(int flag, int argc, char * const argv[],
			bootm_headers_t *images)
{
	/* No need for those on MIPS */
	if (flag & BOOTM_STATE_OS_BD_T)
		return -1;

	if (flag & BOOTM_STATE_OS_CMDLINE) {
		boot_cmdline_linux(images);
		return 0;
	}

	if (flag & BOOTM_STATE_OS_PREP) {
		boot_prep_linux(images);
		return 0;
	}

	if (flag & BOOTM_STATE_OS_GO) {
		boot_jump_linux(images);
		return 0;
	}

	boot_cmdline_linux(images);
	boot_prep_linux(images);
	boot_jump_linux(images);

	/* does not return */
	return 1;
}

#ifdef	CONFIG_CMD_BOOTA

// I really wanted to put all this code into a separate
// arch/mips/lib/boota.c file but do_boota_linux() uses too much of the
// same code used by do_bootm_linux() and all (most?) of the utility
// routines are local to this file so do_boota_linux() ended up here.

#include <fastboot.h>

extern void flush_cache_all(void);

// TODO:  This is a bunch of cruft from X-Boot.  All these things
// should be replaced with either U-Boot definitions, or the values
// should be calculated at run time.  I suspect that the correct place
// to get this stuff from is the boot.img header.
//
// In the case of CFG_KERNEL_DST, it seems that the corresponding
// address from the the uImage file used to build boot.img is
// 0x80010000:
//
//      $ file uImage
//      uImage: u-boot legacy uImage, Linux-3.0.8-gb006f40, Linux/MIPS,
//      OS Kernel Image (gzip), 3762679 bytes, Thu Aug  7 16:22:16 2014,
//      Load Address: 0x80010000, Entry Point: 0x805FF400,
//      Header CRC: 0xC31FEAAA, Data CRC: 0xABB89D18
//
// Don't yet have a guess about what should replace CFG_RAMDISK_DST.
#define	CFG_KERNEL_DST	0x80f00000	/* Load Linux kernel to this addr */
#define	CFG_RAMDISK_DST	0x81a00000	/* initrd address */

bootm_headers_t images;		/* pointers to os/initrd/fdt images */

void do_boota_linux(bootm_headers_t * images, const char *addr,
		    struct fastboot_boot_img_hdr *fb_hdr)
{
	unsigned page_mask;
	unsigned kernel_actual;
	unsigned ramdisk_actual;
	char initrd_param[64];
	char cmdline[256];
	void (*kernel) (int, char **, char **, ulong);

	/* init kernel, ramdisk and prepare parameters */
	page_mask = fb_hdr->page_size - 1;
	kernel_actual = (fb_hdr->kernel_size + page_mask) & (~page_mask);
	ramdisk_actual = (fb_hdr->ramdisk_size + page_mask) & (~page_mask);
	fb_hdr->kernel_addr = (unsigned int)(addr + fb_hdr->page_size);
	fb_hdr->ramdisk_addr = fb_hdr->kernel_addr + kernel_actual;

#ifdef  DEBUG
	printf("      page_mask: 0x%x\n", page_mask);
	printf("    kernel size: 0x%x\n", kernel_actual);
	printf(" kernel address: 0x%x\n", fb_hdr->kernel_addr);
	printf("   ramdisk size: 0x%x\n", ramdisk_actual);
	printf("ramdisk address: 0x%x\n", fb_hdr->ramdisk_addr);
#endif /* DEBUG */

	memcpy((char *)CFG_KERNEL_DST, (char *)fb_hdr->kernel_addr,
	       fb_hdr->kernel_size);

	memcpy((char *)CFG_RAMDISK_DST, (char *)fb_hdr->ramdisk_addr,
	       fb_hdr->ramdisk_size);

	sprintf(initrd_param, " rd_start=0x%X rd_size=0x%X",
		CFG_RAMDISK_DST, fb_hdr->ramdisk_size);
#ifdef  DEBUG
	printf("initrd_param: \"%s\"\n", initrd_param);
#endif /* DEBUG */

	boot_cmdline_linux(NULL);
	// TODO:  These extra lines are being added to support old
	// kernels which expect these two parameters to be included
	// on the command line.
	sprintf(cmdline, "rd_start=0x%X", CFG_RAMDISK_DST);
	linux_cmdline_set(cmdline, strlen(cmdline));
	sprintf(cmdline, "rd_size=0x%X", fb_hdr->ramdisk_size);
	linux_cmdline_set(cmdline, strlen(cmdline));

	boot_prep_linux(images);

	kernel = (void (*)(int, char **, char **))CFG_KERNEL_DST;

	// TODO:  Replace with boot_jump_linux(images)?
	flush_cache_all();
	(*kernel) (linux_argc, linux_argv, linux_env, 0ul);
}

#endif /* CONFIG_CMD_BOOTA */
