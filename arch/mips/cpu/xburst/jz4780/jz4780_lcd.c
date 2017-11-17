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
#include <asm/io.h>
#include "jz4780_android_lcd.h"
#include "logo/bmp_logo_info.h"


extern struct jzfb_config_info lcd_config_info;

#define  reg_write(addr,config)		\
	writel(config, lcd_config_info.lcdbaseoff + addr)

#define reg_read(addr)	\
	readl(lcd_config_info.lcdbaseoff + addr)

int lcd_line_length;

/*
 * Frame buffer memory information
 */
void *lcd_base;			/* Start of framebuffer memory	*/
void *lcd_console_address;	/* Start of console buffer	*/

void lcd_enable(void);
void lcd_disable(void);
static int jzfb_set_par(struct jzfb_config_info *);

int jzfb_get_controller_bpp(unsigned int bpp)
{
	switch (bpp) {
	case 18:
	case 24:
		return 32;
	case 15:
		return 16;
	default:
		return bpp;
	}
}

static void jzfb_config_tft_lcd_dma(struct jzfb_config_info *info)
{
	struct jz_fb_dma_descriptor *framedesc = info->dmadesc_fbhigh;
	//cmd_num;

#define BYTES_PER_PANEL	 (((info->modes->xres * jzfb_get_controller_bpp(info->bpp) / 8 + 3) >> 2 << 2) * info->modes->yres)
	framedesc->fdadr = virt_to_phys((void*)info->dmadesc_fbhigh);
	framedesc->fsadr = virt_to_phys((void *)info->screen);
	framedesc->fidr = 0xda0;
	framedesc->ldcmd = LCDC_CMD_EOFINT | LCDC_CMD_FRM_EN;
	framedesc->ldcmd |= BYTES_PER_PANEL/4;
	framedesc->offsize = 0;
	framedesc->page_width = 0;
	info->fdadr0 = virt_to_phys((void*)info->dmadesc_fbhigh);

	switch (jzfb_get_controller_bpp(info->bpp)) {
	case 16:
		framedesc->cmd_num = LCDC_CPOS_RGB_RGB565
			| LCDC_CPOS_BPP_16;
		break;
	case 30:
		framedesc->cmd_num = LCDC_CPOS_BPP_30;
		break;
	default:
		framedesc->cmd_num = LCDC_CPOS_BPP_18_24;
		break;
	}
	/* global alpha mode */
	framedesc->cmd_num |= 0;
	/* data has not been premultied */
	framedesc->cmd_num |= LCDC_CPOS_PREMULTI;
	/* coef_sle 0 use 1 */
	framedesc->cmd_num |= LCDC_CPOS_COEF_SLE_1;

	/* fg0 alpha value */
	framedesc->desc_size = 0xff << LCDC_DESSIZE_ALPHA_BIT;
	framedesc->desc_size |= (((info->modes->yres - 1) << LCDC_DESSIZE_HEIGHT_BIT & LCDC_DESSIZE_HEIGHT_MASK) |
			((info->modes->xres - 1) << LCDC_DESSIZE_WIDTH_BIT & LCDC_DESSIZE_WIDTH_MASK));
}

static void jzfb_config_fg1_dma(struct jzfb_config_info *info)
{
	struct jz_fb_dma_descriptor *framedesc = info->dmadesc_fblow;

	/*
	 * the descriptor of DMA 1 just init once
	 * and generally no need to use it
	 */

#define BYTES_PER_PANEL	 (((info->modes->xres * jzfb_get_controller_bpp(info->bpp) / 8 + 3) >> 2 << 2) * info->modes->yres)
	framedesc->fsadr = virt_to_phys((void *)(info->screen + BYTES_PER_PANEL));
	framedesc->fdadr = (unsigned)virt_to_phys((void *)info->dmadesc_fblow);
	info->fdadr1 = (unsigned)virt_to_phys((void *)info->dmadesc_fblow);

	framedesc->fidr = 0xda1;

	framedesc->ldcmd = (LCDC_CMD_EOFINT & ~LCDC_CMD_FRM_EN)
		| (BYTES_PER_PANEL/4);
	framedesc->offsize = 0;
	framedesc->page_width = 0;

	/* global alpha mode, data has not been premultied, COEF_SLE is 11 */
	framedesc->cmd_num = LCDC_CPOS_BPP_18_24 | LCDC_CPOS_COEF_SLE_3 | LCDC_CPOS_PREMULTI;
	framedesc->desc_size |= (((info->modes->yres - 1) << LCDC_DESSIZE_HEIGHT_BIT & LCDC_DESSIZE_HEIGHT_MASK) |
			((info->modes->xres - 1) << LCDC_DESSIZE_WIDTH_BIT & LCDC_DESSIZE_WIDTH_MASK));

	framedesc->desc_size |= 0xff <<
		LCDC_DESSIZE_ALPHA_BIT;

	reg_write( LCDC_DA1, framedesc->fdadr);
}

