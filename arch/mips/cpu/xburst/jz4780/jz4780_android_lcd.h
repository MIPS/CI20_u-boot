/*
 * Copyright (C) 2005-2008, Ingenic Semiconductor Inc.
 * Copyright (c) 2017 Mips Tech LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __JZ4780_ANDROID_LCD_H__
#define __JZ4780_ANDROID_LCD_H__

#include "jz4780_lcd.h"
/*
 * LCD panel specific default definition
 */

#define __lcd_special_on()			\
do {						\
	udelay(50);				\
	gpio_direction_output(CONFIG_LCD_RESET_PIN, 0);	\
	udelay(100);				\
	gpio_direction_output(CONFIG_LCD_RESET_PIN, 1);		\
} while (0)

#define __lcd_special_off() 			\
do { 						\
	gpio_direction_output(CONFIG_LCD_RESET_PIN, 0);		\
} while (0)

/*****************************************************************************
 * LCD panel			*
 *****************************************************************************/

#define __lcd_display_pin_init() 	\
do { 					\
	__lcd_special_pin_init();	\
} while (0)

#define __lcd_display_on() 		\
do { 					\
	__lcd_special_on();		\
	mdelay(80);			\
} while (0)

#define __lcd_display_off() 		\
do { 					\
	__lcd_close_backlight();	\
	__lcd_special_off();	 	\
} while (0)

/************************************************************************/
/*
 * LCD controller stucture for JZSOC: JZ4730 JZ4740
 */
struct jz_fb_dma_descriptor {
        unsigned long   fdadr;          /* Frame descriptor address register */
	unsigned long   fsadr;          /* Frame source address register */
        unsigned long   fidr;           /* Frame ID register */
        unsigned long   ldcmd;          /* Command register */
        unsigned long   offsize;        /* Stride Offsize(in word) */
        unsigned long   page_width;     /* Stride Pagewidth(in word) */
        unsigned long   cmd_num;        /* Command Number(for SLCD) */
        unsigned long   desc_size;      /* Foreground Size */
};

#define PIXEL_ALIGN 16
#define MODE_NAME_LEN 32

#define PICOS2KHZ(a) (1000000000/(a))
#define KHZ2PICOS(a) (1000000000/(a))
#define FB_SYNC_HOR_HIGH_ACT    1   /* horizontal sync high active  */
#define FB_SYNC_VERT_HIGH_ACT   2   /* vertical sync high active    */
#define FB_SYNC_EXT		4   /* external sync        */
#define FB_SYNC_COMP_HIGH_ACT   8   /* composite sync high active   */
#define FB_SYNC_BROADCAST	16  /* broadcast video timings      */
/* vtotal = 144d/288n/576i => PAL  */
/* vtotal = 121d/242n/484i => NTSC */
#define FB_SYNC_ON_GREEN	32  /* sync on green */

#define FB_VMODE_NONINTERLACED  0	/* non interlaced */
#define FB_VMODE_INTERLACED	1	/* interlaced	*/
#define FB_VMODE_DOUBLE		2	/* double scan */
#define FB_VMODE_ODD_FLD_FIRST	4	/* interlaced: top line first */
#define FB_VMODE_MASK		255

enum jzfb_format_order {
	FORMAT_X8R8G8B8 = 1,
	FORMAT_X8B8G8R8,
};

/* LCD controller supported display device output mode */
enum jzfb_lcd_type {
	LCD_TYPE_GENERIC_16_BIT = 0,
	LCD_TYPE_GENERIC_18_BIT = 0 | (1 << 7),
	LCD_TYPE_GENERIC_24_BIT = 0 | (1 << 6),
	LCD_TYPE_SPECIAL_TFT_1 = 1,
	LCD_TYPE_SPECIAL_TFT_2 = 2,
	LCD_TYPE_SPECIAL_TFT_3 = 3,
	LCD_TYPE_8BIT_SERIAL = 0xc,
	LCD_TYPE_LCM = 0xd | (1 << 31),
};

/**
 * enum data_format - LVDS output data format
 * @JEIDA: JEIDA model.
 * @VESA: VESA model.
 */
enum data_format {
	JEIDA,
	VESA,
};
/* Output data start-edge tuning in 1x clock output mode. TXCTRL: 15-13 bit */
enum data_start_edge {
        START_EDGE_0 = 0x0, /*0 of T7X*/
        START_EDGE_1, /*1 of T7X*/
        START_EDGE_2, /*2 of T7X*/
        START_EDGE_3, /*3 of T7X*/
        START_EDGE_4, /*4 of T7X*/
        START_EDGE_5, /*5 of T7X*/
        START_EDGE_6, /*6 of T7X*/
        START_EDGE_7, /*7 of T7X*/
};

