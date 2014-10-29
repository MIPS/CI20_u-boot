#define	OTG_BASE	0xb3500000

/* Globle Regs define */
#define	GOTG_CTL		0x00
#define	GOTG_INTR		0x04
#define	GAHB_CFG		0x08
#define	GUSB_CFG		0x0c
#define GUSB_CFG_PHY_8BIT	(0 << 3)
#define GUSB_CFG_PHY_16BIT	(1 << 3)
#define GUSB_CFG_UTMI		(0 << 4)
#define GUSB_CFG_ULPI		(1 << 4)
#define GUSB_CFG_HIGH_SPEED	(0 << 6)
#define GUSB_CFG_FULL_SPEED	(1 << 6)
#define GUSB_CFG_SRP_DISABLE	(0 << 8)
#define GUSB_CFG_SRP_ENABLE	(1 << 8)
#define GUSB_CFG_HNP_DISABLE	(0 << 9)
#define GUSB_CFG_HNP_ENABLE	(1 << 9)
#define	GRST_CTL		0x10
#define	GINT_STS		0x14
#define	GINT_MASK		0x18
#define GINT_MASK_ConIDStsChngMsk	(1 << 28)
#define GINT_MASK_OEPIntMsk		(1 << 19)
#define GINT_MASK_IEPIntMsk		(1 << 18)
#define GINT_MASK_EnumDoneMsk		(1 << 13)
#define GINT_MASK_USBRstMsk		(1 << 12)
#define GINT_MASK_RxFLvIMsk		(1 << 4)
#define	GRXSTS_READ		0x1c
#define	GRXSTS_POP		0x20
#define	GRXFIFO_SIZE		0x24
#define	GNPTXFIFO_SIZE		0x28
#define	GDTXFIFO_SIZE		0x104
#define	GHW_CFG1		0x44
#define	GHW_CFG2		0x48
#define	GHW_CFG3		0x4c
#define	GHW_CFG4		0x50
#define	GDFIFO_CFG		0x5c

/* Device Regs define */
#define	EP_FIFO(n)		((n+1)*0x1000)	// FIX ME

#define	OTG_DCFG		0x800
#define	OTG_DCTL		0x804
#define	OTG_DSTS		0x808
#define USB_SPEED_HIGH	0
#define USB_SPEED_FULL	1
#define USB_SPEED_LOW		2
#define	DIEP_MASK		0x810
#define DIEP_MASK_XferComplMsk	(1 << 0)
#define DIEP_MASK_TimeOUTMsk		(1 << 3)
#define	DOEP_MASK		0x814
#define DOEP_MASK_XferComplMsk 	(1 << 0)
#define DOEP_MASK_SetUPMsk		(1 << 3)
#define	OTG_DAINT		0x818
#define	DAINT_MASK		0x81c
#define DAINT_MASK_INEP(n)	(1 << (n))
#define DAINT_MASK_OUTEP(n)	(1 << (16+(n)))

#define	DIEP_EMPMSK		0x834
#define DIEP_EMPMSK_Txf(n)	(1 << (n))

/* It's used in OTG_MULT_PROC_INTRPT = 1
#define	DEACH_INT		(OTG_BASE + 0x838)
#define	DEACH_INTMASK		(OTG_BASE + 0x83c)
#define	DIEP0_INTMASK		(OTG_BASE + 0x840)
#define	DIEP1_INTMASK		(OTG_BASE + 0x844)
#define	DOEP0_INTMASK		(OTG_BASE + 0x880)
#define	DOEP1_INTMASK		(OTG_BASE + 0x884)
*/

#define	DIEP_CTL(n)		(0x900 + (n)*0x20)
#define	DOEP_CTL(n)		(0xb00 + (n)*0x20)

#define	DIEP_INT(n)		(0x908 + (n)*0x20)
#define	DOEP_INT(n)		(0xb08 + (n)*0x20)

#define	DIEP_SIZE(n)		(0x910 + (n)*0x20)
#define DIEP_SIZE_PktCnt(n)	((n) << 19)
#define	DOEP_SIZE(n)		(0xb10 + (n)*0x20)

#define	DIEP_TXFSTS(n)		(0x918 + (n)*0x20)

/* Regs macro define */
/*************************************************/
#define	AHBCFG_DMA_ENA		BIT5
#define	AHBCFG_GLOBLE_INTRMASK	BIT0
#define	USBCFG_FORCE_DEVICE	BIT30
#define	USBCFG_TRDTIME_MASK	(0xF << 10)
#define	USBCFG_TRDTIME_9	(9 << 10)
#define	USBCFG_TRDTIME_6	(6 << 10)

/* GRSTCTL */
#define	RSTCTL_AHB_IDLE		BIT31
#define	RSTCTL_TXFNUM_ALL	(0x10 << 6)
#define	RSTCTL_TXFIFO_FLUSH	BIT5
#define	RSTCTL_RXFIFO_FLUSH	BIT4
#define	RSTCTL_INTK_FLUSH	BIT3
#define	RSTCTL_FRMCNT_RST	BIT2
#define	RSTCTL_CORE_RST		BIT0

