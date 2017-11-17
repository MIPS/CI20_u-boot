/*
 * Include file for Ingenic Semiconductor's JZ4780 CPU.
 */
#ifndef __JZ4780_LCD_H__
#define __JZ4780_LCD_H__
#include <common.h>
#include <asm/arch/jz4780.h>

//----------------------------------------------------------------------
// Register Definitions
//
#define	LCD0_BASE	0xB3050000
#define LCD1_BASE	0xB30A0000
#define LVDS_BASE   LCD0_BASE

/*************************************************************************
 * LCD (LCD Controller)
 *************************************************************************/
/* Register Map Of LCDC */
#define LCDC_CFG	(LCD0_BASE+0x00) /* Configure Register */
#define LCDC_CTRL	(LCD0_BASE+0x30) /* Control Register */
#define LCDC_STATE	(LCD0_BASE+0x34) /* Status Register */

#define LCDC_OSDC	(LCD0_BASE+0x100) /* OSD Configure Register */
#define LCDC_OSDCTRL	(LCD0_BASE+0x104) /* OSD Control Register */
#define LCDC_OSDS	(LCD0_BASE+0x108) /* OSD Status Register */
#define LCDC_BGC0	(LCD0_BASE+0x10c) /* Background 0 Color Register */
#define LCDC_BGC1	(LCD0_BASE+0x24c) /* Background 1 Color Register */
#define LCDC_KEY0	(LCD0_BASE+0x110) /* Foreground Color Key Register 0 */
#define LCDC_KEY1	(LCD0_BASE+0x114) /* Foreground Color Key Register 1 */
#define LCDC_ALPHA	(LCD0_BASE+0x118) /* ALPHA Register */
#define LCDC_IPUR	(LCD0_BASE+0x11c) /* IPU Restart Register */
#define LCDC_RGBC	(LCD0_BASE+0x90) /* RGB Controll Register */

#define LCDC_VAT	(LCD0_BASE+0x0c) /* Virtual Area Setting Register */
#define LCDC_DAH	(LCD0_BASE+0x10) /* Display Area Horizontal Start/End Point */
#define LCDC_DAV	(LCD0_BASE+0x14) /* Display Area Vertical Start/End Point */

#define LCDC_XYP0	(LCD0_BASE+0x120) /* Foreground 0 XY Position Register */
#define LCDC_XYP1	(LCD0_BASE+0x124) /* Foreground 1 XY Position Register */
#define LCDC_SIZE0	(LCD0_BASE+0x128) /* Foreground 0 Size Register */
#define LCDC_SIZE1	(LCD0_BASE+0x12c) /* Foreground 1 Size Register */

#define LCDC_VSYNC	(LCD0_BASE+0x04) /* Vertical Synchronize Register */
#define LCDC_HSYNC	(LCD0_BASE+0x08) /* Horizontal Synchronize Register */
#define LCDC_PS		(LCD0_BASE+0x18) /* PS Signal Setting */
#define LCDC_CLS	(LCD0_BASE+0x1c) /* CLS Signal Setting */
#define LCDC_SPL	(LCD0_BASE+0x20) /* SPL Signal Setting */
#define LCDC_REV	(LCD0_BASE+0x24) /* REV Signal Setting */
#define LCDC_IID	(LCD0_BASE+0x38) /* Interrupt ID Register */
#define LCDC_DA0	(LCD0_BASE+0x40) /* Descriptor Address Register 0 */
#define LCDC_SA0	(LCD0_BASE+0x44) /* Source Address Register 0 */
#define LCDC_FID0	(LCD0_BASE+0x48) /* Frame ID Register 0 */
#define LCDC_CMD0	(LCD0_BASE+0x4c) /* DMA Command Register 0 */

#define LCDC_OFFS0	(LCD0_BASE+0x60) /* DMA Offsize Register 0 */
#define LCDC_PW0	(LCD0_BASE+0x64) /* DMA Page Width Register 0 */
#define LCDC_CNUM0	(LCD0_BASE+0x68) /*
				* DMA Command Counter Register 0
				* only used in smart LCD mode
				*/
#define LCDC_CPOS0		(LCD0_BASE+0x68) /* DMA Command and position Register 0 */
#define LCDC_DESSIZE0	(LCD0_BASE+0x6c) /* Foreground Size in Descriptor 0 Register*/

