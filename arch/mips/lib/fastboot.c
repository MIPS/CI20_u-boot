/*
 * Platform Specific Portion of Fastboot command for CI20 board.
 *
 * Copyright (c) 2014 Imagination Technologies
 * Author: Chris Larsen <chris.larsen@imgtec.com>
 *
 * Reflash the on-board NAND with new Android images.
 *
 * Large portions of this code were taken from X-Boot but those files
 * don't have attribution information.  Consequently, attribution is
 * missing in this file.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
#include <version.h>
#include <fastboot.h>
#include <asm/arch/jz4780.h>

#define TYPE_DEVICE          1
#define TYPE_CONFIGURATION   2
#define TYPE_STRING          3
#define TYPE_INTERFACE       4
#define TYPE_ENDPOINT        5

#include "usb_descriptors.h"
#include <asm/arch/jz4780_otg.h>

#define PRINT_BUF_MAX 1024
static char print_buf[PRINT_BUF_MAX];

#define	BULK_OUT_BUF_SIZE	0x4000	//0x21000       //buffer size :

unsigned char Bulk_Out_Buf[BULK_OUT_BUF_SIZE];
unsigned char *Bulk_Data_Buf;
u32 Bulk_In_Size = 0, Bulk_In_Finish = 0, Bulk_Out_Size = 0;
static unsigned long Bulk_Data_Size = 0;
static unsigned long rx_length;
static int cmd_downloading = 0;
static unsigned int d_count = 0;
void tx_status(USB_STATUS * status, const char *fmt, ...);
void HW_SendPKT(int epnum, const u8 * buf, int size, USB_STATUS * status);

#define	MAX_PACKET_SIZE	64

void flash_dump_ptn(void)
{
	if (do_mtdparts_default()) {	// mtdparts default
		return;
	}

	if (ubi_part("boot", NULL)) {	// ubi part boot
		debug("%s, line %d:  UBI partition \"boot\" does not exist\n",
			__FILE__, __LINE__);
		return;
	}
	printf("\n");

	dump_ubi_volume_table("boot");

	if (ubi_part("system", NULL)) {	// ubi part system
		debug("%s, line %d:  UBI partition \"system\" does not exist\n",
			__FILE__, __LINE__);
		return;
	}
	printf("\n");

	dump_ubi_volume_table("system");
}

static void handle_early_suspend_intr(void)
{
	debug("Handle early suspend intr.\n");

	otg_setl(GINTSTS_USB_EARLYSUSPEND, GINT_STS);
}

static void handle_start_frame_intr(void)
{
	//debug("Handle start frame intr.\n");

	otg_setl(GINTSTS_START_FRAM, GINT_STS);
}

extern void dwc_otg_flush_tx_fifo(void);

static void handle_reset_intr(void)
{
	//u16 i = 0;

	debug("Handle_reset_intr called \n");
	otg_setl(DAINT_MASK_INEP(0) | DAINT_MASK_OUTEP(0), DAINT_MASK);
	// xfercompl setupdone
	otg_setl(DOEP_MASK_XferComplMsk | DOEP_MASK_SetUPMsk, DOEP_MASK);
	// xfercompl intoken timeout
	otg_setl(DIEP_MASK_XferComplMsk | DIEP_MASK_TimeOUTMsk, DIEP_MASK);

	/* NAK OUT ep */
	otg_setl(DEP_SET_NAK, DOEP_CTL(0));
	otg_setl(DEP_SET_NAK, DOEP_CTL(1));

	/* flush all txfifos */
	dwc_otg_flush_tx_fifo();

	/* Reset Device Address */
	otg_clearl(DCFG_DEV_ADDR_MASK, OTG_DCFG);

	/* setup EP0 to receive SETUP packets */
	otg_writel(DOEPSIZE0_SUPCNT_3 | DOEPSIZE0_PKTCNT_BIT | (8 * 3),
		   DOEP_SIZE(0));

	otg_setl(DEP_ENA_BIT | DEP_CLEAR_NAK, DOEP_CTL(0));

	otg_setl(GINTSTS_USB_RESET, GINT_STS);
}				/* handle_reset_intr */