static int jzfb_prepare_dma_desc(struct jzfb_config_info *info)
{
	info->dmadesc_fblow = (struct jz_fb_dma_descriptor *)((unsigned int)info->palette - 2*32);
	info->dmadesc_fbhigh = (struct jz_fb_dma_descriptor *)((unsigned int)info->palette - 1*32);

	jzfb_config_tft_lcd_dma(info);
	jzfb_config_fg1_dma(info);

	return 0;
}

static void jzfb_lvds_txctrl_is_reset(struct jzfb_config_info *info, int reset)
{
	unsigned int tmp;

	tmp = reg_read( LVDS_TXCTRL);
	if (reset) {
		/* 0:reset */
		tmp &= ~LVDS_TX_RSTB;
		reg_write( LVDS_TXCTRL, tmp);
	} else {
		tmp |= LVDS_TX_RSTB;
		reg_write( LVDS_TXCTRL, tmp);
	}
}

static void jzfb_lvds_txpll0_is_pll_en(struct jzfb_config_info *info, int pll_en)
{
	unsigned int tmp;

	tmp = reg_read( LVDS_TXPLL0);
        if (pll_en) {
		/* 1: enable */
		tmp |= LVDS_PLL_EN;
		reg_write( LVDS_TXPLL0, tmp);
	} else {
		tmp &= ~LVDS_PLL_EN;
		reg_write( LVDS_TXPLL0, tmp);
	}
}

static void jzfb_lvds_txpll0_is_bg_pwd(struct jzfb_config_info *info, int bg)
{
	unsigned int tmp;

	tmp = reg_read( LVDS_TXPLL0);
        if (bg) {
		/* 1: power down */
		tmp |= LVDS_BG_PWD;
		reg_write( LVDS_TXPLL0, tmp);
	} else {
		tmp &= ~LVDS_BG_PWD;
		reg_write( LVDS_TXPLL0, tmp);
	}
}

/* check LVDS_PLL_LOCK lock */
static void jzfb_lvds_check_pll_lock(struct jzfb_config_info *info)
{
	int count = 0;

	while (!(reg_read(LVDS_TXPLL0) & LVDS_PLL_LOCK)) {
		mdelay(1);
		if (count++ > 500) {
			printf("Wait LVDS PLL LOCK timeout\n");
			break;
		}
	}
}

static void jzfb_lvds_txctrl_config(struct jzfb_config_info *info)
{
	struct lvds_txctrl *txctrl = &info->txctrl;
	unsigned int ctrl = 0;

	if (txctrl->data_format) {
		ctrl = LVDS_MODEL_SEL;
	}
	ctrl |= LVDS_TX_RSTB; /* TXCTRL disable reset */
	if (txctrl->clk_edge_falling_7x)
		ctrl |= LVDS_TX_CKBIT_PHA_SEL;
	if (txctrl->clk_edge_falling_1x)
		ctrl |= LVDS_TX_CKBYTE_PHA_SEL;

	 /* 1x clock coarse tuning. TXCTRL: 15-13 bit */
	ctrl |= (txctrl->data_start_edge << LVDS_TX_CKOUT_PHA_S_BIT &
		 LVDS_TX_CKOUT_PHA_S_MASK);
	ctrl |= txctrl->operate_mode; /* TXCTRL: 30, 29, 12, 11, 1, 0 bit */
	/* 1x clock fine tuning. TXCTRL: 10-8 bit */
	ctrl |= ((txctrl->edge_delay << LVDS_TX_DLY_SEL_BIT) &
		 LVDS_TX_DLY_SEL_MASK);

	/* output amplitude control. TXCTRL: 7 6; 5-3 2 bit */
	switch (txctrl->output_amplitude) {
	case VOD_FIX_200MV:
		ctrl &= ~LVDS_TX_AMP_ADJ;
		ctrl &= ~LVDS_TX_LVDS;
		break;
	case VOD_FIX_350MV:
		ctrl &= ~LVDS_TX_AMP_ADJ;
		ctrl |= LVDS_TX_LVDS;
		break;
	default:
		ctrl |= LVDS_TX_AMP_ADJ;
		ctrl &= ~(0xf << 2);
		ctrl |= (txctrl->output_amplitude << 2);
		break;
	}

	reg_write( LVDS_TXCTRL, ctrl);
}

