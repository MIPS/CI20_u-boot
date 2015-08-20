#ifndef __DDR3_K4B2G0846Q_CONFIG_H
#define __DDR3_K4B2G0846Q_CONFIG_H

#if 0

#define SDRAM_PART_NAME "K4B2G0846Q"
#define SDRAM_TYPE_DDR3

#define DDR_MAX(tck, time)                              \
({                                      \
    unsigned long value;                            \
    value = (tck * ps > time) ? (tck * ps) : time;              \
    value = (value % 1000 == 0) ? (value / 1000) : (value / 1000 + 1);  \
    value;                                  \
})

/*
 * This file contains the memory configuration parameters for the cygnus board.
 */
/*--------------------------------------------------------------------------------
 * DDR3-1066 info
 */
/* Chip Select */
#define DDR_CS1EN  0  /* CSEN : whether a ddr chip exists 0 - un-used, 1 - used */
#define DDR_CS0EN  1
#define DDR_DW32   1  /* 0 - 16-bit data width, 1 - 32-bit data width */
//#define SDRAM_DISABLE_DLL

/* DDR3 paramters */
//2 chip
//#define DDR_ROW     14  /* ROW : 12 to 18 row address ,1G only 512MB*/
//4chip
#define DDR_ROW     15  /* ROW : 12 to 18 row address ,1G only 512MB*/
#define DDR_COL     10  /* COL :  8 to 14 column address */
#define DDR_BANK8   1 	/* Banks each chip: 0-4bank, 1-8bank */

#ifdef SDRAM_DISABLE_DLL
#define DDR_CL      6   /* dll off */
#define DDR_tCWL    6	/* DDR3 dll off*/
#else
#define DDR_CL      6   /* CAS latency: 5 to 14 ,tCK*/
#define DDR_tCWL   (DDR_CL - 1) /* DDR3 only: CAS Write Latency, 5 to 8 */
#endif

/*
 * DDR3 controller timing1 register
 */
#define DDR_tRAS 37  /* tRAS: ACTIVE to PRECHARGE command period to the same bank. ns*/
#define DDR_tRP  15  /* tRP: PRECHARGE command period to the same bank. ns*/
#define DDR_tRCD 15  /* ACTIVE to READ or WRITE command period to the same bank. ns*/
#define DDR_tRC  52  /* ACTIVE to ACTIVE command period to the same bank. ns*/
#define DDR_tWR  15  /* WRITE Recovery Time defined by register MR of DDR2 memory, ns*/
#define DDR_tRRD DDR_MAX(4, 6000) /* ACTIVE bank A to ACTIVE bank B command period. DDR3 - tCK*/
#define DDR_tRTP DDR_MAX(4, 7500) /* READ to PRECHARGE command period. DDR3 spec no. 7.5ns*/
#define DDR_tWTR DDR_MAX(4, 7500) /* WRITE to READ command delay. DDR3 spec no. 7.5 ns*/
	 
/*
 * DDR3 controller timing2 register
 */
#define DDR_tRFC   160  /* AUTO-REFRESH command period. DDR3 - ns*/
#define DDR_tMINSR 80   /* Minimum Self-Refresh / Deep-Power-Down . DDR3 no*/
#define DDR_tXP    DDR_MAX(3, 6000)	/* DDR3 only: Exit active power down to any valid command, ns*/
#define DDR_tMRD   4    /* unit: tCK. Load-Mode-Register to next valid command period: DDR3 rang 4 to 7 tCK. DDR3 spec no */

/* new add */
#define DDR_BL	   8   /* DDR3 Burst length: 0 - 8 burst, 2 - 4 burst , 1 - 4 or 8(on the fly)*/
#define DDR_tAL    0	/* Additive Latency, tCK*/
#define DDR_tCCD   4	/* CAS# to CAS# command delay , tCK. 4 or 5 */
#define DDR_tFAW   30	/* Four bank activate period, DDR3 - tCK */
#define DDR_tCKE   	DDR_MAX(3, 5000)	/* CKE minimum pulse width, DDR3 spec no, tCK */
#define DDR_tRL 	(DDR_tAL + DDR_CL)	/* DDR3: Read Latency = tAL + tCL */
#define DDR_tWL 	(DDR_tAL + DDR_tCWL)	/* DDR3: Write Latency = tAL + tCWL */
#define DDR_tRDLAT	(DDR_tRL - 2)
#define DDR_tWDLAT	(DDR_tWL - 1)
#define DDR_tRTW 	(DDR_tRL + DDR_tCCD + 2 - DDR_tWL + 1)	/* Read to Write delay */
#define DDR_tCKSRE 	DDR_MAX(5, 10000) /* Valid Clock Requirement after Self Refresh Entry or Power-Down Entry */