static void handle_enum_done_intr(int *flag)
{
	int speed = 0;

	*flag = 1;

	u32 dsts = otg_readl(OTG_DSTS);
	u32 diep0ctl = otg_readl(DIEP_CTL(0));
	diep0ctl &= ~(0x3);

	otg_clearl(DEP_PKTSIZE_MASK, DIEP_CTL(1));
	otg_clearl(DEP_PKTSIZE_MASK, DOEP_CTL(1));
	otg_clearl(DEP_TYPE_MASK, DIEP_CTL(1));
	otg_clearl(DEP_TYPE_MASK, DOEP_CTL(1));

	debug("Handle enum done intr.\n");

	switch (dsts & DSTS_ENUM_SPEED_MASK) {
	case DSTS_ENUM_SPEED_HIGH:
		debug("High speed.\n");
		speed = USB_SPEED_HIGH;
		otg_setl(DEP_HS_PKTSIZE, DIEP_CTL(1));
		otg_setl(DEP_HS_PKTSIZE, DOEP_CTL(1));
		break;
	case DSTS_ENUM_SPEED_FULL_30OR60:
	case DSTS_ENUM_SPEED_FULL_48:
		debug("Full speed.\n");
		speed = USB_SPEED_FULL;
		otg_setl(DEP_FS_PKTSIZE, DIEP_CTL(1));
		otg_setl(DEP_FS_PKTSIZE, DOEP_CTL(1));
		break;
	case DSTS_ENUM_SPEED_LOW:
		speed = USB_SPEED_LOW;
		diep0ctl |= DEP_EP0_MPS_8;
		break;
	default:
		debug("Fault speed enumeration\n");
		break;
	}

	otg_writel(diep0ctl, DIEP_CTL(0));
	otg_setl(DEP_ENA_BIT, DOEP_CTL(0));
	otg_setl(USB_ACTIVE_EP | DEP_TYPE_BULK, DIEP_CTL(1));
	otg_setl(USB_ACTIVE_EP | DEP_TYPE_BULK, DOEP_CTL(1));

	otg_setl(DCTL_CLR_GNPINNAK, OTG_DCTL);

	otg_setl(GINTSTS_ENUM_DONE, GINT_STS);
}

void dwc_read_out_packet(USB_STATUS * status, u32 count)
{
	int i;
	int dwords = (count + 3) / 4;
	int current = 0;

	//debug("--------------read out data: %08X\n", dwords);
	if (0 == cmd_downloading) {
		debug("Bulk_Out_Size:  %08X\n", Bulk_Out_Size);
		unsigned int *buf =
		    (unsigned int *)(Bulk_Out_Buf + Bulk_Out_Size);
		for (i = 0; i < dwords; i++) {
			buf[i] = otg_readl(EP_FIFO(1));
		}
		Bulk_Out_Size += count;

	} else {
		if (Bulk_Data_Size == 0 || Bulk_Data_Size > 0x00330000) {
			debug("Bulk_Data_Size:  %08lX\n", Bulk_Data_Size);
		}
		if (Bulk_Data_Size <= rx_length) {
			unsigned int *buf =
			    (unsigned int *)(Bulk_Data_Buf + Bulk_Data_Size);
			for (i = 0; i < dwords; i++) {
				buf[i] = otg_readl(EP_FIFO(1));
			}
			Bulk_Data_Size += count;
			d_count++;

			if ((d_count / 50000) && ((d_count % 50000) == 0)) {
				/* printf("rx_length\n"); */
				/* debug("0x%08X\n", rx_length); */
				/* printf("Bulk_Data_Size\n"); */
				/* debug("0x%08X\n", Bulk_Data_Size); */
				current = Bulk_Data_Size / (rx_length / 100);
				debug("0x%08X\n", current);
			}
			if ((Bulk_Data_Size % 0x100000) == 0 ||
			    Bulk_Data_Size == rx_length) {
				printf("0x%x\r", Bulk_Data_Size);
			}
		}
		if ((count < DEP_EP_MAXPKT(1)) || (Bulk_Data_Size == rx_length)) {
			tx_status(status, FASTBOOT_REPLY_OKAY);
			printf("last data packet\n");
			cmd_downloading = 0;
		}
	}
}

void handle_rxfifo_nempty(USB_STATUS * status)
{
	u32 count;
	u32 rxsts_pop = otg_readl(GRXSTS_POP);

	//debug("Handle rxfifo nempty.\n");
	switch (rxsts_pop & GRXSTSP_PKSTS_MASK) {
	case GRXSTSP_PKSTS_GOUT_NAK:
		debug("GRXSTSP_PKSTS_GOUT_NAK.\n");
		break;
	case GRXSTSP_PKSTS_GOUT_RECV:
		//debug("GRXSTSP_PKSTS_GOUT_RECV.\n");
		/* FIXME */
		count =
		    (rxsts_pop & GRXSTSP_BYTE_CNT_MASK) >> GRXSTSP_BYTE_CNT_BIT;
		if (count) {
			dwc_read_out_packet(status, count);
		}
		break;
	case GRXSTSP_PKSTS_TX_COMP:
		debug("GRXSTSP_PKSTS_TX_COMP.\n");
		break;
	case GRXSTSP_PKSTS_SETUP_COMP:
		debug("GRXSTSP_PKSTS_SETUP_COMP.\n");
		break;
	case GRXSTSP_PKSTS_SETUP_RECV:
		debug("GRXSTSP_PKSTS_SETUP_RECV.\n");
		status->setup_packet[0] = otg_readl(EP_FIFO(0));
		status->setup_packet[1] = otg_readl(EP_FIFO(0));
		debug("word1:%08X\n", status->setup_packet[0]);
		debug("word2:%08X\n", status->setup_packet[1]);
		break;
	default:
		break;
	}

	otg_setl(GINTSTS_RXFIFO_NEMPTY, GINT_STS);
}

