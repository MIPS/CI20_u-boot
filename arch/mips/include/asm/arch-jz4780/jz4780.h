/*
 * JZ4780 definitions
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

#ifndef __JZ4780_H__
#define __JZ4780_H__

/* AHB0 BUS Devices */
#define	DDRC_BASE	0xb3010000

/* AHB2 BUS Devices */
#define NEMC_BASE	0xb3410000
#define BCH_BASE	0xb34d0000

/* APB BUS Devices */
#define	CPM_BASE	0xb0000000
#define	TCU_BASE	0xb0002000
#define	WDT_BASE	0xb0002000
#define	GPIO_BASE	0xb0010000
#define	UART0_BASE	0xb0030000
#define	UART1_BASE	0xb0031000
#define	UART2_BASE	0xb0032000
#define	UART3_BASE	0xb0033000
#define	MSC0_BASE	0xb3450000
#define	MSC1_BASE	0xb3460000
#define	MSC2_BASE	0xb3470000

/*************************************************************************
 * CPM (Clock reset and Power control Management)
 *************************************************************************/
#define CPM_CPCCR		(CPM_BASE+0x00) /* Clock control register		*/
#define CPM_CPCSR		(CPM_BASE+0xd4) /* Clock Status register		*/
#define CPM_CPPCR		(CPM_BASE+0x0c) /* PLL control register 		*/
#define CPM_CPAPCR		(CPM_BASE+0x10) /* APLL control Register	*/
#define CPM_CPMPCR		(CPM_BASE+0x14) /* MPLL control Register	*/
#define CPM_CPEPCR		(CPM_BASE+0x18) /* EPLL control Register	*/
#define CPM_CPVPCR		(CPM_BASE+0x1c) /* VPLL control Register	*/
#define CPM_DDCDR		(CPM_BASE+0x2c) /* DDR clock divider register	*/
#define CPM_VPUCDR		(CPM_BASE+0x30) /* VPU clock divider register	*/
#define CPM_CPSPR		(CPM_BASE+0x34) /* CPM scratch pad register		*/
#define CPM_CPSPPR		(CPM_BASE+0x38) /* CPM scratch protected register	*/
#define CPM_USBPCR		(CPM_BASE+0x3c) /* USB parameter control register	*/
#define CPM_USBRDT		(CPM_BASE+0x40) /* USB reset detect timer register	*/
#define CPM_USBVBFIL		(CPM_BASE+0x44) /* USB jitter filter register		*/
#define CPM_USBPCR1		(CPM_BASE+0x48) /* USB parameter control register 1	*/
#define CPM_USBCDR		(CPM_BASE+0x50) /* USB OTG PHY clock divider register	*/
#define CPM_I2SCDR		(CPM_BASE+0x60) /* I2S device clock divider register	*/
#define CPM_I2S1CDR		(CPM_BASE+0xa0) /* I2S1 device clock divider register	*/
#define CPM_LPCDR1		(CPM_BASE+0x64) /* LCD1 pix clock divider register	*/
#define CPM_LPCDR		(CPM_BASE+0x54) /* LCD pix clock divider register	*/
#define CPM_MSCCDR		(CPM_BASE+0x68) /* MSC clock divider register		*/
#define CPM_MSCCDR1		(CPM_BASE+0xa4) /* MSC1 clock divider register		*/
#define CPM_MSCCDR2		(CPM_BASE+0xa8) /* MSC2 clock divider register		*/
#define CPM_UHCCDR		(CPM_BASE+0x6C) /* UHC 48M clock divider register	*/
#define CPM_SSICDR		(CPM_BASE+0x74) /* SSI clock divider register		*/
#define CPM_CIMCDR		(CPM_BASE+0x7c) /* CIM MCLK clock divider register	*/
#define CPM_PCMCDR		(CPM_BASE+0x84) /* PCM device clock divider register	*/
#define CPM_GPUCDR		(CPM_BASE+0x88) /* GPU clock divider register		*/
#define CPM_HDMICDR		(CPM_BASE+0x8C) /* GPU clock divider register		*/
#define CPM_BCHCDR		(CPM_BASE+0xAC) /* BCH clock divider register		*/

#define CPM_LCR			(CPM_BASE+0x04)
#define CPM_SPCR0		(CPM_BASE+0xb8) /* SRAM Power Control Register0 */
#define CPM_SPCR1		(CPM_BASE+0xbc) /* SRAM Power Control Register1 */
#define CPM_PSWCST(n)		(CPM_BASE+0x4*(n)+0x90)
#define CPM_CLKGR0		(CPM_BASE+0x20) /* Clock Gate Register0 */
#define CPM_CLKGR1		(CPM_BASE+0x28) /* Clock Gate Register1 */
#define CPM_OPCR		(CPM_BASE+0x24) /* Oscillator and Power Control Register */

#define CPM_RSR			(CPM_BASE+0x08)

/* Clock control register */
#define CPM_CPCCR_SEL_SRC_BIT		30
#define CPM_CPCCR_SEL_SRC_MASK		(0x3 << CPM_CPCCR_SEL_SRC_BIT)
 #define CPM_SRC_SEL_STOP 0
  #define CPM_SRC_SEL_APLL  1
  #define CPM_SRC_SEL_EXCLK 2
  #define CPM_SRC_SEL_RTCLK 3
#define CPM_CPCCR_SEL_CPLL_BIT		28
#define CPM_CPCCR_SEL_CPLL_MASK		(0x3 << CPM_CPCCR_SEL_CPLL_BIT)
#define CPM_CPCCR_SEL_H0PLL_BIT		26
#define CPM_CPCCR_SEL_H0PLL_MASK		(0x3 << CPM_CPCCR_SEL_H0PLL_BIT)
#define CPM_CPCCR_SEL_H2PLL_BIT		24
#define CPM_CPCCR_SEL_H2PLL_MASK		(0x3 << CPM_CPCCR_SEL_H2PLL_BIT)
  #define CPM_PLL_SEL_STOP  0
  #define CPM_PLL_SEL_SRC   1
  #define CPM_PLL_SEL_MPLL  2
  #define CPM_PLL_SEL_EPLL  3
#define CPM_CPCCR_CE_CPU		(0x1 << 22)
#define CPM_CPCCR_CE_AHB0		(0x1 << 21)
#define CPM_CPCCR_CE_AHB2		(0x1 << 20)
#define CPM_CPCCR_PDIV_BIT		16
#define CPM_CPCCR_PDIV_MASK		(0xf << CPM_CPCCR_PDIV_BIT)
#define CPM_CPCCR_H2DIV_BIT		12
#define CPM_CPCCR_H2DIV_MASK		(0xf << CPM_CPCCR_H2DIV_BIT)
#define CPM_CPCCR_H0DIV_BIT		8
#define CPM_CPCCR_H0DIV_MASK		(0x0f << CPM_CPCCR_H0DIV_BIT)
#define CPM_CPCCR_L2DIV_BIT		4
#define CPM_CPCCR_L2DIV_MASK		(0x0f << CPM_CPCCR_L2DIV_BIT)
#define CPM_CPCCR_CDIV_BIT		0
#define CPM_CPCCR_CDIV_MASK		(0x0f << CPM_CPCCR_CDIV_BIT)

/* Clock Status register */
#define CPM_CPCSR_H2DIV_BUSY		(1 << 2)
#define CPM_CPCSR_H0DIV_BUSY		(1 << 1)
#define CPM_CPCSR_CDIV_BUSY		(1 << 0)

/* PLL control register */
#define CPM_CPPCR_PLLST_BIT		0
#define CPM_CPPCR_PLLST_MASK		(0xff << CPM_CPPCR_PLLST_BIT)

/* XPLL control register */
#define CPM_CPXPCR_XPLLM_BIT		19
#define CPM_CPXPCR_XPLLM_MASK		(0x1fff << CPM_CPXPCR_XPLLM_BIT)
#define CPM_CPXPCR_XPLLN_BIT		13
#define CPM_CPXPCR_XPLLN_MASK		(0x3f << CPM_CPXPCR_XPLLN_BIT)
#define CPM_CPXPCR_XPLLOD_BIT		9
#define CPM_CPXPCR_XPLLOD_MASK		(0xf << CPM_CPXPCR_XPLLOD_BIT)
#define CPM_CPXPCR_XLOCK		(1 << 6)
#define CPM_CPXPCR_XPLL_ON		(1 << 4)
#define CPM_CPXPCR_XF_MODE		(1 << 3)
#define CPM_CPXPCR_XPLLBP		(1 << 1)
#define CPM_CPXPCR_XPLLEN		(1 << 0)

/* CPM scratch protected register */
#define CPM_CPSPPR_BIT			0
#define CPM_CPSPPR_MASK			(0xffff << CPM_CPSPPR_BIT)

