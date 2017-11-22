/*
 * NPM801 setup code
 *
 * Copyright (c) 2013 Imagination Technologies
 * Copyright (c) 2017 Mips Tech LLC
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

#include <common.h>
#include <asm/arch/efuse.h>
#include <asm/arch/jz4780.h>
#include <asm/jz_mmc.h>

struct npm801_otp {
	uint32_t serial_number;
	uint32_t date;
	char manufacturer[2];
	uint8_t mac[6];
} __packed;

int misc_init_r(void)
{
	uint32_t cpccr, ahb2_div;
	struct npm801_otp otp;
	char manufacturer[3];

	/* read the board OTP data */
	cpccr = readl(CPM_CPCCR);
	ahb2_div = ((cpccr & CPM_CPCCR_H2DIV_MASK) >> CPM_CPCCR_H2DIV_BIT) + 1;
	jz4780_efuse_init(CONFIG_SYS_MEM_SPEED / ahb2_div);
	jz4780_efuse_read(0x18, 16, (uint8_t *)&otp);

	/* put other board information into the environment */
	setenv_ulong("serial#", otp.serial_number);
	setenv_ulong("board_date", otp.date);
	memcpy(manufacturer, otp.manufacturer, 2);
	manufacturer[2] = 0;
	setenv("board_mfr", manufacturer);

	return 0;
}

int board_mmc_init(bd_t *bd)
{
	uint32_t msc_cdr;

	writel(0x30f00000, GPIO_PXINTC(4));
	writel(0x30f00000, GPIO_PXMASKC(4));
	writel(0x30f00000, GPIO_PXPAT1C(4));
	writel(0x30f00000, GPIO_PXPAT0C(4));

	msc_cdr = CONFIG_SYS_MEM_SPEED / 24000000 / 2 - 1;
	writel(msc_cdr | CPM_MSCCDR_CE | (1 << 31), CPM_MSCCDR);
	while (readl(CPM_MSCCDR) & CPM_MSCCDR_MSC_BUSY);

	jz_mmc_init((msc_cdr + 1) * 2);
	return 0;
}

/* U-Boot common routines */
int checkboard(void)
{
	return 0;
}

#ifdef CONFIG_SPL_BUILD

void spl_board_init(void)
{
}

#endif /* CONFIG_SPL_BUILD */

/* Basic RTC ops */

static unsigned int rtc_read_reg(unsigned int reg)
{
  unsigned int data;
  do {
    data = readl(reg);
  } while (readl(reg) != data);
  return data;
}

/* Waiting for the RTC register writing finish */
static void __wait_write_ready(void)
{
  while (!(readl(RTC_RTCCR) & RTCCR_WRDY));
}

/* Waiting for the RTC register writable */
static void __wait_writable(void)
{
  __wait_write_ready();
  writel(WENR_WENPAT_WRITABLE, RTC_WENR);
  __wait_write_ready();
  while (!(readl(RTC_WENR) & WENR_WEN));
}

static void rtc_write_reg(unsigned int reg, unsigned int data)
{
  __wait_writable();
  writel(data, reg);
  __wait_write_ready();
}

int pm_do_hibernate(void)
{
	/*
	 * RTC Wakeup or 1Hz interrupt can be enabled or disabled
	 * through  RTC driver's ioctl (linux/driver/char/rtc_jz.c).
	 */
	writel(0x0, CPM_RSR);

	/* Set minimum wakeup_n pin low-level assertion time for wakeup: 2000ms */
	rtc_write_reg(RTC_HWFCR, HWFCR_WAIT_TIME(2000));

	/* Set reset pin low-level assertion time after wakeup: must  > 60ms */
	rtc_write_reg(RTC_HRCR, HRCR_WAIT_TIME(60));

	/* Scratch pad register to be reserved */
	rtc_write_reg(RTC_HSPR, 0x52544356);

	/* clear wakeup status register */
	rtc_write_reg(RTC_HWRSR, 0x0);

	/* set wake up valid level as low */
	rtc_write_reg(RTC_HWCR,0x8);

	/* Put CPU to hibernate mode */
	rtc_write_reg(RTC_HCR, HCR_PD);

	/* We can't get here */
	return 0;
}


static int wakeup_pressed(int filter)
{
	while ((!gpio_get_pin(CONFIG_GPIO_KEY_WAKEUP)) && --filter)
		mdelay(1);

	return !filter;
}

int hibernate_condition(void)
{
	if ((cpm_get_scrpad() & 0xffff) == REBOOT_SIGNATURE) {
		return 0;
	}

	if (!(rtc_read_reg(RTC_HWRSR) & RTC_HWRSR_PPR) &&
		!wakeup_pressed(500)) {
		return 1;
	}

	return 0;
}
