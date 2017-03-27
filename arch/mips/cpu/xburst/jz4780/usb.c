#include <serial.h>
#include <asm/arch/jz4780.h>
#include <asm/arch/jz4780_otg.h>

uint32_t otg_readl(uint32_t off)
{
	return readl((void __iomem *)OTG_BASE + off);
}

void otg_writel(uint32_t val, uint32_t off)
{
	writel(val, (void __iomem *)OTG_BASE + off);
}

void otg_setl(uint32_t val, uint32_t off)
{
	otg_writel(otg_readl(off) | val, off);
}

void otg_clearl(uint32_t val, uint32_t off)
{
	otg_writel(otg_readl(off) & ~val, off);
}

static inline void setl(uint32_t val, uint32_t off)
{
	writel(readl(off) | val, off);
}

static inline void clearl(uint32_t val, uint32_t off)
{
	writel(readl(off) & ~val, off);
}

/* extern void udelay(unsigned usecs); */

static void dwc_otg_cpm_init(void)
{
	/* Set clock frequency to required 48MHz */
	clearl(CPM_USBPCR1_REF_CLK_SEL_LOW, CPM_USBPCR1);
	setl(CPM_USBPCR1_REF_CLK_SEL_HIGH, CPM_USBPCR1);

	/* Select Synopsis OTG rather than Mentor OTG */
	setl(CPM_USBPCR1_USB_SEL, CPM_USBPCR1);

	/* Operate in USB mode rather than OTG */
	clearl(CPM_USBPCR_USB_MODE, CPM_USBPCR);
	/* Control whether the device gets power (V) from the bus. */
	setl(USBPCR_VBUSVLDEXT, CPM_USBPCR);

	/* This bit controls OTG PHY Power On Reset (POR) */
	setl(USBPCR_POR, CPM_USBPCR);
	udelay(30);
	/* This bit controls OTG PHY Power On Reset (POR) */
	clearl(USBPCR_POR, CPM_USBPCR);
	udelay(300);
}

static int dwc_get_utmi_width(void)
{
	return (otg_readl(GHW_CFG4) >> 14) & 0x3;
}

static void dwc_otg_select_phy_width(void)
{
// fixme full speed use 16bit phy can use trdtime_6?
	if (((otg_readl(GHW_CFG2) >> 6) & 0x3) == 1) {
		/* Recognized as High Speed PHY */
		/* UTMI+ WIDTH choose diff trdtiming */
		if (dwc_get_utmi_width() == 0) {
			debug("8BIT UTMI+.\n");
			otg_clearl(GUSB_CFG_PHY_16BIT, GUSB_CFG);
			otg_setl(USBCFG_TRDTIME_9, GUSB_CFG);
			clearl((CPM_USBPCR1_WORD_IF0 | CPM_USBPCR1_WORD_IF1),
			       CPM_USBPCR1);
		} else if (dwc_get_utmi_width() == 1) {
			debug("16BIT UTMI+.\n");
			otg_setl(GUSB_CFG_PHY_16BIT, GUSB_CFG);
			otg_setl(USBCFG_TRDTIME_6, GUSB_CFG);
			setl((CPM_USBPCR1_WORD_IF0 | CPM_USBPCR1_WORD_IF1),
			     CPM_USBPCR1);
		} else if (dwc_get_utmi_width() == 2) {
			debug("8BIT or 16BIT UTMI+.\n");
			otg_clearl(USBCFG_TRDTIME_MASK, GUSB_CFG);

			if (UTMI_PHY_WIDTH == 8) {
				/* 8bit */
				otg_clearl(GUSB_CFG_PHY_16BIT, GUSB_CFG);
				otg_setl(USBCFG_TRDTIME_9, GUSB_CFG);
				clearl((CPM_USBPCR1_WORD_IF0 |
					CPM_USBPCR1_WORD_IF1),
				       CPM_USBPCR1);
			} else {
				/* 16bit */
				otg_setl(GUSB_CFG_PHY_16BIT, GUSB_CFG);
				otg_setl(USBCFG_TRDTIME_6, GUSB_CFG);
				setl((CPM_USBPCR1_WORD_IF0 |
				      CPM_USBPCR1_WORD_IF1),
				     CPM_USBPCR1);
			}

		}
	} else
		debug("Unknown PHY TYPE?.\n");
}