/* GINTSTS */
#define	GINTSTS_RSUME_DETE	BIT31
#define	GINTSTS_CONID_STSCHG	BIT28
#define	GINTSTS_RESET_DETE	BIT23
#define	GINTSTS_FETCH_SUSPEND	BIT22
#define	GINTSTS_OEP_INTR	BIT19
#define	GINTSTS_IEP_INTR	BIT18
#define	GINTSTS_EP_MISMATCH	BIT17
#define	GINTSTS_ENUM_DONE	BIT13
#define	GINTSTS_USB_RESET	BIT12
#define	GINTSTS_USB_SUSPEND	BIT11
#define	GINTSTS_USB_EARLYSUSPEND	BIT10
#define	GINTSTS_I2C_INT		BIT9
#define	GINTSTS_ULPK_CKINT	BIT8
#define	GINTSTS_GOUTNAK_EFF	BIT7
#define	GINTSTS_GINNAK_EFF	BIT6
#define	GINTSTS_NPTXFIFO_EMPTY	BIT5
#define	GINTSTS_RXFIFO_NEMPTY	BIT4
#define	GINTSTS_START_FRAM	BIT3
#define	GINTSTS_OTG_INTR	BIT2
#define	GINTSTS_MODE_MISMATCH	BIT1

/* DCTL */
#define	DCTL_CLR_GNPINNAK	BIT8
#define	DCTL_SOFT_DISCONN	BIT1

/* DCFG */
#define	DCFG_DEV_ADDR_MASK	(0x7F << 4)
#define	DCFG_DEV_ADDR_BIT	4

/* DSTS */
#define	DSTS_ENUM_SPEED_MASK		(0x3 << 1)
#define	DSTS_ENUM_SPEED_BIT		BIT1
#define	DSTS_ENUM_SPEED_HIGH		(0x0 << 1)
#define	DSTS_ENUM_SPEED_FULL_30OR60	(0x1 << 1)
#define	DSTS_ENUM_SPEED_LOW		(0x2 << 1)
#define	DSTS_ENUM_SPEED_FULL_48		(0x3 << 1)

/* GRXSTSR/GRXSTSP */
#define	GRXSTSP_PKSTS_MASK		(0xF << 17)
#define	GRXSTSP_PKSTS_GOUT_NAK		(0x1 << 17)
#define	GRXSTSP_PKSTS_GOUT_RECV		(0x2 << 17)
#define	GRXSTSP_PKSTS_TX_COMP		(0x3 << 17)
#define	GRXSTSP_PKSTS_SETUP_COMP	(0x4 << 17)
#define	GRXSTSP_PKSTS_SETUP_RECV	(0x6 << 17)
#define	GRXSTSP_BYTE_CNT_MASK		(0x7FF << 4)
#define	GRXSTSP_BYTE_CNT_BIT		4
#define	GRXSTSP_EPNUM_MASK		(0xF)
#define	GRXSTSP_EPNUM_BIT		BIT0

/* DIOEPCTL */
// ep0
#define	DEP_EP0_MAXPKET_SIZE	64
#define	DEP_EP0_MPS_64		(0x0)
#define	DEP_EP0_MPS_32		(0x1)
#define	DEP_EP0_MPS_16		(0x2)
#define	DEP_EP0_MPS_8		(0x3)

#define	DEP_ENA_BIT		BIT31
#define	DEP_DISENA_BIT		BIT30
#define	DEP_SET_NAK		BIT27
#define	DEP_CLEAR_NAK		BIT26
#define	DEP_TYPE_MASK		(0x3 << 18)
#define	DEP_TYPE_CNTL		(0x0 << 18)
#define	DEP_TYPE_ISO		(0x1 << 18
#define	DEP_TYPE_BULK		(0x2 << 18)
#define	DEP_TYPE_INTR		(0x3 << 18)
#define	USB_ACTIVE_EP		BIT15
#define	DEP_PKTSIZE_MASK	0x7FF
#define	DEP_FS_PKTSIZE		64
#define	DEP_HS_PKTSIZE		512

/* DIOEPINT */
#define	DEP_NYET_INT		BIT14
#define	DEP_NAK_INT		BIT13
#define	DEP_BABBLE_ERR_INT	BIT12
#define	DEP_PKT_DROP_STATUS	BIT11
#define	DEP_BNA_INT		BIT9
#define	DEP_TXFIFO_UNDRN	BIT8	// Only for INEP
#define	DEP_OUTPKT_ERR		BIT8	// Only for OUTEP
#define	DEP_TXFIFO_EMPTY	BIT7
#define	DEP_INEP_NAKEFF		BIT6	// Only for INEP
#define	DEP_B2B_SETUP_RECV	BIT6	// Only for OUTEP0
#define	DEP_INTOKEN_EPMISATCH	BIT5	// Only for INEP
#define	DEP_STATUS_PHASE_RECV	BIT5	// Only for OUTEP0
#define	DEP_INTOKEN_RECV_TXFIFO_EMPTY	BIT4	// Only for INEP
#define	DEP_OUTTOKEN_RECV_EPDIS	BIT4	// Only for OUTEP
#define	DEP_TIME_OUT		BIT3	// Only for INEP
#define	DEP_SETUP_PHASE_DONE	BIT3	// Only for OUTEP0
#define	DEP_AHB_ERR		BIT2
#define	DEP_EPDIS_INT		BIT1
#define	DEP_XFER_COMP		BIT0	// Used by INEP and OUTEP