/* USB parameter control register */
#define CPM_USBPCR_USB_MODE		(1 << 31)  /* 1: OTG, 0: UDC*/
#define CPM_USBPCR_AVLD_REG		(1 << 30)
#define CPM_USBPCR_IDPULLUP_MASK_BIT	28
#define CPM_USBPCR_IDPULLUP_MASK_MASK	(0x02 << IDPULLUP_MASK_BIT)
#define CPM_USBPCR_INCR_MASK		(1 << 27)
#define CPM_USBPCR_CLK12_EN		(1 << 26)
#define CPM_USBPCR_COMMONONN		(1 << 25)
#define CPM_USBPCR_VBUSVLDEXT		(1 << 24)
#define CPM_USBPCR_VBUSVLDEXTSEL	(1 << 23)
#define CPM_USBPCR_POR			(1 << 22)
#define CPM_USBPCR_SIDDQ		(1 << 21)
#define CPM_USBPCR_OTG_DISABLE		(1 << 20)
#define CPM_USBPCR_COMPDISTUNE_BIT	17
#define CPM_USBPCR_COMPDISTUNE_MASK	(0x07 << COMPDISTUNE_BIT)
#define CPM_USBPCR_OTGTUNE_BIT		14
#define CPM_USBPCR_OTGTUNE_MASK		(0x07 << OTGTUNE_BIT)
#define CPM_USBPCR_SQRXTUNE_BIT		11
#define CPM_USBPCR_SQRXTUNE_MASK	(0x7x << SQRXTUNE_BIT)
#define CPM_USBPCR_TXFSLSTUNE_BIT	7
#define CPM_USBPCR_TXFSLSTUNE_MASK	(0x0f << TXFSLSTUNE_BIT)
#define CPM_USBPCR_TXPREEMPHTUNE	(1 << 6)
#define CPM_USBPCR_TXRISETUNE_BIT	4
#define CPM_USBPCR_TXRISETUNE_MASK	(0x03 << TXRISETUNE_BIT)
#define CPM_USBPCR_TXVREFTUNE_BIT	0
#define CPM_USBPCR_TXVREFTUNE_MASK	(0x0f << TXVREFTUNE_BIT)

/* DDR memory clock divider register */
#define CPM_DDRCDR_DCS_BIT		30
#define CPM_DDRCDR_DCS_MASK		(0x3 << CPM_DDRCDR_DCS_BIT)
  #define CPM_DDRCDR_DCS_STOP		(0x0 << CPM_DDRCDR_DCS_BIT)
  #define CPM_DDRCDR_DCS_SRC		(0x1 << CPM_DDRCDR_DCS_BIT)
  #define CPM_DDRCDR_DCS_MPLL		(0x2 << CPM_DDRCDR_DCS_BIT)
#define CPM_DDRCDR_CE_DDR		(1 << 29)
#define CPM_DDRCDR_DDR_BUSY		(1 << 28)
#define CPM_DDRCDR_DDR_STOP		(1 << 27)
#define CPM_DDRCDR_DDRDIV_BIT		0
#define CPM_DDRCDR_DDRDIV_MASK		(0xf << CPM_DDRCDR_DDRDIV_BIT)

/* USB reset detect timer register */
#define CPM_USBRDT_VBFIL_LD_EN		(1 << 25)
#define CPM_USBRDT_IDDIG_EN		(1 << 24)
#define CPM_USBRDT_IDDIG_REG		(1 << 23)
#define CPM_USBRDT_USBRDT_BIT		0
#define CPM_USBRDT_USBRDT_MASK		(0x7fffff << CPM_USBRDT_USBRDT_BIT)

/* USB OTG PHY clock divider register */
#define CPM_USBCDR_UCS			(1 << 31)
#define CPM_USBCDR_UPCS			(1 << 30)
#define CPM_USBCDR_CEUSB		(1 << 29)
#define CPM_USBCDR_USB_BUSY		(1 << 28)
#define CPM_USBCDR_OTGDIV_BIT		0
#define CPM_USBCDR_OTGDIV_MASK		(0xff << CPM_USBCDR_OTGDIV_BIT)

/* I2S device clock divider register */
#define CPM_I2SCDR_I2CS			(1 << 31)
#define CPM_I2SCDR_I2PCS		(1 << 30)
#define CPM_I2SCDR_I2SDIV_BIT		0
#define CPM_I2SCDR_I2SDIV_MASK		(0x1ff << CPM_I2SCDR_I2SDIV_BIT)

/* LCD0 pix clock divider register */
#define CPM_LPCDR_LPCS_BIT			30
#define CPM_LPCDR_LPCS_MASK			(0x3 << CPM_LPCDR_LPCS_BIT)
#define CPM_LPCDR_CELCD				(1 << 28)
#define CPM_LPCDR_LCD_BUSY			(1 << 27)
#define CPM_LPCDR_LCD_STOP			(1 << 26)
#define CPM_LPCDR_PIXDIV_BIT		0
#define CPM_LPCDR_PIXDIV_MASK		(0xff << CPM_LPCDR_PIXDIV_BIT)

/* MSC clock divider register */
#define CPM_MSCCDR_MPCS_BIT		30
#define CPM_MSCCDR_MPCS_MASK		(3 << CPM_MSCCDR_MPCS_BIT)
  #define CPM_MSCCDR_MPCS_STOP		(0x0 << CPM_MSCCDR_MPCS_BIT)
  #define CPM_MSCCDR_MPCS_SRC		(0x1 << CPM_MSCCDR_MPCS_BIT)
  #define CPM_MSCCDR_MPCS_MPLL		(0x2 << CPM_MSCCDR_MPCS_BIT)
#define CPM_MSCCDR_CE			(1 << 29)
#define CPM_MSCCDR_MSC_BUSY		(1 << 28)
#define CPM_MSCCDR_MSC_STOP		(1 << 27)
#define CPM_MSCCDR_MSC_CLK0_SEL		(1 << 15)
#define CPM_MSCCDR_MSCDIV_BIT		0
#define CPM_MSCCDR_MSCDIV_MASK		(0xff << CPM_MSCCDR_MSCDIV_BIT)

/* UHC 48M clock divider register */
#define CPM_UHCCDR_UHCS_BIT		30
#define CPM_UHCCDR_UHCS_MASK		(0x3 << CPM_UHCCDR_UHCS_BIT)
  #define CPM_UHCCDR_UHCS_SRC		(0x0 << CPM_UHCCDR_UHCS_BIT)
  #define CPM_UHCCDR_UHCS_MPLL		(0x1 << CPM_UHCCDR_UHCS_BIT)
  #define CPM_UHCCDR_UHCS_EPLL		(0x2 << CPM_UHCCDR_UHCS_BIT)
  #define CPM_UHCCDR_UHCS_OTG		(0x3 << CPM_UHCCDR_UHCS_BIT)
#define CPM_UHCCDR_CE_UHC		(1 << 29)
#define CPM_UHCCDR_UHC_BUSY		(1 << 28)
#define CPM_UHCCDR_UHC_STOP		(1 << 27)
#define CPM_UHCCDR_UHCDIV_BIT		0
#define CPM_UHCCDR_UHCDIV_MASK		(0xff << CPM_UHCCDR_UHCDIV_BIT)

/* SSI clock divider register */
#define CPM_SSICDR_SCS			(1 << 31)
#define CPM_SSICDR_SSIDIV_BIT		0
#define CPM_SSICDR_SSIDIV_MASK		(0x3f << CPM_SSICDR_SSIDIV_BIT)

/* CIM MCLK clock divider register */
#define CPM_CIMCDR_CIMDIV_BIT		0
#define CPM_CIMCDR_CIMDIV_MASK		(0xff << CPM_CIMCDR_CIMDIV_BIT)

/* GPS clock divider register */
#define CPM_GPSCDR_GPCS			(1 << 31)
#define CPM_GPSCDR_GPSDIV_BIT		0
#define CPM_GSPCDR_GPSDIV_MASK		(0xf << CPM_GPSCDR_GPSDIV_BIT)

/* PCM device clock divider register */
#define CPM_PCMCDR_PCMS			(1 << 31)
#define CPM_PCMCDR_PCMPCS		(1 << 30)
#define CPM_PCMCDR_PCMDIV_BIT		0
#define CPM_PCMCDR_PCMDIV_MASK		(0x1ff << CPM_PCMCDR_PCMDIV_BIT)

/* GPU clock divider register */
#define CPM_GPUCDR_GPCS			(1 << 31)
#define CPM_GPUCDR_GPUDIV_BIT		0
#define CPM_GPUCDR_GPUDIV_MASK		(0x7 << CPM_GPUCDR_GPUDIV_BIT)

/* HDMI clock divider register */
#define CPM_HDMICDR_HPCS_BIT			30
#define CPM_HDMICDR_HPCS_MASK			(0x3 << CPM_HDMICDR_HPCS_BIT)
#define CPM_HDMICDR_CEHDMI			(1 << 29)
#define CPM_HDMICDR_HDMI_BUSY			(1 << 28)
#define CPM_HDMICDR_HDMI_STOP			(1 << 26)
#define CPM_HDMICDR_HDMIDIV_BIT			0
#define CPM_HDMICDR_HDMIDIV_MASK		(0xff << CPM_HDMICDR_HDMIDIV_BIT)

/* Low Power Control Register */
#define CPM_LCR_PD_SCPU		(1 << 31)
#define CPM_LCR_PD_VPU		(1 << 30)
#define CPM_LCR_PD_GPU		(1 << 29)
#define CPM_LCR_PD_GPS		(1 << 28)
#define CPM_LCR_SCPUS		(1 << 27)
#define CPM_LCR_VPUS		(1 << 26)
#define CPM_LCR_GPUS		(1 << 25)
#define CPM_LCR_GPSS		(1 << 24)
#define CPM_LCR_GPU_IDLE	(1 << 20)
#define CPM_LCR_PST_BIT 	8
#define CPM_LCR_PST_MASK 	(0xfff << CPM_LCR_PST_BIT)
#define CPM_LCR_DOZE_DUTY_BIT 	3
#define CPM_LCR_DOZE_DUTY_MASK 	(0x1f << CPM_LCR_DOZE_DUTY_BIT)
#define CPM_LCR_DOZE_ON		(1 << 2)
#define CPM_LCR_LPM_BIT		0
#define CPM_LCR_LPM_MASK	(0x3 << CPM_LCR_LPM_BIT)
  #define CPM_LCR_LPM_IDLE	(0x0 << CPM_LCR_LPM_BIT)
  #define CPM_LCR_LPM_SLEEP	(0x1 << CPM_LCR_LPM_BIT)

