/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 */

#ifndef __DDR4PHY_REGS_H__
#define __DDR4PHY_REGS_H__

#include <utils.h>

#define PHY_REG_SHIFT		2

#define PHY_RIDR		(0x000 << (PHY_REG_SHIFT))
#define PHY_PIR			(0x001 << (PHY_REG_SHIFT))
#define   PHY_PIR_INIT			BIT(0)	/* Initialization Trigger */
#define   PHY_PIR_ZCAL			BIT(1)	/* Impedance Calibration */
#define   PHY_PIR_PLLINIT		BIT(4)	/* PLL Initialization */
#define   PHY_PIR_DCAL			BIT(5)	/* DDL Calibration */
#define   PHY_PIR_PHYRST		BIT(6)	/* PHY Reset */
#define   PHY_PIR_DRAMRST		BIT(7)	/* DRAM Reset */
#define   PHY_PIR_DRAMINIT		BIT(8)	/* DRAM Initialization */
#define   PHY_PIR_WL			BIT(9)	/* Write Leveling */
#define   PHY_PIR_QSGATE		BIT(10)	/* Read DQS Gate Training */
#define   PHY_PIR_WLADJ			BIT(11)	/* Write Leveling Adjust */
#define   PHY_PIR_RDDSKW		BIT(12)	/* Read Data Bit Deskew */
#define   PHY_PIR_WRDSKW		BIT(13)	/* Write Data Bit Deskew */
#define   PHY_PIR_RDEYE			BIT(14)	/* Read Data Eye Training */
#define   PHY_PIR_WREYE			BIT(15)	/* Write Data Eye Training */
#define   PHY_PIR_CTLDINIT		BIT(18)	/* Controller DRAM init */
#define PHY_PGCR0		(0x004 << (PHY_REG_SHIFT))
#define PHY_PGCR1		(0x005 << (PHY_REG_SHIFT))
#define PHY_PGCR2		(0x006 << (PHY_REG_SHIFT))
#define PHY_PGCR3		(0x007 << (PHY_REG_SHIFT))
#define PHY_PGCR4		(0x008 << (PHY_REG_SHIFT))
#define PHY_PGCR5		(0x009 << (PHY_REG_SHIFT))
#define PHY_PGCR6		(0x00a << (PHY_REG_SHIFT))
#define   PHY_PGCR6_INHVT		BIT(0)	/* VT Calculation Inhibit */
#define PHY_PGCR7		(0x00b << (PHY_REG_SHIFT))
#define PHY_PGCR8		(0x00c << (PHY_REG_SHIFT))
#define PHY_PGSR0		(0x00d << (PHY_REG_SHIFT))
#define   PHY_PGSR0_IDONE		BIT(0)	/* Initialization Done */
#define   PHY_PGSR0_PLDONE		BIT(1)	/* PLL Lock Done */
#define   PHY_PGSR0_DCDONE		BIT(2)	/* DDL Calibration Done */
#define   PHY_PGSR0_ZCDONE		BIT(3)	/* Impedance Calibration Done */
#define   PHY_PGSR0_DIDONE		BIT(4)	/* DRAM Initialization Done */
#define   PHY_PGSR0_WLDONE		BIT(5)	/* Write Leveling Done */
#define   PHY_PGSR0_QSGDONE		BIT(6)	/* DQS Gate Training Done */
#define   PHY_PGSR0_WLADONE		BIT(7)	/* Write Leveling Adjust Done */
#define   PHY_PGSR0_RDDONE		BIT(8)	/* Read Bit Deskew Done */
#define   PHY_PGSR0_WDDONE		BIT(9)	/* Write Bit Deskew Done */
#define   PHY_PGSR0_REDONE		BIT(10)	/* Read Eye Training Done */
#define   PHY_PGSR0_WEDONE		BIT(11)	/* Write Eye Training Done */
#define   PHY_PGSR0_CADONE		BIT(12)	/* CA Training Done */
#define   PHY_PGSR0_SRDDONE		BIT(13)	/* Static Read Done */
#define   PHY_PGSR0_VDONE		BIT(14)	/* VREF Training Done */
#define   PHY_PGSR0_VERR		BIT(19)	/* VREF Training Error */
#define   PHY_PGSR0_ZCERR		BIT(20)	/* Impedance Calibration Error */
#define   PHY_PGSR0_WLERR		BIT(21)	/* Write Leveling Error */
#define   PHY_PGSR0_QSGERR		BIT(22)	/* DQS Gate Training Error */
#define   PHY_PGSR0_WLAERR		BIT(23)	/* Write Leveling Adj Error */
#define   PHY_PGSR0_RDERR		BIT(24)	/* Read Bit Deskew Error */
#define   PHY_PGSR0_WDERR		BIT(25)	/* Write Bit Deskew Error */
#define   PHY_PGSR0_REERR		BIT(26)	/* Read Eye Training Error */
#define   PHY_PGSR0_WEERR		BIT(27)	/* Write Eye Training Error */
#define   PHY_PGSR0_CAERR		BIT(28)	/* CA Training Error */
#define PHY_PGSR1		(0x00e << (PHY_REG_SHIFT))
#define   PHY_PGSR1_VTSTOP		BIT(30)	/* VT Stop */
#define PHY_PTR0		(0x010 << (PHY_REG_SHIFT))
#define PHY_PTR1		(0x011 << (PHY_REG_SHIFT))
#define PHY_PTR2		(0x012 << (PHY_REG_SHIFT))
#define PHY_PTR3		(0x013 << (PHY_REG_SHIFT))
#define PHY_PTR4		(0x014 << (PHY_REG_SHIFT))
#define PHY_PTR5		(0x015 << (PHY_REG_SHIFT))
#define PHY_PTR6		(0x016 << (PHY_REG_SHIFT))
#define PHY_PLLCR		(0x020 << (PHY_REG_SHIFT))
#define PHY_DXCCR		(0x022 << (PHY_REG_SHIFT))
#define PHY_DSGCR		(0x024 << (PHY_REG_SHIFT))
#define PHY_ODTCR		(0x026 << (PHY_REG_SHIFT))
#define PHY_AACR		(0x028 << (PHY_REG_SHIFT))
#define PHY_DCR			(0x040 << (PHY_REG_SHIFT))
#define PHY_DTPR0		(0x044 << (PHY_REG_SHIFT))
#define PHY_DTPR1		(0x045 << (PHY_REG_SHIFT))
#define PHY_DTPR2		(0x046 << (PHY_REG_SHIFT))
#define PHY_DTPR3		(0x047 << (PHY_REG_SHIFT))
#define PHY_DTPR4		(0x048 << (PHY_REG_SHIFT))
#define PHY_DTPR5		(0x049 << (PHY_REG_SHIFT))
#define PHY_DTPR6		(0x04a << (PHY_REG_SHIFT))
#define PHY_MR0			(0x060 << (PHY_REG_SHIFT))
#define PHY_MR1			(0x061 << (PHY_REG_SHIFT))
#define PHY_MR2			(0x062 << (PHY_REG_SHIFT))
#define PHY_MR3			(0x063 << (PHY_REG_SHIFT))
#define PHY_DTCR0		(0x080 << (PHY_REG_SHIFT))
#define PHY_DTCR1		(0x081 << (PHY_REG_SHIFT))
#define PHY_DCUAR		(0x0c0 << (PHY_REG_SHIFT))
#define PHY_DCUDR		(0x0c1 << (PHY_REG_SHIFT))
#define PHY_DCURR		(0x0c2 << (PHY_REG_SHIFT))
#define PHY_DCULR		(0x0c3 << (PHY_REG_SHIFT))
#define PHY_DCUTPR		(0x0c5 << (PHY_REG_SHIFT))
#define PHY_DCUSR0		(0x0c6 << (PHY_REG_SHIFT))
#define PHY_RANKIDR		(0x137 << (PHY_REG_SHIFT))
#define PHY_ACIOCR0		(0x140 << (PHY_REG_SHIFT))
#define PHY_ACIOCR1		(0x141 << (PHY_REG_SHIFT))
#define PHY_ACIOCR2		(0x142 << (PHY_REG_SHIFT))
#define PHY_ACIOCR3		(0x143 << (PHY_REG_SHIFT))
#define PHY_ACIOCR4		(0x144 << (PHY_REG_SHIFT))
#define PHY_ACBDLR0		(0x150 << (PHY_REG_SHIFT))
#define PHY_ACBDLR1		(0x151 << (PHY_REG_SHIFT))
#define PHY_ACBDLR2		(0x152 << (PHY_REG_SHIFT))
#define PHY_ACBDLR3		(0x153 << (PHY_REG_SHIFT))
#define PHY_ACBDLR4		(0x154 << (PHY_REG_SHIFT))
#define PHY_ACBDLR5		(0x155 << (PHY_REG_SHIFT))
#define PHY_ACBDLR6		(0x156 << (PHY_REG_SHIFT))
#define PHY_ACBDLR7		(0x157 << (PHY_REG_SHIFT))
#define PHY_ACBDLR8		(0x158 << (PHY_REG_SHIFT))
#define PHY_ACBDLR9		(0x159 << (PHY_REG_SHIFT))
#define PHY_ACMDLR0		(0x168 << (PHY_REG_SHIFT))
#define PHY_ZQCR		(0x1a0 << (PHY_REG_SHIFT))
#define PHY_ZQ0PR		(0x1a1 << (PHY_REG_SHIFT))
#define PHY_ZQ0DR		(0x1a2 << (PHY_REG_SHIFT))
#define PHY_ZQ1PR		(0x1a5 << (PHY_REG_SHIFT))
#define PHY_ZQ1DR		(0x1a6 << (PHY_REG_SHIFT))
#define PHY_ZQ2PR		(0x1a9 << (PHY_REG_SHIFT))
#define PHY_ZQ2DR		(0x1aa << (PHY_REG_SHIFT))
#define PHY_DXGCR0(dx)		((0x1c0 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXGCR1(dx)		((0x1c1 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXGCR2(dx)		((0x1c2 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXGCR3(dx)		((0x1c3 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXGCR4(dx)		((0x1c4 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXBDLR2(dx)		((0x1d2 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXLCDLR0(dx)	((0x1e0 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXLCDLR1(dx)	((0x1e1 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXLCDLR2(dx)	((0x1e2 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXLCDLR3(dx)	((0x1e3 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXLCDLR4(dx)	((0x1e4 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXMDLR0(dx)		((0x1e8 + 0x040 * (dx)) << (PHY_REG_SHIFT))
#define PHY_DXGTR0(dx)		((0x1f0 + 0x040 * (dx)) << (PHY_REG_SHIFT))