static void dwc_otg_write_packet(USB_STATUS * status, int epnum)
{
	int i;
	u32 dwords;
	u32 byte_count;
	byte_count = status->length - status->xfer_count;

	if (byte_count > DEP_EP_MAXPKT(epnum))
		byte_count = DEP_EP_MAXPKT(epnum);

	dwords = (byte_count + 3) / 4;

	debug("dwc_otg_write_packet, dwords:%08X\n", dwords);
	unsigned int *buf = (unsigned int *)status->addr;
	for (i = 0; i < dwords; i++) {
		otg_writel(buf[i], EP_FIFO(epnum));
	}

	status->xfer_count += byte_count;
	status->addr += byte_count;
}

void write_ep_packet(USB_STATUS * status, int epnum)
{
	u32 xfersize;
	u32 dwords;
	status->xfer_count = 0;
	u32 txstatus = otg_readl(DIEP_TXFSTS(epnum));
	if (status->length > DEP_EP_MAXPKT(epnum))
		xfersize = DEP_EP_MAXPKT(epnum);
	else
		xfersize = status->length;

	dwords = (xfersize + 3) / 4;
	debug("txstatus:%x dwords:%x status->xfer_count:%x\n", txstatus,
	      dwords, status->xfer_count);
	while ((txstatus > dwords) && (status->length > 0)
	       && (status->xfer_count < status->length)) {
		dwc_otg_write_packet(status, epnum);
		xfersize = status->length - status->xfer_count;
		if (xfersize > DEP_EP_MAXPKT(epnum))
			xfersize = DEP_EP_MAXPKT(epnum);

		dwords = (xfersize + 3) / 4;
		txstatus = otg_readl(DIEP_TXFSTS(epnum));
	}
	Bulk_In_Finish = status->xfer_count;
	if (Bulk_In_Size > Bulk_In_Finish) {
		printf("Bulk_in_size > Bulk_in_finish\n");
		u32 intr = otg_readl(DIEP_INT(epnum));

		while (!(intr & DEP_TXFIFO_EMPTY)) {
			intr = otg_readl(DIEP_INT(epnum));
		}
		printf("Bulk_in_size > Bulk_in_finish\n");
		HW_SendPKT(epnum, status->addr, Bulk_In_Size - Bulk_In_Finish,
			   status);
	}
}

void handle_inep_intr(USB_STATUS * status)
{
	u32 ep_intr, intr;
	int epnum = 31;

	ep_intr = (otg_readl(OTG_DAINT) & 0xffff);
	while (ep_intr) {
		if (!(ep_intr & (0x1 << epnum))) {
			epnum--;
			continue;
		}

		intr = otg_readl(DIEP_INT(epnum));

		if ((intr & DEP_TXFIFO_EMPTY)
		    && (otg_readl(DIEP_EMPMSK) & DIEP_EMPMSK_Txf(epnum))) {
			if (!status->flag && status->length) {
				debug("inep xfer start. handle = %08X\n",
				      status->length);
				write_ep_packet(status, epnum);
			}
			otg_setl(DEP_TXFIFO_EMPTY, DIEP_INT(epnum));

			if (!status->flag && status->length) {
				while (!(otg_readl(DIEP_INT(epnum)) &
					 DEP_XFER_COMP)) ;

				debug("inep xfer comp. handle = %08X\n", epnum);
				otg_setl(DEP_XFER_COMP, DIEP_INT(epnum));
				otg_clearl(DIEP_EMPMSK_Txf(epnum), DIEP_EMPMSK);
				otg_setl(DEP_ENA_BIT | DEP_CLEAR_NAK,
					 DOEP_CTL(epnum));
				if (intr & DEP_NAK_INT)
					otg_setl(DEP_NAK_INT, DIEP_INT(epnum));
			}

			if (status->length == status->xfer_count) {
				status->flag = 1;
				if (status->cb) {
					(*status->cb)();
					status->cb = NULL;
				}
			}
		}

		ep_intr &= ~(0x1 << epnum);
		epnum--;
	}
}

void handle_ep_status_in_phase(USB_STATUS * status, int epnum)
{
	// pktcnt->1 xfersize->0
	otg_writel(DOEPSIZE0_PKTCNT_BIT, DIEP_SIZE(epnum));
	otg_setl(DEP_ENA_BIT | DEP_CLEAR_NAK, DIEP_CTL(epnum));
}