#define LCDC_DA1	(LCD0_BASE+0x50) /* Descriptor Address Register 1 */
#define LCDC_SA1	(LCD0_BASE+0x54) /* Source Address Register 1 */
#define LCDC_FID1	(LCD0_BASE+0x58) /* Frame ID Register 1 */
#define LCDC_CMD1	(LCD0_BASE+0x5c) /* DMA Command Register 1 */
#define LCDC_OFFS1	(LCD0_BASE+0x70) /* DMA Offsize Register 1 */
#define LCDC_PW1	(LCD0_BASE+0x74) /* DMA Page Width Register 1 */
#define LCDC_CNUM1	(LCD0_BASE+0x78) /*
				* DMA Command Counter Register 1
				* only used in smart LCD mode
				*/
#define LCDC_CPOS1	(LCD0_BASE+0x78) /* DMA Command and position Register 1 */
#define LCDC_DESSIZE1	(LCD0_BASE+0x7c) /* Foreground Size in Descriptor 1 Register */

#define LCDC_PCFG	(LCD0_BASE+0x2c0) /* Priority level threshold configure Register */
#define LCDC_DUAL_CTRL	(LCD0_BASE+0x2c8) /* Dual LCDC Channel Control register */

#define LCDC_ENH_CFG	(LCD0_BASE+0x400) /* Image inhancement CFG Register */
#define LCDC_ENH_CSCCFG	(LCD0_BASE+0x404) /* Color space conversion CFG Register */
#define LCDC_ENH_LUMACFG	(LCD0_BASE+0x408) /* LUMA CFG Register */
#define LCDC_ENH_CHROCFG0	(LCD0_BASE+0x40c) /* CHROMA0 CFG Register */
#define LCDC_ENH_CHROCFG1	(LCD0_BASE+0x410) /* CHROMA1 CFG Register */
#define LCDC_ENH_DITHERCFG	(LCD0_BASE+0x414) /* DITHER CFG Register */
#define LCDC_ENH_STATUS	(LCD0_BASE+0x418) /* Enhance status Register */
#define LCDC_ENH_GAMMA	(LCD0_BASE+0x800) /* GAMMA CFG Register */
#define LCDC_ENH_VEE	(LCD0_BASE+0x1000) /* VEE CFG Register */

/* Register Map Of LCDC */
#define LCDC_CFG	(LCD0_BASE+0x00) /* Configure Register */
#define LCDC_CTRL	(LCD0_BASE+0x30) /* Control Register */
#define LCDC_STATE	(LCD0_BASE+0x34) /* Status Register */

#define LCDC_OSDC	(LCD0_BASE+0x100) /* OSD Configure Register */
#define LCDC_OSDCTRL	(LCD0_BASE+0x104) /* OSD Control Register */
#define LCDC_OSDS	(LCD0_BASE+0x108) /* OSD Status Register */
#define LCDC_BGC0	(LCD0_BASE+0x10c) /* Background 0 Color Register */
#define LCDC_BGC1	(LCD0_BASE+0x24c) /* Background 1 Color Register */
#define LCDC_KEY0	(LCD0_BASE+0x110) /* Foreground Color Key Register 0 */
#define LCDC_KEY1	(LCD0_BASE+0x114) /* Foreground Color Key Register 1 */
#define LCDC_ALPHA	(LCD0_BASE+0x118) /* ALPHA Register */
#define LCDC_IPUR	(LCD0_BASE+0x11c) /* IPU Restart Register */
#define LCDC_RGBC	(LCD0_BASE+0x90) /* RGB Controll Register */

#define LCDC_VAT	(LCD0_BASE+0x0c) /* Virtual Area Setting Register */
#define LCDC_DAH	(LCD0_BASE+0x10) /* Display Area Horizontal Start/End Point */
#define LCDC_DAV	(LCD0_BASE+0x14) /* Display Area Vertical Start/End Point */

#define LCDC_XYP0	(LCD0_BASE+0x120) /* Foreground 0 XY Position Register */
#define LCDC_XYP1	(LCD0_BASE+0x124) /* Foreground 1 XY Position Register */
#define LCDC_SIZE0	(LCD0_BASE+0x128) /* Foreground 0 Size Register */
#define LCDC_SIZE1	(LCD0_BASE+0x12c) /* Foreground 1 Size Register */