#define DDR_tDLLLOCK	512		/* DDR3 only: DLL LOCK, tck */
#define DDR_tXSDLL 	DDR_MAX(DDR_tDLLLOCK, 0)		/* DDR3 only: EXit self-refresh to command requiring a locked DLL, tck*/
#define DDR_tMOD   	DDR_MAX(12, 15 * 1000)	/* DDR3 only: Mode Register Set Command update delay*/
#define DDR_tXPDLL 	DDR_MAX(10, 24 * 1000)	 /* DDR3 only: Exit active power down to command requirint a locked DLL, ns*/
#define DDR_tXS    	DDR_MAX(5, (DDR_tRFC + 10) * 1000) /* DDR3 only: EXit self-refresh to command not requiring a locked DLL, ns*/
#define DDR_tXSRD  	100		/* DDR2 only: Exit self refresh to a read command, tck */

/*
 * DDR3 controller refcnt register
 */
#define DDR_tREFI   7800	/* Refresh period: 64ms / 32768 = 1.95 us , 2 ^ 15 = 32768 */
#define DDR_CLK_DIV 1    	/* Clock Divider. auto refresh
			  *	cnt_clk = memclk/(16*(2^DDR_CLK_DIV))
			  */
#endif

#include "ddr_parameters.h"


static const struct jz4780_ddr_config K4B2G0846Q_48_config = {
	.name		= "K4B2G0846Q",

	.timing1	= (	(4	<< DDRC_TIMING1_TRTP_BIT)	|
				(13	<< DDRC_TIMING1_TWTR_BIT)	|
				(6	<< DDRC_TIMING1_TWR_BIT)	|
				(5	<< DDRC_TIMING1_TWL_BIT)),

	.timing2	= (	(4	<< DDRC_TIMING2_TCCD_BIT)	|
				(15	<< DDRC_TIMING2_TRAS_BIT)	|
				(6	<< DDRC_TIMING2_TRCD_BIT)	|
				(6	<< DDRC_TIMING2_TRL_BIT)),

	.timing3	= (	(4	<< DDRC_TIMING3_ONUM)		|
				(7	<< DDRC_TIMING3_TCKSRE_BIT)	|
				(6	<< DDRC_TIMING3_TRP_BIT)	|
				(4	<< DDRC_TIMING3_TRRD_BIT)	|
				(21	<< DDRC_TIMING3_TRC_BIT)),

	.timing4	= (	(31	<< DDRC_TIMING4_TRFC_BIT)	|
				(1	<< DDRC_TIMING4_TRWCOV_BIT)	|
				(4	<< DDRC_TIMING4_TCKE_BIT)	|
				(9	<< DDRC_TIMING4_TMINSR_BIT)	|
				(8	<< DDRC_TIMING4_TXP_BIT)	|
				(3	<< DDRC_TIMING4_TMRD_BIT)),

	.timing5	= (	(8	<< DDRC_TIMING5_TRTW_BIT)	|
				(4	<< DDRC_TIMING5_TRDLAT_BIT)	|
				(4	<< DDRC_TIMING5_TWDLAT_BIT)),

	.timing6	= (	(25	<< DDRC_TIMING6_TXSRD_BIT)	|
				(12	<< DDRC_TIMING6_TFAW_BIT)	|
				(2	<< DDRC_TIMING6_TCFGW_BIT)	|
				(2	<< DDRC_TIMING6_TCFGR_BIT)),
	/* PHY */

	/* Mode Register 0 */
	.mr0		= 	(0x00000420),
#ifdef SDRAM_DISABLE_DLL
	.mr1		=	(DDR3_MR1_DIC_7 | DDR3_MR1_RTT_DIS | DDR3_MR1_DLL_DISABLE),
#else
	.mr1		=	(DDR3_MR1_DIC_7 | DDR3_MR1_RTT_DIS),
#endif

	.ptr0		=	0x002000d4,
	.ptr1		=	0x02230d40,
	.ptr2		=	0x04013880,

	.dtpr0		=	0x2a8f6690,
	.dtpr1		=	0x00400860,
	.dtpr2		=	0x10042a00,

	.pullup		=	0x0b,
	.pulldn		=	0x0b,
};
#endif /* __DDR3_K4B2G0846Q_CONFIG_H */