static void dwc_otg_core_reset(void)
{
	u32 greset = 0;
	u32 cnt = 0;
	do {
		udelay(10);
		greset = otg_readl(GRST_CTL);
		if (cnt++ > 100000) {
			debug("GRESET wait IDLE timeout.\n");
			return;
		}
	} while ((greset & RSTCTL_AHB_IDLE) == 0);

	cnt = 0;

	otg_writel(greset | RSTCTL_CORE_RST, GRST_CTL);
	do {
		greset = otg_readl(GRST_CTL);
		if (cnt++ > 10000) {
			debug("GRESET wait reset timeout.\n");
			return;
		}
		udelay(10);
	} while (greset & RSTCTL_CORE_RST);

	/* wait for 3 phy clocks */
	udelay(100);
}

static void dwc_otg_core_init(void)
{
	otg_clearl((GUSB_CFG_ULPI |	  /* Force to UTMI+ rather than ULPI */
		    GUSB_CFG_FULL_SPEED | /* Force to High (USB 2.0) speed */
		    GUSB_CFG_SRP_ENABLE | /* Disable SRP */
		    GUSB_CFG_HNP_ENABLE), /* Disable HNP */
		   GUSB_CFG);

	dwc_otg_select_phy_width();

	dwc_otg_core_reset();

	otg_writel(1 << 7, GAHB_CFG);	// Slave mode and Mask all intr
	// Mask Intr
	otg_writel(0, GINT_MASK);
}

static void dwc_otg_flush_rx_fifo(void)
{
	otg_writel(RSTCTL_RXFIFO_FLUSH, GRST_CTL);

	while (otg_readl(GRST_CTL) & RSTCTL_RXFIFO_FLUSH) ;
}

void dwc_otg_flush_tx_fifo(void)
{
	otg_writel(RSTCTL_TXFNUM_ALL | RSTCTL_TXFIFO_FLUSH, GRST_CTL);

	while (otg_readl(GRST_CTL) & RSTCTL_TXFIFO_FLUSH) ;
}

static void disable_all_ep(void)
{
	int i;

	for (i = 0; i < DEP_NUM; i++) {
		otg_setl((DEP_DISENA_BIT | DEP_SET_NAK), DIEP_CTL(i));
		otg_setl((DEP_DISENA_BIT | DEP_SET_NAK), DOEP_CTL(i));

		otg_writel(0, DIEP_SIZE(i));
		otg_writel(0, DOEP_SIZE(i));

		otg_writel(0xFF, DIEP_INT(i));
		otg_writel(0xFF, DOEP_INT(i));
	}
}

static void dwc_otg_device_init(void)
{
	u16 epinfobase, gdfifocfg;

	otg_writel(DEP_RXFIFO_SIZE, GRXFIFO_SIZE);
	otg_writel((DEP_NPTXFIFO_SIZE << 16) | DEP_RXFIFO_SIZE, GNPTXFIFO_SIZE);
	otg_writel((DEP_DTXFIFO_SIZE << 16) |
		   (DEP_RXFIFO_SIZE + DEP_NPTXFIFO_SIZE), GDTXFIFO_SIZE);

	gdfifocfg = otg_readl(GHW_CFG3) >> 16;
	epinfobase =
	    (otg_readl(GRXFIFO_SIZE) & 0xffff) +
	    (otg_readl(GNPTXFIFO_SIZE) >> 16);
	otg_writel((epinfobase << 16) | gdfifocfg, GDFIFO_CFG);

	dwc_otg_flush_tx_fifo();
	dwc_otg_flush_rx_fifo();

	/* clear irq and mask all ep intr */
	otg_writel(0, DOEP_MASK);
	otg_writel(0, DIEP_MASK);
	otg_writel(0xFFFFFFFF, OTG_DAINT);
	otg_writel(0, DAINT_MASK);

	/* disable all in and out ep */
	disable_all_ep();

	otg_writel(0xFFFFFFFF, GINT_STS);

	otg_writel(0, OTG_DCFG);	// Slave mode and High speed to enum
	otg_writel(0, OTG_DCTL);	// Soft Disconnect-> 0(normal mode)

	otg_setl(GINT_MASK_RxFLvIMsk, GINT_MASK);
}

static void dwc_otg_enable_common_irq(void)
{
	otg_setl(1, GAHB_CFG);
	/*  CONIDSTS    OUTEP       INEP        enum        usbreset */
	otg_setl((GINT_MASK_ConIDStsChngMsk |
		  GINT_MASK_OEPIntMsk |
		  GINT_MASK_IEPIntMsk |
		  GINT_MASK_EnumDoneMsk |
		  GINT_MASK_USBRstMsk),
		 GINT_MASK);
}

void usbloader_init(void)
{
	dwc_otg_cpm_init();

	/* Enable OTG PHY */
	setl(OPCR_OTGPHY_ENABLE, CPM_OPCR);

	dwc_otg_core_init();

	dwc_otg_device_init();

	dwc_otg_enable_common_irq();
}