static void jzfb_lvds_txpll0_config(struct jzfb_config_info *info)
{
	struct lvds_txpll0 *txpll0 = &info->txpll0;
	unsigned int cfg = 0;
        int indiv = 0, fbdiv = 0;

	cfg = LVDS_PLL_EN; /* PLL enable control. 1:enable */
	if (txpll0->ssc_enable) {
		cfg |= LVDS_PLL_SSC_EN;
	} else {
		cfg &= ~LVDS_PLL_SSC_EN;
	}
	if (txpll0->ssc_mode_center_spread)
		cfg |= LVDS_PLL_SSC_MODE;

	/* post diinfoer */
	cfg |= (txpll0->post_divider << LVDS_PLL_POST_DIVA_BIT &
		LVDS_PLL_POST_DIVA_MASK);
       /* feedback_divider */
        if (txpll0->feedback_divider == 260) {
               fbdiv = 0;
        } else if (txpll0->feedback_divider >= 8 && txpll0->feedback_divider
		 <= 259) {
               fbdiv = txpll0->feedback_divider/2 - 2;
        }
	cfg |= (fbdiv << LVDS_PLL_PLLN_BIT & LVDS_PLL_PLLN_MASK);

	if (txpll0->input_divider_bypass) {
		cfg |= LVDS_PLL_IN_BYPASS;
		reg_write( LVDS_TXPLL0, cfg);
		return;
	}

        /*input_divider*/
        if (txpll0->input_divider == 2) {
                indiv = 0;
        } else if (txpll0->input_divider >= 3 && txpll0->input_divider <= 17) {
                indiv = txpll0->input_divider - 2;
        } else if (txpll0->input_divider >= 18 && txpll0->input_divider <= 34) {
                indiv = (txpll0->input_divider) / 2 - 2;
                indiv += 32;
        }
	cfg |= ((indiv << LVDS_PLL_INDIV_BIT) & LVDS_PLL_INDIV_MASK);

	reg_write( LVDS_TXPLL0, cfg);
}

static void jzfb_lvds_txpll1_config(struct jzfb_config_info *info)
{
	struct lvds_txpll1 *txpll1 = &info->txpll1;
	unsigned int cfg;

	cfg = (txpll1->charge_pump << LVDS_PLL_ICP_SEL_BIT &
	       LVDS_PLL_ICP_SEL_MASK);
	cfg |= (txpll1->vco_gain << LVDS_PLL_KVCO_BIT & LVDS_PLL_KVCO_MASK);
	cfg |= (txpll1->vco_biasing_current << LVDS_PLL_IVCO_SEL_BIT &
	       LVDS_PLL_IVCO_SEL_MASK);

        if (txpll1->sscn == 130) {
		cfg |= 0;
        }
	if (txpll1->sscn >=3 && txpll1->sscn <=129) {
                cfg |= ((txpll1->sscn - 2) << LVDS_PLL_SSCN_BIT &
			LVDS_PLL_SSCN_MASK);
	}

        if (txpll1->ssc_counter >= 0 && txpll1->ssc_counter <= 15) {
		cfg |= (txpll1->ssc_counter << LVDS_PLL_GAIN_BIT &
			LVDS_PLL_GAIN_MASK);
        }
	if (txpll1->ssc_counter >= 16 && txpll1->ssc_counter <= 8191) {
		cfg |= (txpll1->ssc_counter << LVDS_PLL_COUNT_BIT &
			LVDS_PLL_COUNT_MASK);
        }

	reg_write( LVDS_TXPLL1, cfg);
}