#define LCDC_VSYNC	(LCD0_BASE+0x04) /* Vertical Synchronize Register */
#define LCDC_HSYNC	(LCD0_BASE+0x08) /* Horizontal Synchronize Register */
#define LCDC_PS		(LCD0_BASE+0x18) /* PS Signal Setting */
#define LCDC_CLS	(LCD0_BASE+0x1c) /* CLS Signal Setting */
#define LCDC_SPL	(LCD0_BASE+0x20) /* SPL Signal Setting */
#define LCDC_REV	(LCD0_BASE+0x24) /* REV Signal Setting */
#define LCDC_IID	(LCD0_BASE+0x38) /* Interrupt ID Register */
#define LCDC_DA0	(LCD0_BASE+0x40) /* Descriptor Address Register 0 */
#define LCDC_SA0	(LCD0_BASE+0x44) /* Source Address Register 0 */
#define LCDC_FID0	(LCD0_BASE+0x48) /* Frame ID Register 0 */
#define LCDC_CMD0	(LCD0_BASE+0x4c) /* DMA Command Register 0 */

#define LCDC_OFFS0	(LCD0_BASE+0x60) /* DMA Offsize Register 0 */
#define LCDC_PW0	(LCD0_BASE+0x64) /* DMA Page Width Register 0 */
#define LCDC_CNUM0	(LCD0_BASE+0x68) /*
				* DMA Command Counter Register 0
				* only used in smart LCD mode
				*/
#define LCDC_CPOS0	(LCD0_BASE+0x68) /* DMA Command and position Register 0 */
#define LCDC_DESSIZE0	(LCD0_BASE+0x6c) /* Foreground Size in Descriptor 0 Register*/

#define LCDC_DA1	(LCD0_BASE+0x50) /* Descriptor Address Register 1 */
#define LCDC_SA1	(LCD0_BASE+0x54) /* Source Address Register 1 */
#define LCDC_FID1	(LCD0_BASE+0x58) /* Frame ID Register 1 */
#define LCDC_CMD1	(LCD0_BASE+0x5c) /* DMA Command Register 1 */
#define LCDC_OFFS1	(LCD0_BASE+0x70) /* DMA Offsize Register 1 */
#define LCDC_PW1	(LCD0_BASE+0x74) /* DMA Page Width Register 1 */
#define LCDC_CNUM1	(LCD0_BASE+0x78) /*
				* DMA Command Counter Register 1
				* only used in smart LCD mode
				*/
#define LCDC_CPOS1	(LCD0_BASE+0x78) /* DMA Command and position Register 1 */
#define LCDC_DESSIZE1	(LCD0_BASE+0x7c) /* Foreground Size in Descriptor 1 Register */

#define LCDC_PCFG	(LCD0_BASE+0x2c0) /* Priority level threshold configure Register */
#define LCDC_DUAL_CTRL	(LCD0_BASE+0x2c8) /* Dual LCDC Channel Control register */

#define LCDC_ENH_CFG	(LCD0_BASE+0x400) /* Image inhancement CFG Register */
#define LCDC_ENH_CSCCFG	(LCD0_BASE+0x404) /* Color space conversion CFG Register */
#define LCDC_ENH_LUMACFG	(LCD0_BASE+0x408) /* LUMA CFG Register */
#define LCDC_ENH_CHROCFG0	(LCD0_BASE+0x40c) /* CHROMA0 CFG Register */
#define LCDC_ENH_CHROCFG1	(LCD0_BASE+0x410) /* CHROMA1 CFG Register */
#define LCDC_ENH_DITHERCFG	(LCD0_BASE+0x414) /* DITHER CFG Register */
#define LCDC_ENH_STATUS	(LCD0_BASE+0x418) /* Enhance status Register */
#define LCDC_ENH_GAMMA	(LCD0_BASE+0x800) /* GAMMA CFG Register */
#define LCDC_ENH_VEE	(LCD0_BASE+0x1000) /* VEE CFG Register */


/* LCD Configure Register */
#define LCDC_CFG_LCDPIN_BIT	31  /* LCD pins selection */
#define LCDC_CFG_LCDPIN_MASK	(0x1 << LCDC_CFG_LCDPIN_BIT)
#define LCDC_CFG_LCDPIN_LCD	(0x0 << LCDC_CFG_LCDPIN_BIT)
#define LCDC_CFG_LCDPIN_SLCD	(0x1 << LCDC_CFG_LCDPIN_BIT)
#define LCDC_CFG_TVEPEH		(1 << 30) /* TVE PAL enable extra halfline signal */
                                          /* Keep this bit to 0 */