#define PHY_LCDL_WIDTH			9
#define	PHY_LCDL_MASK			GENMASK(PHY_LCDL_WIDTH - 1, 0)

#define PHY_BIT_DLY_WIDTH		6
#define PHY_BIT_DLY_SHIFT		8
#define	PHY_BITDLY_MASK			GENMASK(PHY_BIT_DLY_WIDTH - 1, 0)
#define	PHY_BITDLY01_MASK		(PHY_BITDLY_MASK | PHY_BITDLY_MASK << \
						PHY_BIT_DLY_SHIFT)
#define	PHY_BITDLY02_MASK		(PHY_BITDLY01_MASK | PHY_BITDLY_MASK << \
						(2 * PHY_BIT_DLY_SHIFT))
#define	PHY_BITDLY03_MASK		(PHY_BITDLY02_MASK | PHY_BITDLY_MASK << \
						(3 * PHY_BIT_DLY_SHIFT))

#define	PHY_IPRD_MASK			GENMASK(PHY_LCDL_WIDTH - 1, 0)
#define	PHY_CK0BD_MASK			GENMASK(PHY_BIT_DLY_WIDTH - 1, 0)
#define PHY_DSWBD_SHIFT			8
#define	PHY_DSWBD_MASK			(GENMASK(PHY_BIT_DLY_WIDTH - 1, 0) << \
						PHY_DSWBD_SHIFT)
#define PHY_DSOEBD_SHIFT		16
#define	PHY_DSOEBD_MASK			(GENMASK(PHY_BIT_DLY_WIDTH - 1, 0) << \
						PHY_DSOEBD_SHIFT)

#define PHY_RANKRID_SHIFT		16
#define PHY_RANKIRD_WIDTH		4
#define PHY_RANKWID_MASK		GENMASK(PHY_RANKIRD_WIDTH - 1, 0)
#define PHY_RANKRID_MASK		(GENMASK(PHY_RANKIRD_WIDTH - 1, 0)  << \
						PHY_RANKRID_SHIFT)

#define PHY_DQSGX_WIDTH			2
#define PHY_DQSGX_SHIFT			6
#define PHY_DQSGX_MASK			(GENMASK(PHY_DQSGX_WIDTH - 1, 0)  << \
						PHY_DQSGX_SHIFT)

#endif /* __DDR4PHY_REGS_H__ */