void handle_ep_data_in_phase(USB_STATUS * status, int epnum)
{
	u32 pktcnt, xfersize;

	xfersize = status->length;

	pktcnt = (xfersize + DEP_EP_MAXPKT(epnum) - 1) / DEP_EP_MAXPKT(epnum);

	if (pktcnt > 1023) {
		printf("WARNING...\n");
		while (1) ;
	}
	otg_writel(DIEP_SIZE_PktCnt(pktcnt) | xfersize, DIEP_SIZE(epnum));
	otg_setl(DEP_ENA_BIT | DEP_CLEAR_NAK, DIEP_CTL(epnum));

	otg_setl(DIEP_EMPMSK_Txf(epnum), DIEP_EMPMSK);
}

int handle_setup_packet(USB_STATUS * status, int epnum)
{
	u32 i;
	u32 addr;

	u32 word1 = status->setup_packet[0];
	u32 word2 = status->setup_packet[1];
	status->data_tran_flag = 0;
	status->length = 0;
	status->flag = 0;

	/* Process regular USB requests only; ignore vendor specific
	 * requests.
	 */
	if (!(word1 & 0x60)) {
		/* standard_request handle */
		debug("Standard_request : ");

		status->data_tran_flag = 1;
		status->length = word2 >> 16;
		status->flag = 0;

		switch ((word1 >> 8) & 0xff) {
		case USB_REQ_SET_ADDRESS:
			addr = word1 >> 16;
			otg_clearl(DCFG_DEV_ADDR_MASK, OTG_DCFG);
			otg_setl(addr << DCFG_DEV_ADDR_BIT, OTG_DCFG);
			debug("SET_ADDRESS. Address = %08X\n", addr);
			break;

		case USB_REQ_SET_CONFIGURATION:
			debug("SET_CONFIGURATION. Configuration = %d\n",
			      word1 >> 16);
			otg_setl(DEP_ENA_BIT | DEP_CLEAR_NAK, DOEP_CTL(1));
			break;

		case USB_REQ_GET_DESCRIPTOR:
			debug("GET_DESCRIPTOR - ");

			switch (word1 >> 24) {
			case USB_DT_DEVICE:
				debug("DEVICE. \n");
				status->addr = device_desc;
				if (status->length > USB_DT_DEVICE_SIZE)
					status->length = USB_DT_DEVICE_SIZE;	// max length of device_desc
				break;

			case USB_DT_CONFIG:
				debug("CONFIG. \n");
				status->addr = config_desc;
				if (status->length > 32)
					status->length = 32;	// max length of device_desc
				break;

			case USB_DT_STRING:
				debug("STRING. \n");
				i = (word1 >> 16) & 0xff;

				if (i == 1) {
					status->addr =
					    (u8 *) manufacturer_string;
					status->length =
					    sizeof(manufacturer_string);
				} else if (i == 2) {
					status->addr = (u8 *) product_string;
					status->length = sizeof(product_string);
				} else {
					status->addr = (u8 *) language_table;
					status->length = sizeof(language_table);
				}
				break;

			case USB_DT_DEVICE_QUALIFIER:
				debug("DEVICE_QUALIFIER. \n");
				status->addr = dev_qualifier;
				if (status->length > 10)
					status->length = 10;	// max length of device_desc
				break;

			default:
				debug("Not contain.\n");
				break;
			}
			break;

		default:
			debug("Not contain.\n");
			break;
		}
	}

	if (status->data_tran_flag)
		handle_ep_data_in_phase(status, 0);
	else
		handle_ep_status_in_phase(status, 0);

	return 0;
}

int handle_outep_intr(USB_STATUS * status)
{
	u32 ep_intr, intr;
	int epnum = 31;
	int ret = 0;

	debug("Handle outep intr.\n");

	ep_intr = (otg_readl(OTG_DAINT) & 0xFFFF0000) >> 16;
	while (ep_intr) {
		if (!(ep_intr & (0x1 << epnum))) {
			epnum--;
			continue;
		}

		debug("Handle outep: %08X\n", epnum);

		intr = otg_readl(DOEP_INT(epnum));
		if (intr & DEP_XFER_COMP) {
			debug("outep xfer comp.\n");
			otg_setl(DEP_XFER_COMP, DOEP_INT(epnum));
			otg_setl(DEP_ENA_BIT | DEP_CLEAR_NAK, DOEP_CTL(epnum));
		}
		if (intr & DEP_SETUP_PHASE_DONE) {
			debug("DEP_SETUP_PHASE_DONE.\n");
			otg_setl(DEP_SETUP_PHASE_DONE, DOEP_INT(epnum));
			ret = handle_setup_packet(status, epnum);
			if (ret)
				return ret;
		}
		ep_intr &= ~(0x1 << epnum);
		epnum--;
	}

	return 0;
}