#define LCDC_CFG_NEWDES		(1 << 28) /* use new descripter. old: 4words, new:8words */
#define LCDC_CFG_PALBP		(1 << 27) /* bypass data format and alpha blending */
#define LCDC_CFG_TVEN		(1 << 26) /* indicate the terminal is lcd or tv */
#define LCDC_CFG_RECOVER		(1 << 25) /* Auto recover when output fifo underrun */
/* Dither function has been move to DITHER CFG Register*/
#define LCDC_CFG_PSM		(1 << 23) /* PS signal mode */
#define LCDC_CFG_CLSM		(1 << 22) /* CLS signal mode */
#define LCDC_CFG_SPLM		(1 << 21) /* SPL signal mode */
#define LCDC_CFG_REVM		(1 << 20) /* REV signal mode */
#define LCDC_CFG_HSYNM		(1 << 19) /* HSYNC signal mode */
#define LCDC_CFG_PCLKM		(1 << 18) /* PCLK signal mode */
#define LCDC_CFG_INVDAT		(1 << 17) /* Inverse output data */
#define LCDC_CFG_SYNDIR_IN	(1 << 16) /* VSYNC&HSYNC direction */
#define LCDC_CFG_PSP		(1 << 15) /* PS pin reset state */
#define LCDC_CFG_CLSP		(1 << 14) /* CLS pin reset state */
#define LCDC_CFG_SPLP		(1 << 13) /* SPL pin reset state */
#define LCDC_CFG_REVP		(1 << 12) /* REV pin reset state */
#define LCDC_CFG_HSP		(1 << 11) /* HSYNC polarity:0-active high,1-active low */
#define LCDC_CFG_PCP		(1 << 10) /* PCLK polarity:0-rising,1-falling */
#define LCDC_CFG_DEP		(1 << 9)  /* DE polarity:0-active high,1-active low */
#define LCDC_CFG_VSP		(1 << 8)  /* VSYNC polarity:0-rising,1-falling */
#define LCDC_CFG_MODE_TFT_18BIT 	(1 << 7)  /* 18bit TFT */
#define LCDC_CFG_MODE_TFT_16BIT 	(0 << 7)  /* 16bit TFT */
#define LCDC_CFG_MODE_TFT_24BIT 	(1 << 6)  /* 24bit TFT */

/* LCD Control Register */
#define LCDC_CTRL_PINMD		(1 << 31) /* This register set Pin distribution in 16-bit parallel mode
					    0: 16-bit data correspond with LCDC_D[15:0]
					    1: 16-bit data correspond with LCDC_D[17:10], LCDC_D[8:1] */
#define LCDC_CTRL_BST_BIT	28  /* Burst Length Selection */
#define LCDC_CTRL_BST_MASK	(0x7 << LCDC_CTRL_BST_BIT)
#define LCDC_CTRL_BST_4		(0 << LCDC_CTRL_BST_BIT) /* 4-word */
#define LCDC_CTRL_BST_8		(1 << LCDC_CTRL_BST_BIT) /* 8-word */
#define LCDC_CTRL_BST_16	(2 << LCDC_CTRL_BST_BIT) /* 16-word */
#define LCDC_CTRL_BST_32	(3 << LCDC_CTRL_BST_BIT) /* 32-word */
#define LCDC_CTRL_BST_64	(4 << LCDC_CTRL_BST_BIT) /* 64-word */
#define LCDC_CTRL_RGB565		(0 << 27) /* RGB565 mode(foreground 0 in OSD mode) */
#define LCDC_CTRL_RGB555		(1 << 27) /* RGB555 mode(foreground 0 in OSD mode) */
#define LCDC_CTRL_OFUP		(1 << 26) /* Output FIFO underrun protection enable */
#define LCDC_CTRL_PDD_BIT	16  /* Load Palette Delay Counter */
#define LCDC_CTRL_PDD_MASK	(0xff << LCDC_CTRL_PDD_BIT)
                                /* Keep this bit to 0 */