/* LVDS controller working mode */
enum operate_mode {
	LVDS_7X_CLKOUT = (1 << 30) | (1 << 29) | (1 << 12) | (1 << 0),
	LVDS_1X_CLKOUT = (1 << 30) | (1 << 29) | (1 << 0),
        CMOS_OUTPUT = (1 << 30) | (1 << 29) | (1 << 11) | (1 << 1) | (1 << 0),
	OUTPUT_HI_Z = (1 << 30) | (1 << 29) | (0 << 0),
	OUTPUT_ZERO = (1 << 30) | (1 << 29) | (1 << 1) | (0 << 0),
};

/* LVDS_TX Input Clock Edge-Delay Control. TXCTRL: 10-8 bit */
enum input_edge_delay {
        DELAY_0_1NS = 0x0, /*0.1ns*/
        DELAY_0_2NS, /*0.2ns*/
        DELAY_0_5NS, /*0.5ns*/
        DELAY_1NS, /*1ns*/
        DELAY_1_5NS, /*1.5ns*/
        DELAY_2NS, /*2.0ns*/
        DELAY_2_5NS, /*2.5ns*/
        DELAY_3NS, /*3.0ns*/
};

/* LVDS_TX Output Amplitude Control. TXCTRL: 7 6; 5-3 2 bit */
enum output_amplitude {
       VOD_FIX_200MV  = 0xfe,  /* fix output 200mv, 0xfe is used as a flag*/
       VOD_FIX_350MV  = 0xff,  /* fix output 350mv, 0xff is used as a flag*/

       VOD_150MV  = 0x0,/* 150mv is swing */
       VOD_200MV = 0x2, /* 200mv is swing */
       VOD_250MV = 0x4, /* 250mv is swing */
       VOD_300MV = 0x6, /* 300mv is swing */
       VOD_350MV = 0x8, /* 350mv is swing */
       VOD_400MV = 0xA, /* 400mv is swing */
       VOD_500MV = 0xC, /* 500mv is swing */
       VOD_600MV = 0xE, /* 600mv is swing */
       VOD_650MV = 0x1, /* 650mv is swing */
       VOD_700MV = 0x3, /* 700mv is swing */
       VOD_750MV = 0x5, /* 750mv is swing */
       VOD_800MV = 0x7, /* 800mv is swing */
       VOD_850MV = 0x9, /* 850mv is swing */
       VOD_900MV = 0xB, /* 900mv is swing */
       VOD_1000MV = 0xD, /* 1000mv is swing */
       VOD_1100MV = 0xF, /* 1100mv is swing */
};

/* PLL post divider control bits A */
enum pll_post_divider {
	POST_DIV_1,
	POST_DIV_2,
	POST_DIV_3 = 0x03,
	POST_DIV_4 = 0x02,
};

/*Note: This may not be the correct corresponding*/
enum pll_charge_pump {
        CHARGE_PUMP_4UA = 0x7, /*4uA*/
        CHARGE_PUMP_2UA = 0x3, /*2uA, N_fbk:32-40 */
        CHARGE_PUMP_2_5UA = 0x2, /*2.5uA N_fbk:42-60 */
        CHARGE_PUMP_3_3UA = 0x1, /*3.3uA N_fbk:62-80 */
        CHARGE_PUMP_5UA = 0x6, /*5uA   N_fbk:82-110 */
	CHARGE_PUMP_6_7_UA = 0x5, /*6.7uA N_fbk:112-158 */
        CHARGE_PUMP_10UA = 0x4, /*10uA  N_fbk:160-258 */
};

/*PLL KVCO (F_vco)*/
enum pll_vco_gain {
        VCO_GAIN_900M_1G = 0x0, /* 150M-400M VCO Frequency Range */
        VCO_GAIN_650M_900M = 0x1, /* 400M-650M VCO Frequency Range */
        VCO_GAIN_400M_650M = 0x2, /* 650M-900M VCO Frequency Range */
        VCO_GAIN_150M_400M = 0x3, /* 900M-1G VCO Frequency Range */
};

/* vco output current */
enum pll_vco_biasing_current {
	VCO_BIASING_1_25UA = 0x0, /* 1.25uA*/
        VCO_BIASING_2_5UA = 0x1, /* 2.5uA*/
        VCO_BIASING_3_75UA = 0x2, /* 3.75uA*/
        VCO_BIASING_5UA = 0x3, /* 5uA*/
};