void HW_SendPKT(int epnum, const u8 * buf, int size, USB_STATUS * status)
{
	Bulk_In_Size = size;
	Bulk_In_Finish = 0;
	status->addr = (u8 *) buf;
	status->length = size;
	status->flag = 0;

	if (Bulk_In_Size > MAX_PKT_CNT * DEP_EP_MAXPKT(epnum))
		status->length = MAX_PKT_CNT * DEP_EP_MAXPKT(epnum);
	if (status->length)
		status->data_tran_flag = 1;
	status->xfer_count = 0;

	if (status->data_tran_flag) {
		handle_ep_data_in_phase(status, epnum);
	} else {
		handle_ep_status_in_phase(status, epnum);
	}

}

void tx_status(USB_STATUS * status, const char *fmt, ...)
{
	static char buffer[MAX_PACKET_SIZE + 1];	// Fastboot packet size
							// limited to 64 bytes;
							// need one extra byte
							// for null character.
	va_list argp;

	debug("%s, line %d: fmt = \"%s\"\n", __FILE__, __LINE__, fmt);
	va_start(argp, fmt);
	debug("%s, line %d: back from va_start()\n", __FILE__, __LINE__);
	vsnprintf(buffer, sizeof(buffer), fmt, argp);
	debug("%s, line %d: back from vsnprintf(): \"%s\"\n", __FILE__,
	      __LINE__, buffer);
	va_end(argp);
	debug("%s, line %d: back from va_end()\n", __FILE__, __LINE__);

	HW_SendPKT(1, (const u8 *)buffer, strlen(buffer), status);
	debug("%s, line %d: back from HW_SendPKT()\n", __FILE__, __LINE__);
}

#include <linux/ctype.h>

// Just enough of a sscanf function to read Fastboot commands.  If
// you need a complete implementation you're out of luck 'cause this
// ain't it.
//
// TODO: Get "free" version of a complete sscanf() implementation and
// put it in the same part of the source code tree as the existing
// sprintf() function.
int sscanf(const char *src, const char *fmt, ...)
{
	int count = 0;
	int i, j;
	int *pi;
	char *pc;
	va_list argp;

	va_start(argp, fmt);

	for (i = j = 0; fmt[i] && src[j]; ++i) {
		switch (fmt[i]) {
		case '%':
			// Read next data field
			++i;
			if (fmt[i] == 's') {
				pc = va_arg(argp, char *);
				strcpy(pc, src + j);
				j = strlen(src);
				++count;
			} else if (tolower(fmt[i]) == 'x') {
				pi = va_arg(argp, int *);
				if (!isxdigit(src[j]))
					goto error;
				*pi = 0;
				while (isxdigit(src[j])) {
					*pi *= 16;
					*pi += isdigit(src[j]) ?
					    src[j] - '0' :
					    tolower(src[j]) - 'a' + 10;
					++j;
				}
				++count;
			} else if (fmt[i] == '%') {
				if (fmt[i] == src[j]) {
					++j;
				} else {
					goto error;
				}
			} else {
				goto error;
			}
			break;
		default:
			if (fmt[i] == src[j]) {
				++j;
			} else {
				goto error;
			}
			break;
		}
	}

error:
	va_end(argp);
	return count;
}

void do_cmd_getvar(USB_STATUS * status, char *strparam)
{
	char tmp[32] = "0x";
	char buf[MAX_PACKET_SIZE + 1];
	strcpy(buf, FASTBOOT_REPLY_OKAY);
	char ptnparam[MAX_PACKET_SIZE + 1];

	printf("\nprocess--->getvar\n");

	if (!strcmp("version", strparam)) {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
			 FASTBOOT_VERSION);
	} else if (!strcmp("version-bootloader", strparam)) {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
			 U_BOOT_VERSION);
	} else if (!strcmp("version-baseband", strparam)) {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY);
	} else if (!strcmp("product", strparam)) {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
			 CONFIG_SYS_BOARD);
	} else if (!strcmp("serialno", strparam)) {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
			 getenv("serial#"));
	} else if (!strcmp("secure", strparam)) {
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s", "no");
	} else if (sscanf(strparam, "partition-type:%s", ptnparam) == 1) {
		if (!strcmp(FB_PARTITION_BOOT, ptnparam)) {
			snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
				 "raw");
		} else if (!strcmp(FB_PARTITION_RECOVERY, ptnparam)) {
			snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
				 "raw");
		} else if (!strcmp(FB_PARTITION_SYSTEM, ptnparam)) {
			snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
				 "ubifs");
		} else {
			snprintf(buf, sizeof(buf), FASTBOOT_REPLY_OKAY "%s",
				 "UNKNOWN");
		}
	} else {
		printf("Unknown Fastboot Command: \"%s\"\n",
		       (char *)Bulk_Out_Buf);
		snprintf(buf, sizeof(buf), FASTBOOT_REPLY_FAIL "%s",
			 "Unknown Fastboot Command");
	}

	mdelay(100);
	tx_status(status, buf);
}