#define LCDC_CTRL_DACTE		(1 << 14) /* DAC loop back test */
#define LCDC_CTRL_EOFM		(1 << 13) /* EOF interrupt mask */
#define LCDC_CTRL_SOFM		(1 << 12) /* SOF interrupt mask */
#define LCDC_CTRL_OFUM		(1 << 11) /* Output FIFO underrun interrupt mask */
#define LCDC_CTRL_IFUM0		(1 << 10) /* Input FIFO 0 underrun interrupt mask */
#define LCDC_CTRL_IFUM1		(1 << 9)  /* Input FIFO 1 underrun interrupt mask */
#define LCDC_CTRL_LDDM		(1 << 8)  /* LCD disable done interrupt mask */
#define LCDC_CTRL_QDM		(1 << 7)  /* LCD quick disable done interrupt mask */
#define LCDC_CTRL_BEDN		(1 << 6)  /* Endian selection */
#define LCDC_CTRL_PEDN		(1 << 5)  /* Endian in byte:0-msb first, 1-lsb first */
#define LCDC_CTRL_DIS		(1 << 4)  /* Disable indicate bit */
#define LCDC_CTRL_ENA		(1 << 3)  /* LCDC enable bit */
#define LCDC_CTRL_BPP_BIT	0  /* Bits Per Pixel */
#define LCDC_CTRL_BPP_MASK	(0x07 << LCDC_CTRL_BPP_BIT)
#define LCDC_CTRL_BPP_1		(0 << LCDC_CTRL_BPP_BIT) /* 1 bpp */
#define LCDC_CTRL_BPP_2		(1 << LCDC_CTRL_BPP_BIT) /* 2 bpp */
#define LCDC_CTRL_BPP_4		(2 << LCDC_CTRL_BPP_BIT) /* 4 bpp */
#define LCDC_CTRL_BPP_8		(3 << LCDC_CTRL_BPP_BIT) /* 8 bpp */
#define LCDC_CTRL_BPP_16	(4 << LCDC_CTRL_BPP_BIT) /* 15/16 bpp */
#define LCDC_CTRL_BPP_18_24	(5 << LCDC_CTRL_BPP_BIT) /* 18/24/32 bpp */
#define LCDC_CTRL_BPP_CMPS_24	(6 << LCDC_CTRL_BPP_BIT) /* 24 compress bpp */
#define LCDC_CTRL_BPP_30	(7 << LCDC_CTRL_BPP_BIT) /* 30 bpp */

/* LCD Status Register */
#define LCDC_STATE_QD		(1 << 7) /* Quick Disable Done */
#define LCDC_STATE_EOF		(1 << 5) /* EOF Flag */
#define LCDC_STATE_SOF		(1 << 4) /* SOF Flag */
#define LCDC_STATE_OFU		(1 << 3) /* Output FIFO Underrun */
#define LCDC_STATE_IFU0		(1 << 2) /* Input FIFO 0 Underrun */
#define LCDC_STATE_IFU1		(1 << 1) /* Input FIFO 1 Underrun */
#define LCDC_STATE_LDD		(1 << 0) /* LCD Disabled */

/* Foreground 0 or Foreground 1 Size Register */
#define LCDC_SIZE_HEIGHT_BIT	16 /* The height of foreground 0 or 1 */
#define LCDC_SIZE_HEIGHT_MASK	(0xfff << LCDC_SIZE_HEIGHT_BIT) /* The height of foreground 0 or 1 */
#define LCDC_SIZE_WIDTH_BIT	0 /* The width of foreground 0 or 1 */
#define LCDC_SIZE_WIDTH_MASK	(0xfff << LCDC_SIZE_WIDTH_BIT) /* The width of foreground 0 or 1 */

/* REV Signal Setting */
#define LCDC_REV_REVS_BIT	16  /* REV signal start position in dot clock */
#define LCDC_REV_REVS_MASK	(0xfff << LCDC_REV_REVS_BIT)


/* DMA Command 0 or 1 Register */
#define LCDC_CMD_SOFINT		(1 << 31) /* Enable start of frame interrupt */
#define LCDC_CMD_EOFINT		(1 << 30) /* Enable end of frame interrupt */
#define LCDC_CMD_CMD		(1 << 29) /* indicate command in slcd mode */
#define LCDC_CMD_PAL		(1 << 28) /* The descriptor contains a palette buffer */
#define LCDC_CMD_COMPEN		(1 << 27) /*
					   * It indicate this frame is 16/24bpp compressed or not
					   * 0:not compressed
					   * 1:compressed
					   */
#define LCDC_CMD_FRM_EN		(1 << 26) /* Indicate this frame is enable */
#define LCDC_CMD_FIELD_SEL	(1 << 25) /* Field select for interlace
					   * 0:odd field or no interlace
					   * 1:even field
					   */