static void jzfb_lvds_txectrl_config(struct jzfb_config_info *info)
{
	struct lvds_txectrl *txectrl = &info->txectrl;
	unsigned int cfg;

	cfg = (txectrl->emphasis_level << LVDS_TX_EM_S_BIT &
	       LVDS_TX_EM_S_MASK);
	if (txectrl->emphasis_enable) {
		cfg |= LVDS_TX_EM_EN;
	}
	cfg |= (txectrl->ldo_output_voltage << LVDS_TX_LDO_VO_S_BIT &
		LVDS_TX_LDO_VO_S_MASK);
	if (!txectrl->phase_interpolator_bypass) {
		cfg |= (txectrl->fine_tuning_7x_clk << LVDS_TX_CK_PHA_FINE_BIT
			& LVDS_TX_CK_PHA_FINE_MASK);
		cfg |= (txectrl->coarse_tuning_7x_clk << LVDS_TX_CK_PHA_COAR_BIT
			& LVDS_TX_CK_PHA_COAR_MASK);
	} else {
		cfg |= LVDS_PLL_PL_BP;
	}

	reg_write( LVDS_TXECTRL, cfg);
}

static void jzfb_config_lvds_controller(struct jzfb_config_info *info)
{
	jzfb_lvds_txctrl_is_reset(info, 1); /* TXCTRL enable reset */
	jzfb_lvds_txpll0_is_bg_pwd(info, 0); /* band-gap power on */

	mdelay(5);

	jzfb_lvds_txpll0_is_pll_en(info, 1); /* pll enable */
	udelay(20);
	jzfb_lvds_txctrl_is_reset(info, 0); /* TXCTRL disable reset */

	jzfb_lvds_txctrl_config(info);
	jzfb_lvds_txpll0_config(info);
	jzfb_lvds_txpll1_config(info);
	jzfb_lvds_txectrl_config(info);
	jzfb_lvds_check_pll_lock(info);
}

static int lcd_enable_state = 0;