void do_cmd_download(USB_STATUS * status, int intparam)
{
	printf("\nprocess--->download\n");
	rx_length = intparam;
	cmd_downloading = 1;
	debug("%s, line %d:  rx_length = 0x%x\n", __FILE__, __LINE__,
	      rx_length);

	Bulk_Data_Buf = (unsigned char *)0x30000000;
	Bulk_Data_Size = 0;

	tx_status(status, FASTBOOT_REPLY_DATA "%08x", rx_length);
}

void do_cmd_flash(USB_STATUS * status, char *ptnparam)
{
	char par_name[16];

	printf("\nprocess--->flash\n");

	cmd_downloading = 0;

	if (!strcmp(FB_PARTITION_BOOT, ptnparam) ||
	    !strcmp(FB_PARTITION_CACHE, ptnparam) ||
	    !strcmp(FB_PARTITION_RECOVERY, ptnparam) ||
	    !strcmp(FB_PARTITION_SYSTEM, ptnparam) ||
	    !strcmp(FB_PARTITION_USERDATA, ptnparam)) {
		strcpy(par_name, ptnparam);
	} else {
		strcpy(par_name, "");
	}

	// Things get a bit tricky here.  What Fastboot calls a
	// partition, UBI calls a volume.  Before we can write the UBI
	// volume we must first select the appropriate UBI partition.
	// All of the Android partitions exist as volumes in the UBI
	// volume "system" (not to be confused with the Android
	// partition "system").
	if (do_mtdparts_default()) {	// mtdparts default
		tx_status(status, FASTBOOT_REPLY_FAIL
			  "do_mtdparts_default() failed");
		return;
	}

	if (!strcmp(par_name, FB_PARTITION_BOOT) ||
	    !strcmp(par_name, FB_PARTITION_RECOVERY)) {

		if (ubi_part("boot", NULL)) {	// ubi part boot
			debug("%s, line %d:  UBI partition \"boot\" does not "
			      "exist\n", __FILE__, __LINE__);
			tx_status(status, FASTBOOT_REPLY_FAIL
				  "UBI partition \"boot\" does not exist");
			return;
		}

		if (memcmp
		    ((void *)Bulk_Data_Buf, FASTBOOT_BOOT_MAGIC,
		     FASTBOOT_BOOT_MAGIC_SIZE)) {
			tx_status(status, FASTBOOT_REPLY_FAIL
				  "image is not a boot image");
			return;
		}
		debug("%s, line %d: valid boot image being written to \"%s\" "
		      "partition\n", __FILE__, __LINE__, par_name);
	} else {
		if (ubi_part("system", NULL)) {	// ubi part system
			debug("%s, line %d:  UBI partition \"system\" does not "
			      "exist\n", __FILE__, __LINE__);
			tx_status(status, FASTBOOT_REPLY_FAIL
				  "UBI partition \"system\" does not exist");
			return;
		}
	}

	// Now we need to write the UBI volume within the UBI partition
	// "system" which corresponds to the Android partition.
	printf("writing %08lx bytes to %s\n", rx_length, par_name);
	int ret = ubi_volume_write(par_name, Bulk_Data_Buf, rx_length);

	if (ret) {
		debug("%s, line %d:  FAILflash write \"%s\" failure\n",
		      __FILE__, __LINE__, par_name);
		tx_status(status, FASTBOOT_REPLY_FAIL "flash write failure");
	} else {
		debug("%s, line %d:  OKAY writing partition \"%s\"\n",
		      __FILE__, __LINE__, par_name);
		tx_status(status, FASTBOOT_REPLY_OKAY);
	}
}