#define LCDC_CMD_16X16BLOCK	(1 << 24) /* Fetch data by 16x16 block */
#define LCDC_CMD_LEN_BIT		0 /* The buffer length value (in word) */
#define LCDC_CMD_LEN_MASK	(0xffffff << LCDC_CMD_LEN_BIT)

/* DMA Offsize Register 0,1 */
#define LCDC_OFFS_BIT		0 /* OFFSIZE value for DMA 0,1(in word) */
#define LCDC_OFFS_OFFSIZE_MASK	(0xffffff << LCDC_OFFS_BIT)

/* DMA Page Width Register 0,1 */
#define LCDC_PW_BIT		0 /* Page width for DMA 0,1(in word) */
#define LCDC_PW_PAGEWIDTH_MASK	(0xffffff << LCDC_PW_BIT)

/* DMA Command Counter Register 0,1 */
#define LCDC_CNUM_BIT		 0 /* Commands' number in this frame transfer by DMA */
#define LCDC_CNUM_CNUM_MASK	(0xff << LCDC_CNUM_BIT) /* Only use in Smart LCD mode */

/* DMA Command Counter Register */
#define LCDC_CPOS_ALPHAMD1	(1 << 31) /* Alpha blending mode for foreground 0,1 */
#define LCDC_CPOS_RGB_RGB565	(0 << 30) /* foreground 0 or 1, 16bpp, 0-RGB565, 1-RGB555 */
#define LCDC_CPOS_RGB_RGB555	(1 << 30) /* foreground 0 or 1, 16bpp, 0-RGB565, 1-RGB555 */

#define LCDC_CPOS_BPP_BIT	27  /* Bits Per Pixel of OSD channel 1 (cannot use palette) */
#define LCDC_CPOS_BPP_MASK	(0x07 << LCDC_CPOS_BPP_BIT)
#define LCDC_CPOS_BPP_16	(4 << LCDC_CPOS_BPP_BIT) /* 15/16 bpp */
#define LCDC_CPOS_BPP_18_24	(5 << LCDC_CPOS_BPP_BIT) /* 18/24/32 bpp */
#define LCDC_CPOS_BPP_CMPS_24	(6 << LCDC_CPOS_BPP_BIT) /* 24 compress bpp */
#define LCDC_CPOS_BPP_30	(7 << LCDC_CPOS_BPP_BIT) /* 30 bpp */

#define LCDC_CPOS_PREMULTI	(1 << 26) /* Premulti enable of foreground 0,1 */
#define LCDC_CPOS_COEF_SLE_BIT	24 /* Select coefficient for foreground 0,1 */
#define LCDC_CPOS_COEF_SLE_MASK	(0x3 << LCDC_CPOS_COEF_SLE_BIT)
#define LCDC_CPOS_COEF_SLE_0	(0 << LCDC_CPOS_COEF_SLE_BIT) /* 00:0 */
#define LCDC_CPOS_COEF_SLE_1	(1 << LCDC_CPOS_COEF_SLE_BIT) /* 01:1 */
#define LCDC_CPOS_COEF_SLE_2	(2 << LCDC_CPOS_COEF_SLE_BIT) /* 10:alpha1 */
#define LCDC_CPOS_COEF_SLE_3	(3 << LCDC_CPOS_COEF_SLE_BIT) /* 11:1-alpha1 */

#define LCDC_CPOS_YPOS_BIT	12 /* The Y position of top-left part for foreground 0,1 */
#define LCDC_CPOS_YPOS_MASK	(0xfff << LCDC_CPOS_YPOS_BIT)
#define LCDC_CPOS_XPOS_BIT	0 /* The Y position of top-left part for foreground 0,1 */
#define LCDC_CPOS_XPOS_MASK	(0xfff << LCDC_CPOS_XPOS_BIT)

/* Foreground 0,1 Size Register */
#define LCDC_DESSIZE_ALPHA_BIT	24  /*  The global alpha value of foreground 0,1 */
#define LCDC_DESSIZE_ALPHA_MASK	(0xff << LCDC_DESSIZE_ALPHA_BIT)
#define LCDC_DESSIZE_HEIGHT_BIT	12  /* height of foreground 1 */
#define LCDC_DESSIZE_HEIGHT_MASK	(0xfff << LCDC_DESSIZE_HEIGHT_BIT)
#define LCDC_DESSIZE_WIDTH_BIT	0  /* width of foreground 1 */
#define LCDC_DESSIZE_WIDTH_MASK	(0xfff << LCDC_DESSIZE_WIDTH_BIT)

