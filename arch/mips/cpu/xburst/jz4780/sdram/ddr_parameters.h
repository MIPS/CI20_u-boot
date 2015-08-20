/*
 * JZ4780 DDR initialization - parameters definitions
 *
 * Copyright (c) 2015 Imagination Technologies
 * Author: Matt Redfearn <matt.redfearn.com>
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

#ifndef __DDR_PARAMETERS_H
#define __DDR_PARAMETERS_H


/* Paramters common to all RAM devices used */

/* Chip Select */
#define DDR_CS0EN	1	/* CSEN : whether a ddr chip exists 0 - un-used, 1 - used */
#define DDR_CS1EN	0	/* CSEN : whether a ddr chip exists 0 - un-used, 1 - used */

#define DDR_ROW		15	/* ROW : 12 to 18 row address ,1G only 512MB*/
#define DDR_COL		10	/* COL :  8 to 14 column address */
#define DDR_BANK8	1	/* Banks each chip: 0-4bank, 1-8bank */
#define DDR_DW32	1	/* 0 - 16-bit data width, 1 - 32-bit data width */

#define DDR_tREFI	7800	/* Refresh period: 64ms / 32768 = 1.95 us , 2 ^ 15 = 32768 */ 
#define DDR_CLK_DIV	1	/* Clock Divider */

#define DDR_BL	   	8	/* DDR3 Burst length: 0 - 8 burst, 2 - 4 burst , 1 - 4 or 8(on the fly)*/

#define DDR_CL		6	/* CAS latency: 5 to 14 ,tCK*/
#define DDR_tCWL	(DDR_CL - 1) /* DDR3 only: CAS Write Latency, 5 to 8 */

/* Structure representing per-RAM type configuration */

struct jz4780_ddr_config {
	const char *name;	/* Part name */

	u32	timing1;	/* Timing1 register value */
	u32	timing2;	/* Timing2 register value */
	u32	timing3;	/* Timing3 register value */
	u32	timing4;	/* Timing4 register value */
	u32	timing5;	/* Timing5 register value */
	u32	timing6;	/* Timing6 register value */

	/* DDR PHY control */

	u32	mr0;	/* Mode Register 0 */
	u32	mr1;	/* Mode Register 1 */

	u32	ptr0;	/* PHY Timing Register 0 */
	u32	ptr1;	/* PHY Timing Register 1 */
	u32	ptr2;	/* PHY Timing Register 1 */

	u32	dtpr0;	/* DRAM Timing Parameters Register 0 */
	u32	dtpr1;	/* DRAM Timing Parameters Register 1 */
	u32	dtpr2;	/* DRAM Timing Parameters Register 2 */

	u8	pullup;	/* PHY pullup impedance */
	u8	pulldn;	/* PHY pulldown impedance */
};


#endif /* __DDR_PARAMETERS_H */