void do_cmd_erase(USB_STATUS * status, char *ptnparam)
{
	char par_name[16];

	printf("\nprocess--->erase\n");
	if (!strcmp(FB_PARTITION_BOOT, ptnparam) ||
	    !strcmp(FB_PARTITION_CACHE, ptnparam) ||
	    !strcmp(FB_PARTITION_MISC, ptnparam) ||
	    !strcmp(FB_PARTITION_RECOVERY, ptnparam) ||
	    !strcmp(FB_PARTITION_STORAGE, ptnparam) ||
	    !strcmp(FB_PARTITION_SYSTEM, ptnparam) ||
	    !strcmp(FB_PARTITION_USERDATA, ptnparam)) {
		strcpy(par_name, ptnparam);
	} else {
		strcpy(par_name, "");
	}

	// Things get a bit tricky here.  What Fastboot calls a
	// partition, UBI calls a volume.  Before we can write the UBI
	// volume we must first select the appropriate UBI partition.
	// All of the Android partitions exist as volumes in the UBI
	// volume "system" (not to be confused with the Android
	// partition "system").
	if (do_mtdparts_default()) {	// mtdparts default
		tx_status(status, FASTBOOT_REPLY_FAIL
			  "do_mtdparts_default() failed");
		return;
	}

	if (ubi_part("boot", NULL)) {	// ubi part boot
		debug("%s, line %d:  UBI partition \"boot\" does not exist\n",
		      __FILE__, __LINE__);
		tx_status(status, FASTBOOT_REPLY_FAIL
			  "UBI partition \"boot\" does not exist");
		return;
	}

	if (ubi_volume_exists(par_name)) {
		// For now, this command doesn't do any work.  Erasing
		// a UBI volume isn't a prerequisite for rewriting the
		// volume.  By avoiding this work, it's hoped that the
		// life of the NAND will be extended.
		//
		// flash_erase(...);
		tx_status(status, FASTBOOT_REPLY_OKAY);
		return;
	}


	if (ubi_part("system", NULL)) {	// ubi part system
		debug("%s, line %d:  UBI partition \"system\" does not exist\n",
		      __FILE__, __LINE__);
		tx_status(status, FASTBOOT_REPLY_FAIL
			  "UBI partition \"system\" does not exist");
		return;
	}

	if (ubi_volume_exists(par_name)) {
		// For now, this command doesn't do any work.  Erasing
		// a UBI volume isn't a prerequisite for rewriting the
		// volume.  By avoiding this work, it's hoped that the
		// life of the NAND will be extended.
		//
		// flash_erase(...);
		tx_status(status, FASTBOOT_REPLY_OKAY);
	} else {
		tx_status(status, FASTBOOT_REPLY_FAIL
			  "partition %s does not exist", ptnparam);
	}
}

void reboot_cb(void)
{
	_machine_restart();
}

void do_cmd_reboot_bootloader(USB_STATUS * status)
{
	cpm_set_scrpad(BOOTLOADER_SIGNATURE);

	printf("\nprocess--->reboot-bootloader\n");
	status->cb = reboot_cb;
	tx_status(status, FASTBOOT_REPLY_OKAY);
}

extern void tcu_writel(uint32_t val, uint32_t off);

void do_cmd_reboot(USB_STATUS * status)
{
	printf("\nRestarting after 4 ms\n");
	cpm_set_scrpad(REBOOT_SIGNATURE);

	printf("\nprocess--->reboot\n");
	status->cb = reboot_cb;
	tx_status(status, FASTBOOT_REPLY_OKAY);
}

void nand_boot(int nand_boot_select)
{
	debug("TODO:  %s\n", __func__);
}

bootm_headers_t images;

