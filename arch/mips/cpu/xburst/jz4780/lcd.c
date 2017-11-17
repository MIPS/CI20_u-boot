/*
 * Copyright (c) 2012 Ingenic Semiconductor Co., Ltd.
 *              http://www.ingenic.com/
 * Copyright (c) 2017 Mips Tech LLC
 *
 * Core file for Ingenic Display Controller driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <lcd.h>
#include "jz4780_android_lcd.h"
#include "logo/bmp_logo_info.h"
#include "logo/bmp_logo_data.h"

extern char lcd_is_enabled;
extern int lcd_line_length;
extern int lcd_color_fg;
extern int lcd_color_bg;

/*
 * Frame buffer memory information
 */
extern void *lcd_base;                  /* Start of framebuffer memory  */
extern void *lcd_console_address;       /* Start of console buffer      */

extern short console_col;
extern short console_row;

extern void lcd_ctrl_init (void *lcdbase);
extern void lcd_enable (void);

unsigned long fb_base;
unsigned long fb_size;

char lcd_is_enabled = 0;
char lcd_fb_base_is_init = 0;

void bitmap_plot (int x, int y)
{
	unsigned short i, j;
	unsigned char *bmap;
	unsigned char *fb;
	unsigned int *fb32;
	unsigned int pixel;

	bmap = &bmp_logo_bitmap[0];
	fb   = (unsigned char *)(lcd_base + y * lcd_line_length + x);

	if ( jzfb_get_controller_bpp(lcd_config_info.bpp)< 12) {
		printf("12\n");
		;
	} else {   /* true color mode */
		if(jzfb_get_controller_bpp(lcd_config_info.bpp) == 16) {
		printf("16\n");
			;
		} else {
			fb32 = (unsigned int *)(lcd_base + y * lcd_line_length);
			for (i=0; i<BMP_LOGO_HEIGHT; ++i) {
				for (j=0; j<BMP_LOGO_WIDTH; j++) {
					pixel = bmp_logo_palette[(bmap[j])] & bmp_logo_palette[(bmap[j] + 1)];
					fb32[j + x] = pixel << 16 | pixel << 8 | pixel;
				}
				bmap += BMP_LOGO_WIDTH;
				fb32 += lcd_config_info.modes->xres;
			}
		}
	}
}

/************************************************************************/
/* ** GENERIC Initialization Routines					*/
/************************************************************************/
int drv_lcd_init (void)
{
	if(!lcd_is_enabled){
		lcd_base = (void *)fb_base;
		int *lcdbase_p = (int *)fb_base;
		int i;


		lcd_line_length = (lcd_config_info.modes->xres * jzfb_get_controller_bpp(lcd_config_info.bpp)) / 8;
		lcd_ctrl_init(lcd_base);
		/* Initialize the lcd controller */

		// sizeof(int) is 4
		for(i = 0; i < (lcd_line_length * lcd_config_info.modes->yres / 4); i++){
			*lcdbase_p++ = 0;
		}

		lcd_enable();
		lcd_is_enabled = 1;
#ifdef CONFIG_ANDROID_LCD_HDMI_DEFAULT
		hdmi_ctrl_init();
#endif
	}
	return 0;
}

int lcd_fb_base_init(void)
{
	/* must do this before board_init() so that we
	 ** use the partition table in the tags if it
	 ** already exists
	 */
	if(!lcd_fb_base_is_init){
		unsigned long ram_size;
		unsigned long addr;

		ram_size = initdram(0);

		if (ram_size > CONFIG_EMC_LOW_SDRAM_SPACE_SIZE)
			ram_size = CONFIG_EMC_LOW_SDRAM_SPACE_SIZE;
		if (ram_size == 0) {
			printf("Error: ram size is 0\n");
			return -1;
		}

		addr = CONFIG_SYS_SDRAM_BASE + ram_size;
		addr &= ~(4096 - 1);
		addr = lcd_setmem(addr);
		if (addr < 0)
			return -1;
		fb_base = addr;

		lcd_fb_base_is_init = 1;
	}
	return 0;
}

/************************************************************************/
/* ** ROM capable initialization part - needed to reserve FB memory	*/
/************************************************************************/

/*
 * This is called early in the system initialization to grab memory
 * for the LCD controller.
 * Returns new address for monitor, after reserving LCD buffer memory
 * Note that this is running from ROM, so no write access to global data.
 */

unsigned long lcd_setmem (unsigned long addr)
{
	unsigned long size;
	int line_length;

	line_length = (lcd_config_info.modes->xres * jzfb_get_controller_bpp(lcd_config_info.bpp))/ 8;
	size = line_length * lcd_config_info.modes->yres;
	/* Round up to nearest full page */
	size = (size + PAGE_SIZE + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
	/* Allocate pages for the frame buffer. */
	fb_size = size;
	addr -= size;
	addr -= 0x03000000;	/* load the logo before 208M */
	//debug ("Reserving %ldk for LCD Framebuffer at: %08lx\n", size>>10, addr);
	return (addr);
}
