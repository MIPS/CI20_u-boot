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

	__asm__ __volatile__("move %0, $sp":"=r"(ret):);

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

static void linux_cmdline_parse(const char *bootargs)
{
	const char *next, *quote;
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
}

static void boot_cmdline_linux(bootm_headers_t * images, const char *baseargs)
{
	linux_cmdline_init();
	linux_cmdline_parse(baseargs);
	linux_cmdline_parse(getenv("bootargs"));
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

static void boot_prep_linux(bootm_headers_t * images)
{
	char env_buf[12];
	const char *cp;
	ulong rd_start, rd_size;

#ifdef CONFIG_MEMSIZE_IN_BYTES
	sprintf(env_buf, "%lu", (ulong) gd->ram_size);
	debug("## Giving linux memsize in bytes, %lu\n", (ulong) gd->ram_size);
#else
	sprintf(env_buf, "%lu", (ulong) (gd->ram_size >> 20));
	debug("## Giving linux memsize in MB, %lu\n",
	      (ulong) (gd->ram_size >> 20));
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

static void boot_jump_linux(bootm_headers_t * images)
{
	typedef void __noreturn(*kernel_entry_t) (int, ulong, ulong, ulong);
	kernel_entry_t kernel = (kernel_entry_t) images->ep;
	ulong linux_extra = 0;

	debug("## Transferring control to Linux (at address %p) ...\n", kernel);

	bootstage_mark(BOOTSTAGE_ID_RUN_OS);

	if (mips_boot_qemu_malta)
		linux_extra = gd->ram_size;

	/* we assume that the kernel is in place */
	printf("\nStarting kernel ...\n\n");

	kernel(linux_argc, (ulong) linux_argv, (ulong) linux_env, linux_extra);
}

int do_bootm_linux(int flag, int argc, char *const argv[],
		   bootm_headers_t * images)
{
	/* No need for those on MIPS */
	if (flag & BOOTM_STATE_OS_BD_T)
		return -1;

	if (flag & BOOTM_STATE_OS_CMDLINE) {
		boot_cmdline_linux(images, NULL);
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

	boot_cmdline_linux(images, NULL);
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

void do_boota_linux(bootm_headers_t * images,
		    struct fastboot_boot_img_hdr *fb_hdr)
{
	unsigned page_mask;
	unsigned kernel_actual;
	const void *kernel_src_addr, *ramdisk_src_addr;
	void *kernel_dst_addr, *ramdisk_dst_addr;
	char arg[256];
	const char *s;

	/* init kernel, ramdisk and prepare parameters */
	page_mask = fb_hdr->page_size - 1;
	kernel_actual = (fb_hdr->kernel_size + page_mask) & (~page_mask);
	kernel_src_addr =
	    (const void *)((unsigned int)fb_hdr + fb_hdr->page_size);
	kernel_dst_addr = (void *)fb_hdr->kernel_addr;
	ramdisk_src_addr =
	    (const void *)(((unsigned int)kernel_src_addr) + kernel_actual);
	ramdisk_dst_addr = (void *)fb_hdr->ramdisk_addr;

	// The default kernel load address used by mkbootimg.c is 0x10000000
	// If the load address specified in the header appears invalid make
	// a guess about where to load it. The default zImage execution address
	// is 0x80f00000
	if (kernel_dst_addr < (void *)0x80000000)
		kernel_dst_addr = (void *)0x80f00000;

	// For the ramdisk we just make a SWAG
	// maybe it would be possible to just leave it where it is?
	if (kernel_dst_addr < (void *)0x80000000)
		kernel_dst_addr = (void *)0x81f00000;

#ifdef	DEBUG
	printf("      page_mask: 0x%x\n", page_mask);
	printf("    kernel size: 0x%x\n", kernel_actual);
	printf(" kernel srcaddr: %p\n", kernel_src_addr);
	printf(" kernel dstaddr: %p\n", kernel_dst_addr);
	printf("ramdisk srcaddr: %p\n", ramdisk_src_addr);
	printf("ramdisk dstaddr: %p\n", ramdisk_dst_addr);
#endif /* DEBUG */

	memcpy(kernel_dst_addr, kernel_src_addr, fb_hdr->kernel_size);
	memcpy(ramdisk_dst_addr, ramdisk_src_addr, fb_hdr->ramdisk_size);

	// Include any kernel command line options from boot.img
	boot_cmdline_linux(images, (const char *)fb_hdr->cmdline);

	// Add board serial number for Android boot
	s = getenv("serial#");
	if (s) {
		snprintf(arg, sizeof(arg), "androidboot.serialno=%s", s);
		linux_cmdline_set(arg, strlen(arg));
	}
	// TODO:  These extra lines are being added to support old
	// kernels which expect these two parameters to be included
	// on the command line.
	snprintf(arg, sizeof(arg), "rd_start=0x%0X",
		 (unsigned int)ramdisk_dst_addr);
	linux_cmdline_set(arg, strlen(arg));
	snprintf(arg, sizeof(arg), "rd_size=0x%X", fb_hdr->ramdisk_size);
	linux_cmdline_set(arg, strlen(arg));
#ifdef	DEBUG
	printf("rd_start=0x%X rd_size=0x%X\n",
	       (unsigned int)ramdisk_dst_addr, fb_hdr->ramdisk_size);
#endif /* DEBUG */

	boot_prep_linux(images);

	// TODO:  Replace with boot_jump_linux(images)?
	flush_cache_all();
	typedef void __noreturn(*kernel_entry_t) (int, ulong, ulong, ulong);
	kernel_entry_t kernel = (kernel_entry_t) kernel_dst_addr;
	(*kernel) (linux_argc, (ulong) linux_argv, (ulong) linux_env, 0ul);
}

#endif /* CONFIG_CMD_BOOTA */