/* Clock Gate Register0 */
#define CPM_CLKGR0_DDR1		(1 << 31)
#define CPM_CLKGR0_DDR0 	(1 << 30)
#define CPM_CLKGR0_IPU    	(1 << 29)
#define CPM_CLKGR0_LCD1		(1 << 28)
#define CPM_CLKGR0_LCD  	(1 << 27)
#define CPM_CLKGR0_CIM    	(1 << 26)
#define CPM_CLKGR0_I2C2    	(1 << 25)
#define CPM_CLKGR0_UHC    	(1 << 24)
#define CPM_CLKGR0_MAC    	(1 << 23)
#define CPM_CLKGR0_GPS    	(1 << 22)
#define CPM_CLKGR0_PDMAC    	(1 << 21)
#define CPM_CLKGR0_SSI2    	(1 << 20)
#define CPM_CLKGR0_SSI1    	(1 << 19)
#define CPM_CLKGR0_UART3    	(1 << 18)
#define CPM_CLKGR0_UART2    	(1 << 17)
#define CPM_CLKGR0_UART1    	(1 << 16)
#define CPM_CLKGR0_UART0	(1 << 15)
#define CPM_CLKGR0_SADC		(1 << 14)
#define CPM_CLKGR0_KBC		(1 << 13)
#define CPM_CLKGR0_MSC2		(1 << 12)
#define CPM_CLKGR0_MSC1		(1 << 11)
#define CPM_CLKGR0_OWI		(1 << 10)
#define CPM_CLKGR0_TSSI		(1 << 9)
#define CPM_CLKGR0_AIC		(1 << 8)
#define CPM_CLKGR0_SCC		(1 << 7)
#define CPM_CLKGR0_I2C1		(1 << 6)
#define CPM_CLKGR0_I2C0		(1 << 5)
#define CPM_CLKGR0_SSI0		(1 << 4)
#define CPM_CLKGR0_MSC0		(1 << 3)
#define CPM_CLKGR0_OTG		(1 << 2)
#define CPM_CLKGR0_BCH		(1 << 1)
#define CPM_CLKGR0_NEMC		(1 << 0)

/* Clock Gate Register1 */
#define CPM_CLKGR1_P1		(1 << 15)
#define CPM_CLKGR1_X2D		(1 << 14)
#define CPM_CLKGR1_DES		(1 << 13)
#define CPM_CLKGR1_I2C4		(1 << 12)
#define CPM_CLKGR1_AHB		(1 << 11)
#define CPM_CLKGR1_UART4	(1 << 10)
#define CPM_CLKGR1_HDMI		(1 << 9)
#define CPM_CLKGR1_OTG1		(1 << 8)
#define CPM_CLKGR1_GPVLC	(1 << 7)
#define CPM_CLKGR1_AIC1 	(1 << 6)
#define CPM_CLKGR1_COMPRES	(1 << 5)
#define CPM_CLKGR1_GPU		(1 << 4)
#define CPM_CLKGR1_PCM		(1 << 3)
#define CPM_CLKGR1_VPU		(1 << 2)
#define CPM_CLKGR1_TSSI1	(1 << 1)
#define CPM_CLKGR1_I2C3		(1 << 0)

/* Oscillator and Power Control Register */
#define CPM_OPCR_O1ST_BIT	8
#define CPM_OPCR_O1ST_MASK	(0xff << CPM_OPCR_O1ST_BIT)
#define CPM_OPCR_SPENDN		(1 << 7)
#define CPM_OPCR_GPSEN		(1 << 6)
#define CPM_OPCR_SPENDH		(1 << 5)
#define CPM_OPCR_O1SE		(1 << 4) /* */
#define CPM_OPCR_ERCS           (1 << 2) /* 0: select EXCLK/512 clock, 1: RTCLK clock */
#define CPM_OPCR_USBM           (1 << 0) /* 0: select EXCLK/512 clock, 1: RTCLK clock */

/* Reset Status Register */
#define CPM_RSR_P0R		(1 << 2)
#define CPM_RSR_WR		(1 << 1)
#define CPM_RSR_PR		(1 << 0)

/* BCH clock divider register */
#define CPM_BCHCDR_BPCS_BIT		30
#define CPM_BCHCDR_BPCS_MASK		(0x3 << CPM_BCHCDR_BPCS_BIT)
  #define CPM_BCHCDR_BPCS_STOP		(0X0 << CPM_BCHCDR_BPCS_BIT)
  #define CPM_BCHCDR_BPCS_SRC_CLK	(0x1 << CPM_BCHCDR_BPCS_BIT)
  #define CPM_BCHCDR_BPCS_MPLL		(0x2 << CPM_BCHCDR_BPCS_BIT)
  #define CPM_BCHCDR_BPCS_EPLL		(0x3 << CPM_BCHCDR_BPCS_BIT)
#define CPM_BCHCDR_CE_BCH		(1 << 29)
#define CPM_BCHCDR_BCH_BUSY		(1 << 28)
#define CPM_BCHCDR_BCH_STOP		(1 << 27)
#define CPM_BCHCDR_BCHCDR_BIT		0
#define CPM_BCHCDR_BCHCDR_MASK		(0x7 << CPM_BCHCDR_BCHCDR_BIT)

/* CPM scratch pad protected register(CPSPPR) */
#define CPSPPR_CPSPR_WRITABLE   (0x00005a5a)
#define RECOVERY_SIGNATURE      (0x1a1a)	/* means "RECY" */
#define RECOVERY_SIGNATURE_SEC  0x800      	/* means "RECY" */

#define REBOOT_SIGNATURE		(0x3535) /* means reboot*/


/*************************************************************************
 * TCU (Timer Counter Unit)
 *************************************************************************/
#define	TCU_TSR		0x1C	/* Timer Stop Register */
#define	TCU_TSSR	0x2C	/* Timer Stop Set Register */
#define	TCU_TSCR	0x3C	/* Timer Stop Clear Register */
#define	TCU_TER		0x10	/* Timer Counter Enable Register */
#define	TCU_TESR	0x14	/* Timer Counter Enable Set Register */
#define	TCU_TECR	0x18	/* Timer Counter Enable Clear Register */
#define	TCU_TFR		0x20	/* Timer Flag Register */
#define	TCU_TFSR	0x24	/* Timer Flag Set Register */
#define	TCU_TFCR	0x28	/* Timer Flag Clear Register */
#define	TCU_TMR		0x30	/* Timer Mask Register */
#define	TCU_TMSR	0x34	/* Timer Mask Set Register */
#define	TCU_TMCR	0x38	/* Timer Mask Clear Register */
#define	TCU_TDFR0	0x40	/* Timer Data Full Register */
#define	TCU_TDHR0	0x44	/* Timer Data Half Register */
#define	TCU_TCNT0	0x48	/* Timer Counter Register */
#define	TCU_TCSR0	0x4C	/* Timer Control Register */
#define	TCU_TDFR1	0x50
#define	TCU_TDHR1	0x54
#define	TCU_TCNT1	0x58
#define	TCU_TCSR1	0x5C
#define	TCU_TDFR2	0x60
#define	TCU_TDHR2	0x64
#define	TCU_TCNT2	0x68
#define	TCU_TCSR2	0x6C
#define	TCU_TDFR3	0x70
#define	TCU_TDHR3	0x74
#define	TCU_TCNT3	0x78
#define	TCU_TCSR3	0x7C
#define	TCU_TDFR4	0x80
#define	TCU_TDHR4	0x84
#define	TCU_TCNT4	0x88
#define	TCU_TCSR4	0x8C
#define	TCU_TDFR5	0x90
#define	TCU_TDHR5	0x94
#define	TCU_TCNT5	0x98
#define	TCU_TCSR5	0x9C

// n = 0,1,2,3,4,5
#define	TCU_TDFR(n)	(0x40 + (n)*0x10)	/* Timer Data Full Reg */
#define	TCU_TDHR(n)	(0x44 + (n)*0x10)	/* Timer Data Half Reg */
#define	TCU_TCNT(n)	(0x48 + (n)*0x10)	/* Timer Counter Reg */
#define	TCU_TCSR(n)	(0x4C + (n)*0x10)	/* Timer Control Reg */

// Register definitions
#define	TCU_TCSR_PWM_SD		(1 << 9)
#define	TCU_TCSR_PWM_INITL_HIGH	(1 << 8)
#define	TCU_TCSR_PWM_EN		(1 << 7)
#define	TCU_TCSR_PRESCALE_BIT	3
#define	TCU_TCSR_PRESCALE_MASK	(0x7 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE1	(0x0 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE4	(0x1 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE16	(0x2 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE64	(0x3 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE256	(0x4 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE1024	(0x5 << TCU_TCSR_PRESCALE_BIT)
#define	TCU_TCSR_EXT_EN		(1 << 2)
#define	TCU_TCSR_RTC_EN		(1 << 1)
#define	TCU_TCSR_PCK_EN		(1 << 0)

#define	TCU_TER_TCEN5		(1 << 5)
#define	TCU_TER_TCEN4		(1 << 4)
#define	TCU_TER_TCEN3		(1 << 3)
#define	TCU_TER_TCEN2		(1 << 2)
#define	TCU_TER_TCEN1		(1 << 1)
#define	TCU_TER_TCEN0		(1 << 0)

#define	TCU_TESR_TCST5		(1 << 5)
#define	TCU_TESR_TCST4		(1 << 4)
#define	TCU_TESR_TCST3		(1 << 3)
#define	TCU_TESR_TCST2		(1 << 2)
#define	TCU_TESR_TCST1		(1 << 1)
#define	TCU_TESR_TCST0		(1 << 0)