/*************************************************************************
 * LVDS interface
 *************************************************************************/
/* Register Map Of LVDSC (LVDS Controller) */
#define LVDS_TXCTRL	(LVDS_BASE + 0x3c0)
#define LVDS_TXPLL0	(LVDS_BASE + 0x3c4)
#define LVDS_TXPLL1	(LVDS_BASE + 0x3c8)
#define LVDS_TXECTRL	(LVDS_BASE + 0x3cc)

/* TXCTRL (LVDS Transmitter Control Register) */
#define LVDS_MODEL_SEL		(1 << 31) /* 0:JEIDA 1:VESA */
#define LVDS_TX_PDB		(1 << 30) /* 0:power down   */
#define LVDS_TX_PDB_CK		(1 << 29) /* 0:power down   */
#define LVDS_RESERVE(n)		(1 << (20 + (n)) /* n = 0,1,2,3,4,5,6,7 */
#define LVDS_TX_RSTB		(1 << 18) /* System reset signal, 0:Reset */
#define LVDS_TX_CKBIT_PHA_SEL	(1 << 17) /* 0:Rising edge 1:Falling edge */
#define LVDS_TX_CKBYTE_PHA_SEL	(1 << 16) /* 0:Rising edge 1:Falling edge */

#define LVDS_TX_CKOUT_PHA_S_BIT	13
#define LVDS_TX_CKOUT_PHA_S_MASK	(0x07 << LVDS_TX_CKOUT_PHA_S_BIT)

#define LVDS_TX_CKOUT_SET	(1 << 12) /* TX colock channel, 0:1x, 1:7x */
#define LVDS_TX_OUT_SEL		(1 << 11) /* 0:LVDS output 1:CMOS RGB output */
#define LVDS_TX_DLY_SEL_BIT	8
#define LVDS_TX_DLY_SEL_MASK	(0x07 << LVDS_TX_DLY_SEL_BIT)
#define LVDS_TX_AMP_ADJ		(1 << 7) /* LVDS output swing control */
#define LVDS_TX_LVDS		(1 << 6) /* 0:VOD = 200mv, 1:VOD = 350mv */
#define LVDS_TX_CR_BIT		3
#define LVDS_TX_CR_MASK		(0x07 << LVDS_TX_CR_BIT)
#define LVDS_TX_CR_CK		(1 << 2)
#define LVDS_TX_OD_S		(1 << 1) /* output level selectable pin */
#define LVDS_TX_OD_EN		(1 << 0) /* Tx output control. 1:enable */

/* TXPLL0 (LVDS Transmitter's PLL Control Register 0) */

#define LVDS_PLL_LOCK		(1 << 31) /* Lock derection output. 1:Lock */
#define LVDS_PLL_EN		(1 << 30) /* PLL enable control. 1:enable */
#define LVDS_BG_PWD		(1 << 29) /* 1:Band-gap power down */
#define LVDS_PLL_SSC_EN		(1 << 27) /* SSC function enable control */
#define LVDS_PLL_SSC_MODE	(1 << 26) /* 0:Down spread 1:Center spread */
#define LVDS_PLL_TEST		(1 << 25) /* Test enable control. 1:Enable */
#define LVDS_PLL_POST_DIVA_BIT	21
#define LVDS_PLL_POST_DIVA_MASK	(0x03 << LVDS_PLL_POST_DIVA_BIT)
#define LVDS_PLL_POST_DIVB_BIT	16
#define LVDS_PLL_POST_DIVB_MASK	(0x1f << LVDS_PLL_POST_DIVB_BIT) /* N/C */
#define LVDS_PLL_PLLN_BIT	8 /* PLL feedback divider value configure */
#define LVDS_PLL_PLLN_MASK	(0x7f << LVDS_PLL_PLLN_BIT)
#define LVDS_PLL_TEST_DIV_BIT	6
#define LVDS_PLL_TEST_DIV_MASK	(0x03 << LVDS_PLL_TEST_DIV_BIT)
#define LVDS_PLL_TEST_DIV_2	(0 << LVDS_PLL_TEST_DIV_BIT)
#define LVDS_PLL_TEST_DIV_4	(1 << LVDS_PLL_TEST_DIV_BIT)
#define LVDS_PLL_TEST_DIV_8	(2 << LVDS_PLL_TEST_DIV_BIT)
#define LVDS_PLL_TEST_DIV_16	(3 << LVDS_PLL_TEST_DIV_BIT)
#define LVDS_PLL_IN_BYPASS	(1 << 5) /* Input divider bypass */
#define LVDS_PLL_INDIV_BIT	0
#define LVDS_PLL_INDIV_MASK	(0x1f << LVDS_PLL_INDIV_BIT)

