/*
 * Copyright (C) 2016 Socionext Inc.
 */

#ifndef __UMC_REGS_H__
#define __UMC_REGS_H__

#include <utils.h>

#define UMC_CMDCTLA		0x00000000
#define UMC_CMDCTLB		0x00000004
#define UMC_CMDCTLC		0x00000008
#define UMC_INITCTLA		0x00000020
#define UMC_INITCTLB		0x00000024
#define UMC_INITCTLC		0x00000028
#define UMC_DRMMR0		0x00000030
#define UMC_DRMMR1		0x00000034
#define UMC_DRMMR2		0x00000038
#define UMC_DRMMR3		0x0000003C
#define UMC_INITSET		0x00000040
#define UMC_INITSTAT		0x00000044
#define UMC_CMDCTLE		0x00000050
#define UMC_CMDCTLF		0x00000054
#define UMC_CMDCTLG		0x00000058
#define UMC_SPCSETB		0x00000084
#define   UMC_SPCSETB_AREFMD_MASK	(0x3)	/* Auto Refresh Mode */
#define   UMC_SPCSETB_AREFMD_ARB	(0x0)	/* control by arbitor */
#define   UMC_SPCSETB_AREFMD_CONT	(0x1)	/* control by DRAMCONT */
#define   UMC_SPCSETB_AREFMD_REG	(0x2)	/* control by register */
#define UMC_ACSSETA		0x000000C0
#define UMC_ACSSETB		0x000000C4
#define UMC_MEMCONF0A		0x00000200
#define UMC_MEMCONF0B		0x00000204
#define UMC_MEMCONFCH		0x00000240
#define UMC_MEMMAPSET		0x00000250
#define UMC_FLOWCTLA		0x00000400
#define UMC_FLOWCTLB		0x00000404
#define UMC_FLOWCTLC		0x00000408
#define UMC_ACFETCHCTRL		0x00000460
#define UMC_FLOWCTLG		0x00000508
#define UMC_RDATACTL_D0		0x00000600
#define UMC_WDATACTL_D0		0x00000604
#define UMC_RDATACTL_D1		0x00000608
#define UMC_WDATACTL_D1		0x0000060C
#define UMC_DATASET		0x00000610
#define UMC_ODTCTL_D0		0x00000618
#define UMC_ODTCTL_D1		0x0000061C
#define UMC_RESPCTL		0x00000624
#define UMC_DIRECTBUSCTRLA	0x00000680
#define UMC_DEBUGC		0x00000718
#define UMC_DCCGCTL		0x00000720
#define UMC_DICGCTLA		0x00000724
#define UMC_DICGCTLB		0x00000728
#define UMC_ERRMASKA		0x00000958
#define UMC_ERRMASKB		0x0000095C
#define UMC_BSICMAPSET		0x00000988
#define UMC_DIOCTLA		0x00000C00
#define   UMC_DIOCTLA_CTL_NRST		BIT(8)	/* ctl_rst_n */
#define   UMC_DIOCTLA_CFG_NRST		BIT(0)	/* cfg_rst_n */
#define UMC_DFISTCTLC		0x00000C18
#define UMC_DFICUPDCTLA		0x00000C20
#define UMC_DFIPUPDCTLA		0x00000C30
#define UMC_DFICSOVRRD		0x00000C84
#define UMC_DFITURNOFF          0x00000C88

/* UM registers */
#define UMC_MBUS0		0x00080004
#define UMC_MBUS1		0x00081004
#define UMC_MBUS2		0x00082004
#define UMC_MBUS3		0x00000C78
#define UMC_MBUS4		0x00000CF8
#define UMC_MBUS5		0x00000E78
#define UMC_MBUS6		0x00000EF8
#define UMC_MBUS7		0x00001278
#define UMC_MBUS8		0x000012F8
#define UMC_MBUS9		0x00002478
#define UMC_MBUS10		0x000024F8

/* UMC1 register */
#define UMC_SIORST		0x00000728
#define UMC_VO0RST		0x0000073c
#define UMC_VPERST		0x00000744
#define UMC_RGLRST		0x00000750
#define UMC_A2DRST		0x00000764
#define UMC_DMDRST		0x00000770

#endif /* __UMC_REGS_H__ */