#define	TCU_TECR_TCCL5		(1 << 5)
#define	TCU_TECR_TCCL4		(1 << 4)
#define	TCU_TECR_TCCL3		(1 << 3)
#define	TCU_TECR_TCCL2		(1 << 2)
#define	TCU_TECR_TCCL1		(1 << 1)
#define	TCU_TECR_TCCL0		(1 << 0)

#define	TCU_TFR_HFLAG5		(1 << 21)
#define	TCU_TFR_HFLAG4		(1 << 20)
#define	TCU_TFR_HFLAG3		(1 << 19)
#define	TCU_TFR_HFLAG2		(1 << 18)
#define	TCU_TFR_HFLAG1		(1 << 17)
#define	TCU_TFR_HFLAG0		(1 << 16)
#define	TCU_TFR_FFLAG5		(1 << 5)
#define	TCU_TFR_FFLAG4		(1 << 4)
#define	TCU_TFR_FFLAG3		(1 << 3)
#define	TCU_TFR_FFLAG2		(1 << 2)
#define	TCU_TFR_FFLAG1		(1 << 1)
#define	TCU_TFR_FFLAG0		(1 << 0)

#define	TCU_TFSR_HFLAG5		(1 << 21)
#define	TCU_TFSR_HFLAG4		(1 << 20)
#define	TCU_TFSR_HFLAG3		(1 << 19)
#define	TCU_TFSR_HFLAG2		(1 << 18)
#define	TCU_TFSR_HFLAG1		(1 << 17)
#define	TCU_TFSR_HFLAG0		(1 << 16)
#define	TCU_TFSR_FFLAG5		(1 << 5)
#define	TCU_TFSR_FFLAG4		(1 << 4)
#define	TCU_TFSR_FFLAG3		(1 << 3)
#define	TCU_TFSR_FFLAG2		(1 << 2)
#define	TCU_TFSR_FFLAG1		(1 << 1)
#define	TCU_TFSR_FFLAG0		(1 << 0)

#define	TCU_TFCR_HFLAG5		(1 << 21)
#define	TCU_TFCR_HFLAG4		(1 << 20)
#define	TCU_TFCR_HFLAG3		(1 << 19)
#define	TCU_TFCR_HFLAG2		(1 << 18)
#define	TCU_TFCR_HFLAG1		(1 << 17)
#define	TCU_TFCR_HFLAG0		(1 << 16)
#define	TCU_TFCR_FFLAG5		(1 << 5)
#define	TCU_TFCR_FFLAG4		(1 << 4)
#define	TCU_TFCR_FFLAG3		(1 << 3)
#define	TCU_TFCR_FFLAG2		(1 << 2)
#define	TCU_TFCR_FFLAG1		(1 << 1)
#define	TCU_TFCR_FFLAG0		(1 << 0)

#define	TCU_TMR_HMASK5		(1 << 21)
#define	TCU_TMR_HMASK4		(1 << 20)
#define	TCU_TMR_HMASK3		(1 << 19)
#define	TCU_TMR_HMASK2		(1 << 18)
#define	TCU_TMR_HMASK1		(1 << 17)
#define	TCU_TMR_HMASK0		(1 << 16)
#define	TCU_TMR_FMASK5		(1 << 5)
#define	TCU_TMR_FMASK4		(1 << 4)
#define	TCU_TMR_FMASK3		(1 << 3)
#define	TCU_TMR_FMASK2		(1 << 2)
#define	TCU_TMR_FMASK1		(1 << 1)
#define	TCU_TMR_FMASK0		(1 << 0)

#define	TCU_TMSR_HMST5		(1 << 21)
#define	TCU_TMSR_HMST4		(1 << 20)
#define	TCU_TMSR_HMST3		(1 << 19)
#define	TCU_TMSR_HMST2		(1 << 18)
#define	TCU_TMSR_HMST1		(1 << 17)
#define	TCU_TMSR_HMST0		(1 << 16)
#define	TCU_TMSR_FMST5		(1 << 5)
#define	TCU_TMSR_FMST4		(1 << 4)
#define	TCU_TMSR_FMST3		(1 << 3)
#define	TCU_TMSR_FMST2		(1 << 2)
#define	TCU_TMSR_FMST1		(1 << 1)
#define	TCU_TMSR_FMST0		(1 << 0)

#define	TCU_TMCR_HMCL5		(1 << 21)
#define	TCU_TMCR_HMCL4		(1 << 20)
#define	TCU_TMCR_HMCL3		(1 << 19)
#define	TCU_TMCR_HMCL2		(1 << 18)
#define	TCU_TMCR_HMCL1		(1 << 17)
#define	TCU_TMCR_HMCL0		(1 << 16)
#define	TCU_TMCR_FMCL5		(1 << 5)
#define	TCU_TMCR_FMCL4		(1 << 4)
#define	TCU_TMCR_FMCL3		(1 << 3)
#define	TCU_TMCR_FMCL2		(1 << 2)
#define	TCU_TMCR_FMCL1		(1 << 1)
#define	TCU_TMCR_FMCL0		(1 << 0)

#define	TCU_TSR_WDTS		(1 << 16)
#define	TCU_TSR_STOP5		(1 << 5)
#define	TCU_TSR_STOP4		(1 << 4)
#define	TCU_TSR_STOP3		(1 << 3)
#define	TCU_TSR_STOP2		(1 << 2)
#define	TCU_TSR_STOP1		(1 << 1)
#define	TCU_TSR_STOP0		(1 << 0)

#define	TCU_TSSR_WDTSS		(1 << 16)
#define	TCU_TSSR_STPS5		(1 << 5)
#define	TCU_TSSR_STPS4		(1 << 4)
#define	TCU_TSSR_STPS3		(1 << 3)
#define	TCU_TSSR_STPS2		(1 << 2)
#define	TCU_TSSR_STPS1		(1 << 1)
#define	TCU_TSSR_STPS0		(1 << 0)

#define	TCU_TSSR_WDTSC		(1 << 16)
#define	TCU_TSSR_STPC5		(1 << 5)
#define	TCU_TSSR_STPC4		(1 << 4)
#define	TCU_TSSR_STPC3		(1 << 3)
#define	TCU_TSSR_STPC2		(1 << 2)
#define	TCU_TSSR_STPC1		(1 << 1)
#define	TCU_TSSR_STPC0		(1 << 0)

/*************************************************************************
 * WDT (WatchDog Timer)
 *************************************************************************/
#define	WDT_TDR		0x00
#define	WDT_TCER	0x04
#define	WDT_TCNT	0x08
#define	WDT_TCSR	0x0C

// Register definition
#define	WDT_TCSR_PRESCALE_BIT	3
#define	WDT_TCSR_PRESCALE_MASK	(0x7 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE1	(0x0 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE4	(0x1 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE16	(0x2 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE64	(0x3 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE256	(0x4 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE1024	(0x5 << WDT_TCSR_PRESCALE_BIT)
#define	WDT_TCSR_EXT_EN		(1 << 2)
#define	WDT_TCSR_RTC_EN		(1 << 1)
#define	WDT_TCSR_PCK_EN		(1 << 0)

#define	WDT_TCER_TCEN		(1 << 0)

//n = 0,1,2,3,4,5
#define GPIO_PXPIN(n)	(GPIO_BASE + (0x00 + (n)*0x100)) /* PIN Level Register */
#define GPIO_PXINT(n)	(GPIO_BASE + (0x10 + (n)*0x100)) /* Port Interrupt Register */
#define GPIO_PXINTS(n)	(GPIO_BASE + (0x14 + (n)*0x100)) /* Port Interrupt Set Register */
#define GPIO_PXINTC(n)	(GPIO_BASE + (0x18 + (n)*0x100)) /* Port Interrupt Clear Register */
#define GPIO_PXMASK(n)	(GPIO_BASE + (0x20 + (n)*0x100)) /* Port Interrupt Mask Register */
#define GPIO_PXMASKS(n)	(GPIO_BASE + (0x24 + (n)*0x100)) /* Port Interrupt Mask Set Reg */
#define GPIO_PXMASKC(n)	(GPIO_BASE + (0x28 + (n)*0x100)) /* Port Interrupt Mask Clear Reg */
#define GPIO_PXPAT1(n)	(GPIO_BASE + (0x30 + (n)*0x100)) /* Port Pattern 1 Register */
#define GPIO_PXPAT1S(n)	(GPIO_BASE + (0x34 + (n)*0x100)) /* Port Pattern 1 Set Reg. */
#define GPIO_PXPAT1C(n)	(GPIO_BASE + (0x38 + (n)*0x100)) /* Port Pattern 1 Clear Reg. */
#define GPIO_PXPAT0(n)	(GPIO_BASE + (0x40 + (n)*0x100)) /* Port Pattern 0 Register */
#define GPIO_PXPAT0S(n)	(GPIO_BASE + (0x44 + (n)*0x100)) /* Port Pattern 0 Set Register */
#define GPIO_PXPAT0C(n)	(GPIO_BASE + (0x48 + (n)*0x100)) /* Port Pattern 0 Clear Register */
#define GPIO_PXFLG(n)	(GPIO_BASE + (0x50 + (n)*0x100)) /* Port Flag Register */
#define GPIO_PXFLGC(n)	(GPIO_BASE + (0x54 + (n)*0x100)) /* Port Flag clear Register */
#define GPIO_PXOEN(n)	(GPIO_BASE + (0x60 + (n)*0x100)) /* Port Output Disable Register */
#define GPIO_PXOENS(n)	(GPIO_BASE + (0x64 + (n)*0x100)) /* Port Output Disable Set Register */
#define GPIO_PXOENC(n)	(GPIO_BASE + (0x68 + (n)*0x100)) /* Port Output Disable Clear Register */
#define GPIO_PXPEN(n)	(GPIO_BASE + (0x70 + (n)*0x100)) /* Port Pull Disable Register */
#define GPIO_PXPENS(n)	(GPIO_BASE + (0x74 + (n)*0x100)) /* Port Pull Disable Set Register */
#define GPIO_PXPENC(n)	(GPIO_BASE + (0x78 + (n)*0x100)) /* Port Pull Disable Clear Register */
#define GPIO_PXDS(n)	(GPIO_BASE + (0x80 + (n)*0x100)) /* Port Drive Strength Register */
#define GPIO_PXDSS(n)	(GPIO_BASE + (0x84 + (n)*0x100)) /* Port Drive Strength set Register */
#define GPIO_PXDSC(n)	(GPIO_BASE + (0x88 + (n)*0x100)) /* Port Drive Strength clear Register */

