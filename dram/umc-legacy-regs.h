/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2016 Socionext Inc.
 */

#ifndef __UMC_REGS_H__
#define __UMC_REGS_H__

/*** DC Register ***/
#define UMC_CMDCTLA		0x00000000
#define UMC_CMDCTLB		0x00000004
#define UMC_INITSET		0x00000014
#define UMC_INITSTAT		0x00000018
#define UMC_SPCCTLA		0x00000030
#define UMC_SPCCTLB		0x00000034
#define UMC_SPCSETB		0x0000003C
#define UMC_SPCSETC		0x00000040
#define UMC_SPCSTATC            0x00000058
#define UMC_ACSSETA		0x00000060
#define UMC_ZQCSCTLA            0x00000090
#define UMC_ZQCSCTLB            0x00000094
#define UMC_ZQCSCTLC            0x00000098
#define UMC_FLOWCTLA            0x00000400
#define UMC_FLOWCTLB            0x00000404
#define UMC_FLOWCTLC            0x00000408
#define UMC_FLOWCTLG            0x00000508
#define UMC_FLOWCTLOB0          0x00000520
#define UMC_FLOWCTLOB1          0x00000524
#define UMC_FLOWCTLOB2          0x00000528
#define UMC_FLOWCTLOB3          0x00000530
#define UMC_FLOWCTLOB4          0x00000534
#define UMC_RDATACTL_D0		0x00000600
#define UMC_WDATACTL_D0		0x00000604
#define UMC_RDATACTL_D1		0x00000608
#define UMC_WDATACTL_D1		0x0000060C
#define UMC_DATASET		0x00000610
#define UMC_DCCGCTL		0x00000720
#define UMC_ERRMASKA		0x00000958
#define UMC_ERRMASKB		0x0000095C
#define UMC_BSICMAPSET          0x00000988
#define UMC_DFICUPDCTLA		0x00000C20
#define UMC_DFILPCTLA		0x00000C40

/*** UMC (except DC) Register ***/

#define BITPERPIXELMODE_D0      0x00000010

/*** MBUS Register ***/
/* UM registers */
#define UMC_MBUS0		0x00080004
#define UMC_MBUS1		0x00081004
#define UMC_MBUS2		0x00082004
#define UMC_MBUS3		0x00083004
#define UMC_MBUS4		0x00084004

#endif /* __UMC_REGS_H__ */