/* Internal LDO output voltage configure */
enum ldo_output_vol {
	LDO_OUTPUT_1V,
	LDO_OUTPUT_1_1V,
	LDO_OUTPUT_1_2V,
	LDO_OUTPUT_1_3V,
};

/* struct lvds_txctrl - used to configure LVDS TXCTRL register */
struct lvds_txctrl {
	enum data_format data_format;
	unsigned clk_edge_falling_7x:1;
	unsigned clk_edge_falling_1x:1;
	enum data_start_edge data_start_edge;
	enum operate_mode operate_mode;
	enum input_edge_delay edge_delay;
	enum output_amplitude output_amplitude;
};

/*
 * struct lvds_txpll0 - used to configure LVDS TXPLL0 register
 * Note:
 F_in == F_pixclk;
 7 * F_in == F_out;
 150M < F_vco < 1G

 F_out = F_vco / N_out;
 F_pfd = F_pixclk / N_in;
 F_vco = (F_in / N_in) * N_fbk;
 */
struct lvds_txpll0 {
	unsigned ssc_enable:1;
	unsigned ssc_mode_center_spread:1;
	enum pll_post_divider post_divider; /* N_out : 1-4 */
	unsigned int feedback_divider; /* N_fbk : 8-260 */
	unsigned input_divider_bypass:1;
	unsigned int input_divider; /* N_in : 2-34 */
};

/*
 * struct lvds_txpll1 - used to configure LVDS TXPLL1 register
 * ssc_counter:
 * 0-8191, if ssc enable. It contain gain(0-15) and
 * count(16-8191). The sscn is smaller than
 * counter when use count.
 */
struct lvds_txpll1 {
	enum pll_charge_pump charge_pump;
	enum pll_vco_gain vco_gain;
	enum pll_vco_biasing_current vco_biasing_current;
        unsigned int sscn;       /* 3-130, if ssc enable */
        unsigned int ssc_counter;
};

/* struct lvds_txectrl - used to configure LVDS TXECTRL register */
struct lvds_txectrl {
	unsigned int emphasis_level;
	unsigned emphasis_enable:1;
	enum ldo_output_vol ldo_output_voltage;
	unsigned phase_interpolator_bypass:1;
	unsigned int fine_tuning_7x_clk;
	unsigned int coarse_tuning_7x_clk;
};

/* smart lcd interface_type */
enum smart_lcd_type {
	SMART_LCD_TYPE_PARALLEL,
	SMART_LCD_TYPE_SERIAL,
};

/* smart lcd command width */
enum smart_lcd_cwidth {
	SMART_LCD_CWIDTH_16_BIT_ONCE = (0 << 8),
	SMART_LCD_CWIDTH_9_BIT_ONCE = SMART_LCD_CWIDTH_16_BIT_ONCE,
	SMART_LCD_CWIDTH_8_BIT_ONCE = (0x1 << 8),
	SMART_LCD_CWIDTH_18_BIT_ONCE = (0x2 << 8),
	SMART_LCD_CWIDTH_24_BIT_ONCE = (0x3 << 8),
};

/* smart lcd data width */
enum smart_lcd_dwidth {
	SMART_LCD_DWIDTH_18_BIT_ONCE_PARALLEL_SERIAL = (0 << 10),
	SMART_LCD_DWIDTH_16_BIT_ONCE_PARALLEL_SERIAL = (0x1 << 10),
	SMART_LCD_DWIDTH_8_BIT_THIRD_TIME_PARALLEL = (0x2 << 10),
	SMART_LCD_DWIDTH_8_BIT_TWICE_TIME_PARALLEL = (0x3 << 10),
	SMART_LCD_DWIDTH_8_BIT_ONCE_PARALLEL_SERIAL = (0x4 << 10),
	SMART_LCD_DWIDTH_24_BIT_ONCE_PARALLEL = (0x5 << 10),
	SMART_LCD_DWIDTH_9_BIT_TWICE_TIME_PARALLEL = (0x7 << 10),
};

/**
 * @reg: the register address
 * @value: the value to be written
 * @type: operation type, 0: write register, 1: write command, 2: write data
 * @udelay: delay time in us
 */
struct smart_lcd_data_table {
	unsigned long reg;
	unsigned long value;
	unsigned long type;
	unsigned long udelay;
};