#define DDR_MEM_PHY_BASE	0x20000000
#define DDR_PHY_OFFSET	0x1000

#define DDRC_ST		(DDRC_BASE + 0x0) /* DDR Status Register */
#define DDRC_CFG	(DDRC_BASE + 0x4) /* DDR Configure Register */
#define DDRC_CTRL	(DDRC_BASE + 0x8) /* DDR Control Register */
#define DDRC_LMR	(DDRC_BASE + 0xc) /* DDR Load-Mode-Register */
#define DDRC_REFCNT	(DDRC_BASE + 0x18) /* DDR  Auto-Refresh Counter */
#define DDRC_DQS	(DDRC_BASE + 0x1c) /* DDR DQS Delay Control Register */
#define DDRC_DQS_ADJ	(DDRC_BASE + 0x20) /* DDR DQS Delay Adjust Register */
#define DDRC_MMAP0	(DDRC_BASE + 0x24) /* DDR Memory Map Config Register */
#define DDRC_MMAP1	(DDRC_BASE + 0x28) /* DDR Memory Map Config Register */
#define DDRC_MDELAY	(DDRC_BASE + 0x2c) /* DDR Memory Map Config Register */
#define DDRC_CKEL	(DDRC_BASE + 0x30) /* DDR CKE Low if it was set to 0 */
#define DDRC_PMEMCTRL0	(DDRC_BASE + 0x54)
#define DDRC_PMEMCTRL1	(DDRC_BASE + 0x50)
#define DDRC_PMEMCTRL2	(DDRC_BASE + 0x58)
#define DDRC_PMEMCTRL3	(DDRC_BASE + 0x5c)

#define DDRC_TIMING(n)	(DDRC_BASE + 0x60 + 4 * (n - 1)) /* DDR Timing Config Register 1-5 */
#define DDRC_REMMAP(n)	(DDRC_BASE + 0x9c + 4 * (n - 1)) /* DDR Address Remapping Register 1-5 */

#define DDRP_PIR	(DDRC_BASE + DDR_PHY_OFFSET + 0x4) /* PHY Initialization Register */
#define DDRP_PGCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x8) /* PHY General Configuration Register*/
#define DDRP_PGSR	(DDRC_BASE + DDR_PHY_OFFSET + 0xc) /* PHY General Status Register*/

#define DDRP_PTR0	(DDRC_BASE + DDR_PHY_OFFSET + 0x18) /* PHY Timing Register 0 */
#define DDRP_PTR1	(DDRC_BASE + DDR_PHY_OFFSET + 0x1c) /* PHY Timing Register 1 */
#define DDRP_PTR2	(DDRC_BASE + DDR_PHY_OFFSET + 0x20) /* PHY Timing Register 2 */

#define DDRP_ACIOCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x24) /* AC I/O Configuration Register */
#define DDRP_DXCCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x28) /* DATX8 Common Configuration Register */
#define DDRP_DSGCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x2c) /* DDR System General Configuration Register */
#define DDRP_DCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x30) /* DRAM Configuration Register*/

#define DDRP_DTPR0	(DDRC_BASE + DDR_PHY_OFFSET + 0x34) /* DRAM Timing Parameters Register 0 */
#define DDRP_DTPR1	(DDRC_BASE + DDR_PHY_OFFSET + 0x38) /* DRAM Timing Parameters Register 1 */
#define DDRP_DTPR2	(DDRC_BASE + DDR_PHY_OFFSET + 0x3c) /* DRAM Timing Parameters Register 2 */
#define DDRP_MR0	(DDRC_BASE + DDR_PHY_OFFSET + 0x40) /* Mode Register 0 */
#define DDRP_MR1	(DDRC_BASE + DDR_PHY_OFFSET + 0x44) /* Mode Register 1 */
#define DDRP_MR2	(DDRC_BASE + DDR_PHY_OFFSET + 0x48) /* Mode Register 2 */
#define DDRP_MR3	(DDRC_BASE + DDR_PHY_OFFSET + 0x4c) /* Mode Register 3 */

#define DDRP_ODTCR	(DDRC_BASE + DDR_PHY_OFFSET + 0x50) /* ODT Configure Register */
#define DDRP_DTAR	(DDRC_BASE + DDR_PHY_OFFSET + 0x54) /* Data Training Address Register */
#define DDRP_DTDR0	(DDRC_BASE + DDR_PHY_OFFSET + 0x58) /* Data Training Data Register 0 */
#define DDRP_DTDR1	(DDRC_BASE + DDR_PHY_OFFSET + 0x5c) /* Data Training Data Register 1 */

#define DDRP_DCUAR	(DDRC_BASE + DDR_PHY_OFFSET + 0xc0) /* DCU Address Register */
#define DDRP_DCUDR	(DDRC_BASE + DDR_PHY_OFFSET + 0xc4) /* DCU Data Register */
#define DDRP_DCURR	(DDRC_BASE + DDR_PHY_OFFSET + 0xc8) /* DCU Run Register */
#define DDRP_DCULR	(DDRC_BASE + DDR_PHY_OFFSET + 0xcc) /* DCU Loop Register */
#define DDRP_DCUGCR	(DDRC_BASE + DDR_PHY_OFFSET + 0xd0) /* DCU Gerneral Configuration Register */
#define DDRP_DCUTPR	(DDRC_BASE + DDR_PHY_OFFSET + 0xd4) /* DCU Timing Parameters Register */
#define DDRP_DCUSR0	(DDRC_BASE + DDR_PHY_OFFSET + 0xd8) /* DCU Status Register 0 */
#define DDRP_DCUSR1	(DDRC_BASE + DDR_PHY_OFFSET + 0xdc) /* DCU Status Register 1 */

#define DDRP_ZQXCR0(n)  (DDRC_BASE + DDR_PHY_OFFSET + 0x180 + n * 0x10) /* ZQ impedance Control Register 0 */
#define DDRP_ZQXCR1(n)  (DDRC_BASE + DDR_PHY_OFFSET + 0x184 + n * 0x10) /* ZQ impedance Control Register 1 */
#define DDRP_ZQXSR0(n)  (DDRC_BASE + DDR_PHY_OFFSET + 0x188 + n * 0x10) /* ZQ impedance Status Register 0 */
#define DDRP_ZQXSR1(n)  (DDRC_BASE + DDR_PHY_OFFSET + 0x18c + n * 0x10) /* ZQ impedance Status Register 1 */

#define DDRP_DXGCR(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x1c0 + n * 0x40) /* DATX8 n General Configuration Register */
#define DDRP_DXGSR0(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x1c4 + n * 0x40) /* DATX8 n General Status Register */
#define DDRP_DXGSR1(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x1c8 + n * 0x40) /* DATX8 n General Status Register */
#define DDRP_DXDQSTR(n)	(DDRC_BASE + DDR_PHY_OFFSET + 0x1d4 + n * 0x40) /* DATX8 n DQS Timing Register */

/* DDRC Status Register */
#define DDRC_ST_ENDIAN	(1 << 7) /* 0 Little data endian
					    1 Big data endian */
#define DDRC_ST_DPDN		(1 << 5) /* 0 DDR memory is NOT in deep-power-down state
					    1 DDR memory is in deep-power-down state */
#define DDRC_ST_PDN		(1 << 4) /* 0 DDR memory is NOT in power-down state
					    1 DDR memory is in power-down state */
#define DDRC_ST_AREF		(1 << 3) /* 0 DDR memory is NOT in auto-refresh state
					    1 DDR memory is in auto-refresh state */
#define DDRC_ST_SREF		(1 << 2) /* 0 DDR memory is NOT in self-refresh state
					    1 DDR memory is in self-refresh state */
#define DDRC_ST_CKE1		(1 << 1) /* 0 CKE1 Pin is low
					    1 CKE1 Pin is high */
#define DDRC_ST_CKE0		(1 << 0) /* 0 CKE0 Pin is low
					    1 CKE0 Pin is high */

/* DDRC Configure Register */
#define DDRC_CFG_ROW1_BIT	27 /* Row Address width. */
#define DDRC_CFG_ROW1_MASK	(0x7 << DDRC_CFG_ROW1_BIT)
#define DDRC_CFG_COL1_BIT	24 /* Row Address width. */
#define DDRC_CFG_COL1_MASK	(0x7 << DDRC_CFG_COL1_BIT)
#define DDRC_CFG_BA1		(1 << 23)
#define DDRC_CFG_IMBA		(1 << 22)
#define DDRC_CFG_BL_8		(1 << 21)