void lcd_enable(void)
{
	unsigned ctrl;

	if (lcd_enable_state == 0) {
		reg_write( LCDC_STATE, 0);
		reg_write( LCDC_DA0, lcd_config_info.fdadr0);
		ctrl = reg_read( LCDC_CTRL);
		ctrl |= LCDC_CTRL_ENA;
		ctrl &= ~LCDC_CTRL_DIS;
		reg_write( LCDC_CTRL, ctrl);
	}
	lcd_enable_state= 1;
}
void lcd_disable(void)
{
	unsigned ctrl;
	if (lcd_enable_state == 1) {
		ctrl = reg_read( LCDC_CTRL);
		ctrl |= LCDC_CTRL_DIS;
		reg_write(LCDC_CTRL, ctrl);
		while(!(reg_read(LCDC_STATE) & LCDC_STATE_LDD));
	}
	lcd_enable_state = 0;
}
static int jzfb_set_par(struct jzfb_config_info *info)
{
	struct fb_videomode *mode = info->modes;
	unsigned short hds, vds;
	unsigned short hde, vde;
	unsigned short ht, vt;
	unsigned cfg, ctrl;
	unsigned size0,size1;
	unsigned smart_cfg = 0;
	unsigned pcfg;
	unsigned long rate;
	unsigned vpll_tmp,nf,nr,no;
	unsigned rate_div;
	int i;

	hds = mode->hsync_len + mode->left_margin;
	hde = hds + mode->xres;
	ht = hde + mode->right_margin;

	vds = mode->vsync_len + mode->upper_margin;
	vde = vds + mode->yres;
	vt = vde + mode->lower_margin;

	/*
	 * configure LCDC config register
	 * use 8words descriptor, not use palette
	 */
	cfg = LCDC_CFG_NEWDES | LCDC_CFG_PALBP | LCDC_CFG_RECOVER;
	cfg |= info->lcd_type;

	if (!(mode->sync & FB_SYNC_HOR_HIGH_ACT))
		cfg |= LCDC_CFG_HSP;

	if (!(mode->sync & FB_SYNC_VERT_HIGH_ACT))
		cfg |= LCDC_CFG_VSP;

	if (lcd_config_info.pixclk_falling_edge)
		cfg |= LCDC_CFG_PCP;

	if (lcd_config_info.date_enable_active_low)
		cfg |= LCDC_CFG_DEP;

	/* configure LCDC control register */
	ctrl = LCDC_CTRL_BST_64 | LCDC_CTRL_OFUM;
	if (lcd_config_info.pinmd)
		ctrl |= LCDC_CTRL_PINMD;

	ctrl |= LCDC_CTRL_BPP_18_24;

	if (mode->pixclock) {
		rate = PICOS2KHZ(mode->pixclock) * 1000;
		mode->refresh = rate / vt / ht;
	} else {
		if (info->lcd_type == LCD_TYPE_8BIT_SERIAL) {
			rate = mode->refresh * (vt + 2 * mode->xres) * ht;
		} else {
			rate = mode->refresh * vt * ht;
		}
		//mode->pixclock = KHZ2PICOS(rate / 1000);
	}

	reg_write( LCDC_VAT, (ht << 16) | vt);
	reg_write( LCDC_DAH, (hds << 16) | hde);
	reg_write( LCDC_DAV, (vds << 16) | vde);

	reg_write( LCDC_HSYNC, mode->hsync_len);
	reg_write( LCDC_VSYNC, mode->vsync_len);

	reg_write( LCDC_CFG, cfg);

	reg_write( LCDC_CTRL, ctrl);

	pcfg = 0xC0000000 | (511<<18) | (400<<9) | (256<<0) ;
	reg_write( LCDC_PCFG, pcfg);

	size0 = (info->modes->xres << LCDC_SIZE_WIDTH_BIT) & LCDC_SIZE_WIDTH_MASK;
	size0 |= ((info->modes->yres << LCDC_SIZE_HEIGHT_BIT) & LCDC_SIZE_HEIGHT_MASK);
	size1 = size0;
	reg_write( LCDC_SIZE0, size0);
	reg_write( LCDC_SIZE1, size1);

	jzfb_prepare_dma_desc(info);

	//__cpm_select_lcdpclk_vpll();
	vpll_tmp = readl(CPM_CPVPCR);
	//printf("CPM_CPVPCR = ");
	//printf("%d\n", vpll_tmp);
	nf = ((vpll_tmp >> 19) & 0x1fff) + 1;
	nr = ((vpll_tmp >> 13) & 0x3f) + 1;
	no = ((vpll_tmp >> 9) & 0xf ) + 1;
	vpll_tmp = (CONFIG_SYS_EXTAL / 1000) * nf / nr / no; /* KHz */

	rate /= 1000; /* KHz */
	for (i = 1; i <= 0x100; i++) {
		if (vpll_tmp / i <= rate) {
			rate_div = i-1;
			break;
		}
	}

	__cpm_lcd1pclk_disable();
	__cpm_set_pix1div(rate_div);
	__cpm_lcd1pclk_enable();


	/* panel'type is TFT LVDS, need to configure LVDS controller */
	if (lcd_config_info.lvds) {
		jzfb_config_lvds_controller(info);
	}

	return 0;
}


static int jz_lcd_init_mem(void *lcdbase, struct jzfb_config_info *info)
{
	unsigned long palette_mem_size;
	int fb_size = (info->modes->xres *(jzfb_get_controller_bpp(info->bpp) / 8))* info->modes->yres;

	info->screen = (unsigned long)lcdbase;
	info->palette_size = 256;
	palette_mem_size = info->palette_size * sizeof(u16);

	/* locate palette and descs at end of page following fb */
	info->palette = (unsigned long)lcdbase + fb_size + PAGE_SIZE - palette_mem_size;

	return 0;
}

void lcd_ctrl_init(void *lcd_base)
{
	/* init registers base address */

	lcd_config_info.lcdbaseoff = LCD1_BASE - LCD0_BASE;
	/*enable pixel clock and lcdc gate */

	__cpm_start_lcd1(); /* gate: lcdc 1 */

	lcd_config_info.fmt_order = FORMAT_X8R8G8B8;

	jz_lcd_init_mem(lcd_base, &lcd_config_info);
	jzfb_set_par(&lcd_config_info);

	return;
}

void bitmap_plot (int x, int y);
int print_logo(void)
{
	int x, y;

	lcd_fb_base_init();
	drv_lcd_init();
	__lcd_special_on();
	__lcd_display_on();
	mdelay(40);
	/* Center the logo */
	x = (lcd_config_info.modes->xres - BMP_LOGO_WIDTH) / 2;
	y = (lcd_config_info.modes->yres - BMP_LOGO_HEIGHT) / 2;
	bitmap_plot(x, y);
	flush_cache_all();
}