void jz4780_handle_fastboot(USB_STATUS * status)
{
	char strparam[MAX_PACKET_SIZE + 1];
	char ptnparam[MAX_PACKET_SIZE + 1];
	int intparam;

	if (Bulk_Out_Size) {
		printf("\nprocess fastboot command : \n");

		if (Bulk_Out_Size > MAX_PACKET_SIZE) {
			Bulk_Out_Buf[MAX_PACKET_SIZE + 1] = '\0';
		} else {
			Bulk_Out_Buf[Bulk_Out_Size] = '\0';
		}

		debug("%s, line %d:  %s\n", __FILE__, __LINE__, Bulk_Out_Buf);

		if (sscanf((char *)Bulk_Out_Buf, "getvar:%s", strparam) == 1) {
			do_cmd_getvar(status, strparam);
		} else if (sscanf((char *)Bulk_Out_Buf,
				  "download:%x", &intparam) == 1) {
			do_cmd_download(status, intparam);
		} else if (sscanf((char *)Bulk_Out_Buf,
				  "flash:%s", ptnparam) == 1) {
			do_cmd_flash(status, ptnparam);
			debug("%s, line %d:  do_cmd_flash() flashed \"%s\"\n",
			      __FILE__, __LINE__, ptnparam);
		} else if (sscanf((char *)Bulk_Out_Buf,
				  "erase:%s", ptnparam) == 1) {
			do_cmd_erase(status, ptnparam);
		} else if (!strcmp((char *)Bulk_Out_Buf, "reboot-bootloader")) {
			// TODO: Need to decide what U-Boot ought to do
			//  when it finds the marker upon reboot.
			// U-Boot ought to do when it finds the
			// marker upon reboot.
			// TODO: Does U-Boot have a mechanism for
			// reflashing itself via Fastboot protocol?
			// If so, having support in Fastboot for
			// rebooting the bootloader would make sense.
			// What is the mechanism?

			/* Leave marker in scratchpad. */
			do_cmd_reboot_bootloader(status);
		} else if (!strcmp((char *)Bulk_Out_Buf, "reboot")) {
			/* Leave marker in scratchpad. */
			do_cmd_reboot(status);
		} else if (!strcmp((char *)Bulk_Out_Buf, "continue")) {
			// TODO:  Need to figure out and implement
			// what it means to continue.
// TODO:  NORMAL_BOOT?
#define NORMAL_BOOT                     1
			tx_status(status, FASTBOOT_REPLY_OKAY);
			// TODO:  Need to allow reply to host complete
			// before calling nand_boot.  Maybe stuff the
			// call to nand_boot() onto an execution queue,
			// and set a flag which will be monitored by
			// the main loop to execute the next item in
			// the queue?
			nand_boot(NORMAL_BOOT);
		} else if (!strcmp((char *)Bulk_Out_Buf, "boot")) {
			tx_status(status, FASTBOOT_REPLY_OKAY);
			do_boota_linux(&images, Bulk_Data_Buf);
		} else if (!strcmp((char *)Bulk_Out_Buf, "powerdown")) {
			tx_status(status, FASTBOOT_REPLY_OKAY);
			// TODO:  There's a way to do this on the CI20.
			// Just need to find it in the Linux kernel and
			// implement that logic here.
			/* do_powerdown(); */
		} else {	// Invalid command
			printf("***** ERROR Unknown Fastboot Command: %s\n",
			       (char *)Bulk_Out_Buf);
			tx_status(status, FASTBOOT_REPLY_FAIL
				  "Unknown command: %s", (char *)Bulk_Out_Buf);
		}
	}

cmd_finish:
	Bulk_Out_Size = 0;
	memset(Bulk_Out_Buf, 0, BULK_OUT_BUF_SIZE);
}

void fastboot_usb_poll(USB_STATUS * status)
{
	u32 intsts;
	static int usb_reset_flag = 0;
	//printf("=====================\n");

	intsts = otg_readl(GINT_STS);
	//debug("0x%08X\n", intsts);
#if 0
	if (0 == usb_reset_flag) {
		mdelay(5000);
		debug("idle for usb \n");
	}
#endif
	if (intsts & GINTSTS_USB_EARLYSUSPEND) {
		//printf("=====================\n");
		handle_early_suspend_intr();
	}

	if (intsts & GINTSTS_START_FRAM) {
		handle_start_frame_intr();
	}

	/* reset interrupt handle */
	if (intsts & GINTSTS_USB_RESET) {
		//printf("=====================\n");
		handle_reset_intr();
	}

	/* enum done */
	if (intsts & GINTSTS_ENUM_DONE) {
		//printf("=====================\n");
		//pmon_clear_cnt();
		//pmon_stop();
		handle_enum_done_intr(&usb_reset_flag);
	}

	if (intsts & GINTSTS_RXFIFO_NEMPTY) {
		//printf("=====================\n");
		handle_rxfifo_nempty(status);
	}

	if (intsts & GINTSTS_IEP_INTR) {
		handle_inep_intr(status);
	}

	if (intsts & GINTSTS_OEP_INTR) {
		handle_outep_intr(status);
	}

	jz4780_handle_fastboot(status);
}

/*
 * USB boot routine
 *
 * Desc: usb boot.
 * Retn: 0 for success, -1 for error.
 */
int fastboot_usb_boot(unsigned zero, unsigned type, unsigned tags)
{
	USB_STATUS status = { NULL, {0, 0}, 0, 0, 0, NULL };

	flash_dump_ptn();
	usbloader_init();
	for (;;) {
		fastboot_usb_poll(&status);
	}

	return 0;
}

/*
 * fastboot_usb_init() - platform specific code to initialize the USB
 *                       port being used for Fastboot protocol.
 */
void fastboot_usb_init(void)
{
	gpio_disable_pull(GPIO_USB_DETECT);
	gpio_disable_pull(GPIO_DC_DETE_N);

	gpio_direction_input(GPIO_USB_DETECT);
	gpio_direction_input(GPIO_DC_DETE_N);
	int i;
	for (i = 3; i; --i) {
		gpio_get_pin(GPIO_USB_DETECT);
		gpio_get_pin(GPIO_DC_DETE_N);
	}
}

/*
 * fastboot_usb_detected() - platform specific code to recognize
 *                           activity on the USB port being used for
 *                           Fastboot protocol.
 */
int fastboot_usb_detected(void)
{
	return gpio_get_pin(GPIO_USB_DETECT);
}