#define DDRC_CFG_TYPE_BIT	17
#define DDRC_CFG_TYPE_MASK	(0x7 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_DDR1	(2 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_MDDR	(3 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_DDR2	(4 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_LPDDR2	(5 << DDRC_CFG_TYPE_BIT)
  #define DDRC_CFG_TYPE_DDR3	(6 << DDRC_CFG_TYPE_BIT)

#define DDRC_CFG_ODT_EN		(1 << 16)  /* ODT EN */

#define DDRC_CFG_MPRT		(1 << 15)  /* mem protect */

#define DDRC_CFG_ROW_BIT	11 /* Row Address width. */
#define DDRC_CFG_ROW_MASK	(0x7 << DDRC_CFG_ROW_BIT)
  #define DDRC_CFG_ROW_12	(0 << DDRC_CFG_ROW_BIT) /* 12-bit row address is used */
  #define DDRC_CFG_ROW_13	(1 << DDRC_CFG_ROW_BIT) /* 13-bit row address is used */
  #define DDRC_CFG_ROW_14	(2 << DDRC_CFG_ROW_BIT) /* 14-bit row address is used */

#define DDRC_CFG_COL_BIT	8 /* Column Address width.
				     Specify the Column address width of external DDR. */
#define DDRC_CFG_COL_MASK	(0x7 << DDRC_CFG_COL_BIT)
  #define DDRC_CFG_COL_8	(0 << DDRC_CFG_COL_BIT) /* 8-bit Column address is used */
  #define DDRC_CFG_COL_9	(1 << DDRC_CFG_COL_BIT) /* 9-bit Column address is used */
  #define DDRC_CFG_COL_10	(2 << DDRC_CFG_COL_BIT) /* 10-bit Column address is used */
  #define DDRC_CFG_COL_11	(3 << DDRC_CFG_COL_BIT) /* 11-bit Column address is used */

#define DDRC_CFG_CS1EN		(1 << 7) /* 0 DDR Pin CS1 un-used
					    1 There're DDR memory connected to CS1 */
#define DDRC_CFG_CS0EN		(1 << 6) /* 0 DDR Pin CS0 un-used
					    1 There're DDR memory connected to CS0 */
#define DDRC_CFG_CL_BIT		2 /* CAS Latency */
#define DDRC_CFG_CL_MASK	(0xf << DDRC_CFG_CL_BIT)
#define DDRC_CFG_CL_3		(0 << DDRC_CFG_CL_BIT) /* CL = 3 tCK */
#define DDRC_CFG_CL_4		(1 << DDRC_CFG_CL_BIT) /* CL = 4 tCK */
#define DDRC_CFG_CL_5		(2 << DDRC_CFG_CL_BIT) /* CL = 5 tCK */
#define DDRC_CFG_CL_6		(3 << DDRC_CFG_CL_BIT) /* CL = 6 tCK */

#define DDRC_CFG_BA		(1 << 1) /* 0 4 bank device, Pin ba[1:0] valid, ba[2] un-used
					    1 8 bank device, Pin ba[2:0] valid*/
#define DDRC_CFG_DW		(1 << 0) /*0 External memory data width is 16-bit
					   1 External memory data width is 32-bit */

/* DDRC Control Register */
#define DDRC_CTRL_DFI_RST	(1 << 23)
#define DDRC_CTRL_DLL_RST	(1 << 22)
#define DDRC_CTRL_CTL_RST	(1 << 21)
#define DDRC_CTRL_CFG_RST	(1 << 20)
#define DDRC_CTRL_ACTPD		(1 << 15) /* 0 Precharge all banks before entering power-down
					     1 Do not precharge banks before entering power-down */
#define DDRC_CTRL_PDT_BIT	12 /* Power-Down Timer */
#define DDRC_CTRL_PDT_MASK	(0x7 << DDRC_CTRL_PDT_BIT)
  #define DDRC_CTRL_PDT_DIS	(0 << DDRC_CTRL_PDT_BIT) /* power-down disabled */
  #define DDRC_CTRL_PDT_8	(1 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 8 tCK idle */
  #define DDRC_CTRL_PDT_16	(2 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 16 tCK idle */
  #define DDRC_CTRL_PDT_32	(3 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 32 tCK idle */
  #define DDRC_CTRL_PDT_64	(4 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 64 tCK idle */
  #define DDRC_CTRL_PDT_128	(5 << DDRC_CTRL_PDT_BIT) /* Enter power-down after 128 tCK idle */

#define DDRC_CTRL_PRET_BIT	8 /* Precharge Timer */
#define DDRC_CTRL_PRET_MASK	(0x7 << DDRC_CTRL_PRET_BIT) /*  */
  #define DDRC_CTRL_PRET_DIS	(0 << DDRC_CTRL_PRET_BIT) /* PRET function Disabled */
  #define DDRC_CTRL_PRET_8	(1 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 8 tCK idle */
  #define DDRC_CTRL_PRET_16	(2 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 16 tCK idle */
  #define DDRC_CTRL_PRET_32	(3 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 32 tCK idle */
  #define DDRC_CTRL_PRET_64	(4 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 64 tCK idle */
  #define DDRC_CTRL_PRET_128	(5 << DDRC_CTRL_PRET_BIT) /* Precharge active bank after 128 tCK idle */

#define DDRC_CTRL_DPD		(1 << 6) /* 1 Drive external MDDR device entering Deep-Power-Down mode */

#define DDRC_CTRL_SR		(1 << 5) /* 1 Drive external DDR device entering self-refresh mode
					    0 Drive external DDR device exiting self-refresh mode */
#define DDRC_CTRL_UNALIGN	(1 << 4) /* 0 Disable unaligned transfer on AXI BUS
					    1 Enable unaligned transfer on AXI BUS */
#define DDRC_CTRL_ALH		(1 << 3) /* Advanced Latency Hiding:
					    0 Disable ALH
					    1 Enable ALH */
#define DDRC_CTRL_RDC		(1 << 2) /* 0 dclk clock frequency is lower than 60MHz
					    1 dclk clock frequency is higher than 60MHz */
#define DDRC_CTRL_CKE		(1 << 1) /* 0 Not set CKE Pin High
					    1 Set CKE Pin HIGH */
#define DDRC_CTRL_RESET	(1 << 0) /* 0 End resetting ddrc_controller
					    1 Resetting ddrc_controller */


/* DDRC Load-Mode-Register */
#define DDRC_LMR_DDR_ADDR_BIT	16 /* When performing a DDR command, DDRC_ADDR[13:0]
					      corresponding to external DDR address Pin A[13:0] */
#define DDRC_LMR_DDR_ADDR_MASK	(0x3fff << DDRC_LMR_DDR_ADDR_BIT)

#define DDRC_LMR_BA_BIT		8 /* When performing a DDR command, BA[2:0]
				     corresponding to external DDR address Pin BA[2:0]. */
#define DDRC_LMR_BA_MASK	(0x7 << DDRC_LMR_BA_BIT)
  /* For DDR2 */
  #define DDRC_LMR_BA_MRS	(0 << DDRC_LMR_BA_BIT) /* Mode Register set */
  #define DDRC_LMR_BA_EMRS1	(1 << DDRC_LMR_BA_BIT) /* Extended Mode Register1 set */
  #define DDRC_LMR_BA_EMRS2	(2 << DDRC_LMR_BA_BIT) /* Extended Mode Register2 set */
  #define DDRC_LMR_BA_EMRS3	(3 << DDRC_LMR_BA_BIT) /* Extended Mode Register3 set */
  /* For mobile DDR */
  #define DDRC_LMR_BA_M_MRS	(0 << DDRC_LMR_BA_BIT) /* Mode Register set */
  #define DDRC_LMR_BA_M_EMRS	(2 << DDRC_LMR_BA_BIT) /* Extended Mode Register set */
  #define DDRC_LMR_BA_M_SR	(1 << DDRC_LMR_BA_BIT) /* Status Register set */
  /* For Normal DDR1 */
  #define DDRC_LMR_BA_N_MRS	(0 << DDRC_LMR_BA_BIT) /* Mode Register set */
  #define DDRC_LMR_BA_N_EMRS	(1 << DDRC_LMR_BA_BIT) /* Extended Mode Register set */

#define DDRC_LMR_CMD_BIT	4
#define DDRC_LMR_CMD_MASK	(0x3 << DDRC_LMR_CMD_BIT)
  #define DDRC_LMR_CMD_PREC	(0 << DDRC_LMR_CMD_BIT)/* Precharge one bank/All banks */
  #define DDRC_LMR_CMD_AUREF	(1 << DDRC_LMR_CMD_BIT)/* Auto-Refresh */
  #define DDRC_LMR_CMD_LMR	(2 << DDRC_LMR_CMD_BIT)/* Load Mode Register */

#define DDRC_LMR_START		(1 << 0) /* 0 No command is performed
						    1 On the posedge of START, perform a command
						    defined by CMD field */

/* DDRC Timing Config Register 1 */
#define DDRC_TIMING1_TRTP_BIT	24 /* READ to PRECHARGE command period. */
#define DDRC_TIMING1_TRTP_MASK	(0x3f << DDRC_TIMING1_TRTP_BIT)
#define DDRC_TIMING1_TWTR_BIT	16 /* WRITE to READ command delay. */
#define DDRC_TIMING1_TWTR_MASK	(0x3f << DDRC_TIMING1_TWTR_BIT)
  #define DDRC_TIMING1_TWTR_1		(0 << DDRC_TIMING1_TWTR_BIT)
  #define DDRC_TIMING1_TWTR_2		(1 << DDRC_TIMING1_TWTR_BIT)
  #define DDRC_TIMING1_TWTR_3		(2 << DDRC_TIMING1_TWTR_BIT)
  #define DDRC_TIMING1_TWTR_4		(3 << DDRC_TIMING1_TWTR_BIT)