struct fb_videomode {
	const char *name;   /* optional */
	u32 refresh;        /* optional */
	u32 xres;
	u32 yres;
	u32 pixclock;
	u32 left_margin;
	u32 right_margin;
	u32 upper_margin;
	u32 lower_margin;
	u32 hsync_len;
	u32 vsync_len;
	u32 sync;
	u32 vmode;
	u32 flag;
};

struct jzfb_config_info {

	int num_modes;
	struct fb_videomode *modes;			/* valid video modes */
	enum jzfb_format_order fmt_order;	/* frame buffer pixel format order */
	int lcdbaseoff;						/* lcd register base offset from LCD_BASE*/

	enum jzfb_lcd_type lcd_type;		/* lcd type */
	unsigned int bpp;					/* bits per pixel for the lcd */
	unsigned pinmd:1;					/* 16bpp lcd data pin mapping. 0: LCD_D[15:0],1: LCD_D[17:10] LCD_D[8:1]*/

	unsigned pixclk_falling_edge:1;		/* pixclk_falling_edge: pixel clock at falling edge */
	unsigned date_enable_active_low:1;  /* data enable active low */

	unsigned lvds:1;					/* using LVDS controller. 0: not use, 1: use */
	struct lvds_txctrl txctrl;			/* the configure of LVDS Transmitter Control Register */
	struct lvds_txpll0 txpll0;			/* the configure of LVDS Transmitter's PLL Control Register 0 */
	struct lvds_txpll1 txpll1;			/* the configure of LVDS Transmitter's PLL Control Register 1 */
	struct lvds_txectrl txectrl;		/* the configure of LVDS Transmitter's Enhance Control */

	struct {
		enum smart_lcd_type smart_type;		/* smart lcd transfer type, 0: parrallel, 1: serial */
		enum smart_lcd_cwidth cmd_width;	/* smart lcd command width */
		enum smart_lcd_dwidth data_width;	/* smart lcd data Width */

		unsigned clkply_active_rising:1;	/* smart lcd clock polarity:
												0: Active edge is Falling,1: Active edge is Rasing */
		unsigned rsply_cmd_high:1;			/* smart lcd RS polarity.
												0: Command_RS=0, Data_RS=1; 1: Command_RS=1, Data_RS=0 */
		unsigned csply_active_high:1;		/* smart lcd CS Polarity.
												0: Active level is low, 1: Active level is high */
		unsigned long write_gram_cmd;		/* write graphic ram command */
		unsigned bus_width;			/* bus width in bit */
		unsigned int length_data_table;		/* array size of data_table */
		struct smart_lcd_data_table *data_table;/* init data table */
	} smart_config;

	unsigned dither_enable:1;	/* enable dither function: 1, disable dither function: 0 */
	/* when LCD'bpp is lower than 24, suggest to enable it */
	struct {
		unsigned dither_red;	/* 0:8bit dither, 1:6bit dither, 2: 5bit dither, 3: 4bit dither */
		unsigned dither_green;	/* 0:8bit dither, 1:6bit dither, 2: 5bit dither, 3: 4bit dither */
		unsigned dither_blue;	/* 0:8bit dither, 1:6bit dither, 2: 5bit dither, 3: 4bit dither */
	} dither;

	struct {
		unsigned spl;	/* special_tft SPL signal register setting */
		unsigned cls;	/* special_tft CLS signal register setting */
		unsigned ps;	/* special_tft PS signal register setting */
		unsigned rev;	/* special_tft REV signal register setting */
	} special_tft_config;

	unsigned long	fdadr0;	/* physical address of frame/palette descriptor */
	unsigned long	fdadr1;	/* physical address of frame descriptor */

	/* DMA descriptors */
	struct	jz_fb_dma_descriptor *	dmadesc_fblow;
	struct	jz_fb_dma_descriptor *	dmadesc_fbhigh;
	struct	jz_fb_dma_descriptor *	dmadesc_palette;
	struct	jz_fb_dma_descriptor *	dmadesc_cmd;
	struct	jz_fb_dma_descriptor *	dmadesc_cmd_tmp;

	unsigned long	screen;		/* address of frame buffer */
	unsigned long	palette;	/* address of palette memory */
	unsigned int	palette_size;
	unsigned long	dma_cmd_buf;	/* address of dma command buffer */

	void *par;
};

int jzfb_get_controller_bpp(unsigned int);
extern struct jzfb_config_info lcd_config_info;

#endif /* __JZ4780_ANDROID_LCD_H__ */