/* TXPLL1 (LVDS Transmitter's PLL Control Register 1) */

#define LVDS_PLL_ICP_SEL_BIT	29
#define LVDS_PLL_ICP_SEL_MASK	(0x07 << LVDS_PLL_ICP_SEL_BIT)
#define LVDS_PLL_KVCO_BIT	26
#define LVDS_PLL_KVCO_MASK	(0x03 << LVDS_PLL_KVCO_BIT)
#define LVDS_PLL_IVCO_SEL_BIT	24
#define LVDS_PLL_IVCO_SEL_MASK	(0x03 << LVDS_PLL_IVCO_SEL_BIT)
#define LVDS_PLL_SSCN_BIT	17
#define LVDS_PLL_SSCN_MASK	(0x7f << LVDS_PLL_SSCN_BIT)
#define LVDS_PLL_COUNT_BIT	4
#define LVDS_PLL_COUNT_MASK	(0x1fff << LVDS_PLL_COUNT_BIT)
#define LVDS_PLL_GAIN_BIT	0
#define LVDS_PLL_GAIN_MASK	(0x0f << LVDS_PLL_GAIN_BIT)

/* TXECTRL (LVDS Transmitter's Enhance Control */

#define LVDS_TX_EM_S_BIT	9 /* Emphasis level configure */
#define LVDS_TX_EM_S_MASK	(0x03 <<  LVDS_TX_EM_S_BIT)
#define LVDS_TX_EM_EN		(1 << 8) /* Emphasis driver control */
#define LVDS_TX_LDO_VO_S_BIT	5 /* Internal LDO output voltage configure */
#define LVDS_TX_LDO_VO_S_MASK	(0x03 << LVDS_TX_LDO_VO_S_BIT)
#define LVDS_TX_LDO_VO_S_0	(0x00 << LVDS_TX_LDO_VO_S_BIT) /* 00: 1.0V */
#define LVDS_TX_LDO_VO_S_1	(0x01 << LVDS_TX_LDO_VO_S_BIT) /* 01: 1.1V */
#define LVDS_TX_LDO_VO_S_2	(0x02 << LVDS_TX_LDO_VO_S_BIT) /* 02: 1.2V */
#define LVDS_TX_LDO_VO_S_3	(0x03 << LVDS_TX_LDO_VO_S_BIT) /* 03: 1.3V */
#define LVDS_PLL_PL_BP		(1 << 4) /* PLL phase interpolator bypass */

/*
 * Internal 7x clock phase fine tuning for data
 * setup/hold time optimization
 */
#define LVDS_TX_CK_PHA_FINE_BIT	2
#define LVDS_TX_CK_PHA_FINE_MASK	(0x03 << LVDS_TX_CK_PHA_FINE_BIT)
/*
 * Internal 7x clock phase coarse tuning for data
 * setup/hold time optimization
 */
#define LVDS_TX_CK_PHA_COAR_BIT	0
#define LVDS_TX_CK_PHA_COAR_MASK	(0x03 << LVDS_TX_CK_PHA_COAR_BIT)

/***************************************************************************
 * CPM
 ***************************************************************************/

#define __cpm_set_pix1div(v) \
	(writel((readl(CPM_LPCDR1) & ~CPM_LPCDR_PIXDIV_MASK) | ((v) << (CPM_LPCDR_PIXDIV_BIT)), CPM_LPCDR1))

#define __cpm_lcd1pclk_enable()	\
do {	\
	writel(readl(CPM_LPCDR1) | CPM_LPCDR_CELCD, CPM_LPCDR1);	\
	while (readl(CPM_LPCDR1) & CPM_LPCDR_LCD_BUSY);	\
} while (0)

#define __cpm_lcd1pclk_disable()	\
do {	\
	writel(readl(CPM_LPCDR1) & ~CPM_LPCDR_CELCD, CPM_LPCDR1);	\
	while (readl(CPM_LPCDR1) & CPM_LPCDR_LCD_BUSY);	\
} while (0)

#define __cpm_start_lcd1()	(writel(readl(CPM_CLKGR0) & ~CPM_CLKGR0_LCD1, CPM_CLKGR0))


#endif /* __JZ4780_LCD_H__ */