#define DDRC_TIMING1_TWR_BIT 	8 /* WRITE Recovery Time defined by register MR of DDR2 DDR3 memory */
#define DDRC_TIMING1_TWR_MASK	(0x3f << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_1		(0 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_2		(1 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_3		(2 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_4		(3 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_5		(4 << DDRC_TIMING1_TWR_BIT)
  #define DDRC_TIMING1_TWR_6		(5 << DDRC_TIMING1_TWR_BIT)
#define DDRC_TIMING1_TWL_BIT 	0 /* Write latency = RL - 1 */
#define DDRC_TIMING1_TWL_MASK	(0x3f << DDRC_TIMING1_TWL_BIT)

/* DDRC Timing Config Register 2 */
#define DDRC_TIMING2_TCCD_BIT 	24 /* CAS# to CAS# command delay */
#define DDRC_TIMING2_TCCD_MASK 	(0x3f << DDRC_TIMING2_TCCD_BIT)
#define DDRC_TIMING2_TRAS_BIT 	16 /* ACTIVE to PRECHARGE command period (2 * tRAS + 1) */
#define DDRC_TIMING2_TRAS_MASK 	(0x3f << DDRC_TIMING2_TRAS_BIT)
#define DDRC_TIMING2_TRCD_BIT	8  /* ACTIVE to READ or WRITE command period. */
#define DDRC_TIMING2_TRCD_MASK	(0x3f << DDRC_TIMING2_TRCD_BIT)
#define DDRC_TIMING2_TRL_BIT	0  /* Read latency = AL + CL*/
#define DDRC_TIMING2_TRL_MASK	(0x3f << DDRC_TIMING2_TRL_BIT)

/* DDRC Timing Config Register 3 */
#define DDRC_TIMING3_ONUM   27
#define DDRC_TIMING3_TCKSRE_BIT		24 /* Valid clock after enter self-refresh. */
#define DDRC_TIMING3_TCKSRE_MASK 	(0x3f << DDRC_TIMING3_TCKSRE_BIT)
#define DDRC_TIMING3_TRP_BIT	16 /* PRECHARGE command period. */
#define DDRC_TIMING3_TRP_MASK 	(0x3f << DDRC_TIMING3_TRP_BIT)
#define DDRC_TIMING3_TRRD_BIT	8 /* ACTIVE bank A to ACTIVE bank B command period. */
#define DDRC_TIMING3_TRRD_MASK	(0x3f << DDRC_TIMING3_TRRD_BIT)
  #define DDRC_TIMING3_TRRD_DISABLE	(0 << DDRC_TIMING3_TRRD_BIT)
  #define DDRC_TIMING3_TRRD_2		(1 << DDRC_TIMING3_TRRD_BIT)
  #define DDRC_TIMING3_TRRD_3		(2 << DDRC_TIMING3_TRRD_BIT)
  #define DDRC_TIMING3_TRRD_4		(3 << DDRC_TIMING3_TRRD_BIT)
#define DDRC_TIMING3_TRC_BIT 	0 /* ACTIVE to ACTIVE command period. */
#define DDRC_TIMING3_TRC_MASK 	(0x3f << DDRC_TIMING3_TRC_BIT)

/* DDRC Timing Config Register 4 */
#define DDRC_TIMING4_TRFC_BIT         24 /* AUTO-REFRESH command period. */
#define DDRC_TIMING4_TRFC_MASK        (0x3f << DDRC_TIMING4_TRFC_BIT)
#define DDRC_TIMING4_TEXTRW_BIT	      21 /* ??? */
#define DDRC_TIMING4_TEXTRW_MASK      (0x7 << DDRC_TIMING4_TEXTRW_BIT)
#define DDRC_TIMING4_TRWCOV_BIT	      19 /* ??? */
#define DDRC_TIMING4_TRWCOV_MASK      (0x3 << DDRC_TIMING4_TRWCOV_BIT)
#define DDRC_TIMING4_TCKE_BIT	      16 /* ??? */
#define DDRC_TIMING4_TCKE_MASK        (0x7 << DDRC_TIMING4_TCKE_BIT)
#define DDRC_TIMING4_TMINSR_BIT       8  /* Minimum Self-Refresh / Deep-Power-Down time */
#define DDRC_TIMING4_TMINSR_MASK      (0xf << DDRC_TIMING4_TMINSR_BIT)
#define DDRC_TIMING4_TXP_BIT          4  /* EXIT-POWER-DOWN to next valid command period. */
#define DDRC_TIMING4_TXP_MASK         (0x7 << DDRC_TIMING4_TXP_BIT)
#define DDRC_TIMING4_TMRD_BIT         0  /* Load-Mode-Register to next valid command period. */
#define DDRC_TIMING4_TMRD_MASK        (0x3 << DDRC_TIMING4_TMRD_BIT)

/* DDRC Timing Config Register 5 */
#define DDRC_TIMING5_TCTLUPD_BIT	24 /* ??? */
#define DDRC_TIMING4_TCTLUPD_MASK	(0x3f << DDRC_TIMING5_TCTLUDP_BIT)
#define DDRC_TIMING5_TRTW_BIT		16 /* ??? */
#define DDRC_TIMING5_TRTW_MASK		(0x3f << DDRC_TIMING5_TRTW_BIT)
#define DDRC_TIMING5_TRDLAT_BIT		8 /* RL - 2 */
#define DDRC_TIMING5_TRDLAT_MASK	(0x3f << DDRC_TIMING5_TRDLAT_BIT)
#define DDRC_TIMING5_TWDLAT_BIT		0 /* WL - 1 */
#define DDRC_TIMING5_TWDLAT_MASK	(0x3f << DDRC_TIMING5_TWDLAT_BIT)

/* DDRC Timing Config Register 6 */
#define DDRC_TIMING6_TXSRD_BIT		24 /* exit power-down to READ delay */
#define DDRC_TIMING6_TXSRD_MASK		(0x3f << DDRC_TIMING6_TXSRD_BIT)
#define DDRC_TIMING6_TFAW_BIT		16 /* 4-active command window */
#define DDRC_TIMING6_TFAW_MASK		(0x3f << DDRC_TIMING6_TFAW_BIT)
#define DDRC_TIMING6_TCFGW_BIT		8 /* Write PHY configure registers to other commands delay */
#define DDRC_TIMING6_TCFGW_MASK		(0x3f << DDRC_TIMING6_TCFGW_BIT)
#define DDRC_TIMING6_TCFGR_BIT		0 /* Ready PHY configure registers to other commands delay */
#define DDRC_TIMING6_TCFGR_MASK		(0x3f << DDRC_TIMING6_TCFGR_BIT)

/* DDRC  Auto-Refresh Counter */
#define DDRC_REFCNT_CON_BIT           16 /* Constant value used to compare with CNT value. */
#define DDRC_REFCNT_CON_MASK          (0xff << DDRC_REFCNT_CON_BIT)
#define DDRC_REFCNT_CNT_BIT           8  /* 8-bit counter */
#define DDRC_REFCNT_CNT_MASK          (0xff << DDRC_REFCNT_CNT_BIT)
#define DDRC_REFCNT_CLKDIV_BIT        1  /* Clock Divider for auto-refresh counter. */
#define DDRC_REFCNT_CLKDIV_MASK       (0x7 << DDRC_REFCNT_CLKDIV_BIT)
#define DDRC_REFCNT_REF_EN            (1 << 0) /* Enable Refresh Counter */

/* DDRC DQS Delay Control Register */
#define DDRC_DQS_ERROR                (1 << 29) /* ahb_clk Delay Detect ERROR, read-only. */
#define DDRC_DQS_READY                (1 << 28) /* ahb_clk Delay Detect READY, read-only. */
#define DDRC_DQS_AUTO                 (1 << 23) /* Hardware auto-detect & set delay line */
#define DDRC_DQS_DET                  (1 << 24) /* Start delay detecting. */
#define DDRC_DQS_SRDET                (1 << 25) /* Hardware auto-redetect & set delay line. */
#define DDRC_DQS_CLKD_BIT             16 /* CLKD is reference value for setting WDQS and RDQS.*/
#define DDRC_DQS_CLKD_MASK            (0x3f << DDRC_DQS_CLKD_BIT)
#define DDRC_DQS_WDQS_BIT             8  /* Set delay element number to write DQS delay-line. */
#define DDRC_DQS_WDQS_MASK            (0x3f << DDRC_DQS_WDQS_BIT)
#define DDRC_DQS_RDQS_BIT             0  /* Set delay element number to read DQS delay-line. */
#define DDRC_DQS_RDQS_MASK            (0x3f << DDRC_DQS_RDQS_BIT)

/* DDRC DQS Delay Adjust Register */
#define DDRC_DQS_ADJWDQS_BIT          8 /* The adjust value for WRITE DQS delay */
#define DDRC_DQS_ADJWDQS_MASK         (0x1f << DDRC_DQS_ADJWDQS_BIT)
#define DDRC_DQS_ADJRDQS_BIT          0 /* The adjust value for READ DQS delay */
#define DDRC_DQS_ADJRDQS_MASK         (0x1f << DDRC_DQS_ADJRDQS_BIT)

/* DDRC Memory Map Config Register */
#define DDRC_MMAP_BASE_BIT            8 /* base address */
#define DDRC_MMAP_BASE_MASK           (0xff << DDRC_MMAP_BASE_BIT)
#define DDRC_MMAP_MASK_BIT            0 /* address mask */
#define DDRC_MMAP_MASK_MASK           (0xff << DDRC_MMAP_MASK_BIT)

