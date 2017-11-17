/*
 * panel data for jz4780_lcd.c
 *
 * Copyright (c) 2012 Ingenic Semiconductor Co., Ltd.
 *              http://www.ingenic.com/
 * Copyright (c) 2017 Mips Tech LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <lcd.h>
#include "jz4780_android_lcd.h"

static struct fb_videomode jzfb1_videomode = {
		.name = "1024x768",
		.refresh = 60,
		.xres = 1024,
		.yres = 768,
		.pixclock = KHZ2PICOS(52000),
		.left_margin = 90,
		.right_margin = 0,
		.upper_margin = 10,
		.lower_margin = 0,
		.hsync_len = 0,
		.vsync_len = 0,
		.sync = 0 | 0, /* FB_SYNC_HOR_HIGH_ACT:0, FB_SYNC_VERT_HIGH_ACT:0 */
};

struct jzfb_config_info lcd_config_info = {
	.modes = &jzfb1_videomode,

	.lcd_type = LCD_TYPE_GENERIC_24_BIT,
	.bpp = 24,

	.pixclk_falling_edge = 0,
	.date_enable_active_low = 0,

	.lvds = 1,
	.txctrl.data_format = VESA,
	.txctrl.clk_edge_falling_7x = 0,
	.txctrl.clk_edge_falling_1x = 1,
	.txctrl.data_start_edge = START_EDGE_4,
	.txctrl.operate_mode = LVDS_1X_CLKOUT,
	.txctrl.edge_delay = DELAY_0_1NS,
	.txctrl.output_amplitude = VOD_350MV,

	.txpll0.ssc_enable = 0,
	.txpll0.ssc_mode_center_spread = 0,
	.txpll0.post_divider = POST_DIV_1,
	.txpll0.feedback_divider = 70,
	.txpll0.input_divider_bypass = 0,
	.txpll0.input_divider = 10,

	.txpll1.charge_pump = CHARGE_PUMP_10UA,
	.txpll1.vco_gain = VCO_GAIN_150M_400M,
	.txpll1.vco_biasing_current = VCO_BIASING_2_5UA,
	.txpll1.sscn = 0,
	.txpll1.ssc_counter = 0,

	.txectrl.emphasis_level = 0,
	.txectrl.emphasis_enable = 0,
	.txectrl.ldo_output_voltage = LDO_OUTPUT_1V,
	.txectrl.phase_interpolator_bypass = 1,
	.txectrl.fine_tuning_7x_clk = 0,
	.txectrl.coarse_tuning_7x_clk = 0,

	.dither_enable = 0,
};