/* DOEPSIZ0 */
#define	DOEPSIZE0_SUPCNT_1	(0x1 << 29)
#define	DOEPSIZE0_SUPCNT_2	(0x2 << 29)
#define	DOEPSIZE0_SUPCNT_3	(0x3 << 29)
#define	DOEPSIZE0_PKTCNT_BIT	BIT19

#define	DEP_RXFIFO_SIZE		1064
#define	DEP_NPTXFIFO_SIZE	1024
#define	DEP_DTXFIFO_SIZE	768

#define	DEP_NUM			2

#if 0
#define	UTMI_PHY_WIDTH		8
#else
#define	UTMI_PHY_WIDTH		16

/* OTG parameter control register(USBPCR) */
#define	USBPCR_USB_MODE         BIT31
#define	USBPCR_AVLD_REG         BIT30
#define	USBPCR_INCRM            BIT27	/* INCR_MASK bit */
#define	USBPCR_CLK12_EN         BIT26
#define	USBPCR_COMMONONN        BIT25
#define	USBPCR_VBUSVLDEXT       BIT24
#define	USBPCR_VBUSVLDEXTSEL    BIT23
#define	USBPCR_POR              BIT22
#define	USBPCR_SIDDQ            BIT21
#define	USBPCR_OTG_DISABLE      BIT20
#define	USBPCR_TXPREEMPHTUNE    BIT6

/* Oscillator and power control register(OPCR) */
#define	OPCR_OTGPHY_ENABLE      BIT7	/* SPENDN bit */
#define	OPCR_GPSEN              BIT6
#define	OPCR_UHCPHY_DISABLE     BIT5	/* SPENDH bit */
#define	OPCR_O1SE               BIT4
#define	OPCR_PD                 BIT3
#define	OPCR_ERCS               BIT2

#endif

/*---------------------------------------------------------*/
typedef struct usb_status {
	u8 *addr;		// save of operate data address
	u32 setup_packet[2];
	u32 length;		// save of operate data length
	u32 xfer_count;		// length of transferred
	u32 data_tran_flag;
	int flag;
	void (*cb)(void);
} USB_STATUS;

/*---------------------------------------------------------*/
/* Standard requests, for the bRequest field of a SETUP packet. */
#define	USB_REQ_GET_STATUS              0x00
#define	USB_REQ_CLEAR_FEATURE           0x01
#define	USB_REQ_SET_FEATURE             0x03
#define	USB_REQ_SET_ADDRESS             0x05
#define	USB_REQ_GET_DESCRIPTOR          0x06
#define	USB_REQ_SET_DESCRIPTOR          0x07
#define	USB_REQ_GET_CONFIGURATION       0x08
#define	USB_REQ_SET_CONFIGURATION       0x09
#define	USB_REQ_GET_INTERFACE           0x0A
#define	USB_REQ_SET_INTERFACE           0x0B
#define	USB_REQ_SYNCH_FRAME             0x0C

/* Vendor requests. */
#define	EP0_GET_CPU_INFO                0x00
#define	EP0_SET_DATA_ADDRESS            0x01
#define	EP0_SET_DATA_LENGTH             0x02
#define	EP0_FLUSH_CACHES                0x03
#define	EP0_PROG_START1                 0x04
#define	EP0_PROG_START2                 0x05

/* Descriptor types ... USB 2.0 spec table 9.5 */
#define	USB_DT_DEVICE                   0x01
#define	USB_DT_CONFIG                   0x02
#define	USB_DT_STRING                   0x03
#define	USB_DT_INTERFACE                0x04
#define	USB_DT_ENDPOINT                 0x05
#define	USB_DT_DEVICE_QUALIFIER         0x06
#define	USB_DT_OTHER_SPEED_CONFIG       0x07
#define	USB_DT_INTERFACE_POWER          0x08
/* these are from a minor usb 2.0 revision (ECN) */
#define	USB_DT_OTG                      0x09
#define	USB_DT_DEBUG                    0x0A
#define	USB_DT_INTERFACE_ASSOCIATION    0x0B

#define	USB_DT_DEVICE_SIZE              18
#define	USB_DT_CONFIG_SIZE              9

#define	MAX_PKT_CNT     1023

#define	DEP_EP_MAXPKT(n)	((n) ? DEP_HS_PKTSIZE : DEP_FS_PKTSIZE)

static u8 dev_qualifier[] = { 0x0a, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40,
	0x01, 0x00
};