#define DDRC_MMAP0_BASE		     (0x20 << DDRC_MMAP_BASE_BIT)
#define DDRC_MMAP1_BASE_64M	(0x24 << DDRC_MMAP_BASE_BIT) /*when bank0 is 128M*/
#define DDRC_MMAP1_BASE_128M	(0x28 << DDRC_MMAP_BASE_BIT) /*when bank0 is 128M*/
#define DDRC_MMAP1_BASE_256M	(0x30 << DDRC_MMAP_BASE_BIT) /*when bank0 is 128M*/

#define DDRC_MMAP_MASK_64_64	(0xfc << DDRC_MMAP_MASK_BIT)  /*mask for two 128M SDRAM*/
#define DDRC_MMAP_MASK_128_128	(0xf8 << DDRC_MMAP_MASK_BIT)  /*mask for two 128M SDRAM*/
#define DDRC_MMAP_MASK_256_256	(0xf0 << DDRC_MMAP_MASK_BIT)  /*mask for two 128M SDRAM*/

/* DDRP PHY Initialization Register */
#define DDRP_PIR_INIT		(1 << 0)
#define DDRP_PIR_DLLSRST	(1 << 1)
#define DDRP_PIR_DLLLOCK	(1 << 2)
#define DDRP_PIR_ZCAL   	(1 << 3)
#define DDRP_PIR_ITMSRST   	(1 << 4)
#define DDRP_PIR_DRAMRST   	(1 << 5)
#define DDRP_PIR_DRAMINT   	(1 << 6)
#define DDRP_PIR_QSTRN   	(1 << 7)
#define DDRP_PIR_EYETRN   	(1 << 8)
#define DDRP_PIR_DLLBYP   	(1 << 17)
/* DDRP PHY General Configurate Register */
#define DDRP_PGCR_ITMDMD	(1 << 0)
#define DDRP_PGCR_DQSCFG	(1 << 1)
#define DDRP_PGCR_DFTCMP	(1 << 2)
#define DDRP_PGCR_DFTLMT_BIT	3
#define DDRP_PGCR_DTOSEL_BIT	5
#define DDRP_PGCR_CKEN_BIT	9
#define DDRP_PGCR_CKDV_BIT	12
#define DDRP_PGCR_CKINV		(1 << 14)
#define DDRP_PGCR_RANKEN_BIT	18
#define DDRP_PGCR_ZCKSEL_32	(2 << 22)
#define DDRP_PGCR_PDDISDX	(1 << 24)
/* DDRP PHY General Status Register */
#define DDRP_PGSR_IDONE		(1 << 0)
#define DDRP_PGSR_DLDONE	(1 << 1)
#define DDRP_PGSR_ZCDONE	(1 << 2)
#define DDRP_PGSR_DIDONE	(1 << 3)
#define DDRP_PGSR_DTDONE	(1 << 4)
#define DDRP_PGSR_DTERR 	(1 << 5)
#define DDRP_PGSR_DTIERR 	(1 << 6)
#define DDRP_PGSR_DFTEERR 	(1 << 7)
/* DDRP DRAM Configuration Register */
#define DDRP_DCR_TYPE_BIT	0
#define DDRP_DCR_TYPE_MASK	(0x7 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_MDDR	(0 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_DDR	(1 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_DDR2	(2 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_DDR3	(3 << DDRP_DCR_TYPE_BIT)
  #define DDRP_DCR_TYPE_LPDDR2	(4 << DDRP_DCR_TYPE_BIT)
#define DDRP_DCR_DDR8BNK_BIT	3
#define DDRP_DCR_DDR8BNK_MASK	(1 << DDRP_DCR_DDR8BNK_BIT)
  #define DDRP_DCR_DDR8BNK	(1 << DDRP_DCR_DDR8BNK_BIT)
  #define DDRP_DCR_DDR8BNK_DIS	(0 << DDRP_DCR_DDR8BNK_BIT)
/* DDRP PHY Timing Register 0 */
/* DDRP PHY Timing Register 1 */
/* DDRP PHY Timing Register 2 */
/* DDRP DRAM Timing Parameters Register 0 */
/* DDRP DRAM Timing Parameters Register 1 */
#define DRP_DTRP1_RTODT  (1 << 11)    /* ODT may not be turned on until one clock after the read post-amble */
/* DDRP DRAM Timing Parameters Register 2 */
/* DDRP DATX8 n General Configuration Register */
#define DDRP_DXGCR_DXEN  (1 << 0)    /* Data Byte Enable */

#define DDRP_ZQXCR_ZDEN_BIT	28
#define DDRP_ZQXCR_ZDEN		(1 << DDRP_ZQXCR_ZDEN_BIT)
#define DDRP_ZQXCR_PULLUP_IMPE_BIT	5
#define DDRP_ZQXCR_PULLDOWN_IMPE_BIT	0

/* DDR3 Mode Register Set*/
#define DDR3_MR0_BL_BIT		0
#define DDR3_MR0_BL_MASK	(3 << DDR3_MR0_BL_BIT)
  #define DDR3_MR0_BL_8		(0 << DDR3_MR0_BL_BIT)
  #define DDR3_MR0_BL_fly	(1 << DDR3_MR0_BL_BIT)
  #define DDR3_MR0_BL_4		(2 << DDR3_MR0_BL_BIT)
#define DDR3_MR0_BT_BIT		3
#define DDR3_MR0_BT_MASK	(1 << DDR3_MR0_BT_BIT)
  #define DDR3_MR0_BT_SEQ 	(0 << DDR3_MR0_BT_BIT)
  #define DDR3_MR0_BT_INTER 	(1 << DDR3_MR0_BT_BIT)
#define DDR3_MR0_WR_BIT		9

#define DDR3_MR1_DLL_DISABLE	1
#define DDR3_MR1_DIC_6 		(0 << 5 | 0 << 1)
#define DDR3_MR1_DIC_7 		(0 << 5 | 1 << 1)
#define DDR3_MR1_RTT_DIS	(0 << 9 | 0 << 6 | 0 << 2)
#define DDR3_MR1_RTT_4 		(0 << 9 | 0 << 6 | 1 << 2)
#define DDR3_MR1_RTT_2 		(0 << 9 | 1 << 6 | 0 << 2)
#define DDR3_MR1_RTT_6 		(0 << 9 | 1 << 6 | 1 << 2)
#define DDR3_MR1_RTT_12 	(1 << 9 | 0 << 6 | 0 << 2)
#define DDR3_MR1_RTT_8 		(1 << 9 | 0 << 6 | 1 << 2)

#define DDR3_MR2_CWL_BIT	3



/*
 * PLL setup
 */

#if !defined(JZ4780_APLL_M) && !defined(JZ4780_APLL_N) && !defined(JZ4780_APLL_OD)
# define JZ4780_APLL_M  1
# define JZ4780_APLL_N  1
# define JZ4780_APLL_OD 1
#endif

#if !defined(JZ4780_MPLL_M) && !defined(JZ4780_MPLL_N) && !defined(JZ4780_MPLL_OD)
# define JZ4780_MPLL_M  (CONFIG_SYS_MEM_SPEED / CONFIG_SYS_EXTAL * 2)
# define JZ4780_MPLL_N  2
# define JZ4780_MPLL_OD 1
#endif

#if !defined(JZ4780_EPLL_M) && !defined(JZ4780_EPLL_N) && !defined(JZ4780_EPLL_OD)
# define JZ4780_EPLL_M  (CONFIG_SYS_AUDIO_SPEED * 2 / CONFIG_SYS_EXTAL)
# define JZ4780_EPLL_N  1
# define JZ4780_EPLL_OD 2
#endif

#if !defined(JZ4780_VPLL_M) && !defined(JZ4780_VPLL_N) && !defined(JZ4780_VPLL_OD)
# define JZ4780_VPLL_M  ((888 * 1000000) * 2 / CONFIG_SYS_EXTAL)
# define JZ4780_VPLL_N  1
# define JZ4780_VPLL_OD 2
#endif

#ifndef CONFIG_SYS_CPU_PLL
# define CONFIG_SYS_CPU_PLL MPLL
#endif

#ifndef CONFIG_SYS_MEM_PLL
# define CONFIG_SYS_MEM_PLL MPLL
#endif

#ifndef __ASSEMBLY__

#include <asm/io.h>

static inline void gpio_port_set(int port, int pin, int value)
{
	if (value)
		writel(1 << pin, GPIO_PXPAT0S(port));
	else
		writel(1 << pin, GPIO_PXPAT0C(port));
}

static inline void gpio_port_direction_input(int port, int pin)
{
	writel(1 << pin, GPIO_PXINTC(port));
	writel(1 << pin, GPIO_PXMASKS(port));
	writel(1 << pin, GPIO_PXPAT1S(port));
	writel(1 << pin, GPIO_PXPENS(port));
}

static inline void gpio_port_direction_output(int port, int pin, int value)
{
	writel(1 << pin, GPIO_PXINTC(port));
	writel(1 << pin, GPIO_PXMASKS(port));
	writel(1 << pin, GPIO_PXPAT1C(port));

	gpio_port_set(port, pin, value);
}

static inline void gpio_set(int gpio, int value)
{
	int port = gpio / 32;
	int pin = gpio % 32;
	gpio_port_set(port, pin, value);
}

static inline void gpio_direction_input(int gpio)
{
	int port = gpio / 32;
	int pin = gpio % 32;
	gpio_port_direction_input(port, pin);
}

static inline void gpio_direction_output(int gpio, int value)
{
	int port = gpio / 32;
	int pin = gpio % 32;
	gpio_port_direction_output(port, pin, value);
}

extern uint32_t sdram_size(int bank);

#endif /* __ASSEMBLY__ */

#endif	/* __JZ4780_H__ */
