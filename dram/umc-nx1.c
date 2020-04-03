// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2020 Socionext Inc.

#include <board-data.h>
#include <delay.h>
#include <errno.h>
#include <io.h>
#include <printk.h>
#include <sizes.h>
#include <types.h>
#include <umc.h>

#include "ddr4phy-regs.h"
#include "umc-nx1-regs.h"

#define DRAM_DX_NR	4
#define DDRPHY_MAX_CALIB_RETRY 3

/* PHY */
#define PHY_RANK_NR	2

static const int ddrphy_rof_pos_shift[DRAM_DX_NR] = {0, 0, 0, 0};
static const int ddrphy_rof_neg_shift[DRAM_DX_NR] = {0, 0, 0, 0};

static void ddrphy_maskwritel(u32 data, u32 mask, void __iomem *addr)
{
	u32 value;

	value = (readl(addr) & ~(mask)) | (data & mask);
	writel(value, addr);
}

static u32 ddrphy_maskreadl(u32 mask, void __iomem *addr)
{
	return readl(addr) & mask;
}

static void ddrphy_zqcalib(void __iomem *phy_base)
{
	/* ZQ ODT Override OFF */
	writel(0x1007DD00, phy_base + PHY_ZQ0PR);
	writel(0x1007DD00, phy_base + PHY_ZQ1PR);
	writel(0x1007DD00, phy_base + PHY_ZQ2PR);

	/* ZQ Calibration */
	writel(PHY_PIR_ZCAL | PHY_PIR_INIT, phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();

	/* ZQ Force Update */
	writel(0x08058F00, phy_base + PHY_ZQCR);
	writel(0x00058F00, phy_base + PHY_ZQCR);
}

static int ddrphy_hpstep(int delay, int dx, unsigned int freq,
			 void __iomem *phy_base)
{
	int mdl;

	if (dx == -1)
		mdl = ddrphy_maskreadl(PHY_IPRD_MASK, phy_base + PHY_ACMDLR0);
	else
		mdl = ddrphy_maskreadl(PHY_IPRD_MASK, phy_base + PHY_DXMDLR0(dx));

	return DIV_ROUND_CLOSEST((int)freq * delay * mdl, 1000000);
}

static void ddrphy_vt_ctrl(void __iomem *phy_base, int enable)
{
	u32 tmp;

	tmp = readl(phy_base + PHY_PGCR6);

	if (enable)
		tmp &= ~PHY_PGCR6_INHVT;
	else
		tmp |= PHY_PGCR6_INHVT;

	writel(tmp, phy_base + PHY_PGCR6);

	if (!enable) {
		while (!(readl(phy_base + PHY_PGSR1) & PHY_PGSR1_VTSTOP))
			cpu_relax();
	}
}

static void ddrphy_ch_init(void __iomem *phy_base, int resume)
{
	int i;

	/* DDRPHY Initialization */
	writel(0x07D81E00, phy_base + PHY_PGCR0);
	writel(0x0200C6A0, phy_base + PHY_PGCR1);
	writel(0x0000B540, phy_base + PHY_PGCR2);
	writel(0xF0AA0040, phy_base + PHY_PGCR3);
	writel(0x40000000, phy_base + PHY_PGCR4);
	writel(0x01010000, phy_base + PHY_PGCR5);
	writel(0x00010000, phy_base + PHY_PGCR6);
	writel(0x00040000, phy_base + PHY_PGCR7);
	writel(0x00000080, phy_base + PHY_PGCR8);
	writel(0x53629AD0, phy_base + PHY_PTR0);
	writel(0x208D9771, phy_base + PHY_PTR1);
	writel(0x00083DEF, phy_base + PHY_PTR2);
	writel(0x2EBA3161, phy_base + PHY_PTR3);
	writel(0x1003FFFF, phy_base + PHY_PTR4);
	writel(0x2042C216, phy_base + PHY_PTR5);
	writel(0x00000D06, phy_base + PHY_PTR6);
	writel(0x00038000, phy_base + PHY_PLLCR);
	writel(0x00058F00, phy_base + PHY_ZQCR);
	writel(0x1007DD00, phy_base + PHY_ZQ0PR);
	writel(0x1007DD00, phy_base + PHY_ZQ1PR);
	writel(0x1007DD00, phy_base + PHY_ZQ2PR);
	writel(0x0000040C, phy_base + PHY_DCR);
	writel(0x00000A44, phy_base + PHY_MR0);
	writel(0x00000000, phy_base + PHY_RANKIDR);
	writel(0x00000101, phy_base + PHY_MR1);
	writel(0x00000000, phy_base + PHY_RANKIDR);
	writel(0x00000030, phy_base + PHY_MR2);
	writel(0x00000000, phy_base + PHY_RANKIDR);
	writel(0x00000000, phy_base + PHY_MR3);
	writel(0x00000000, phy_base + PHY_MR4);
	writel(0x00000000, phy_base + PHY_RANKIDR);
	writel(0x00000440, phy_base + PHY_MR5);
	writel(0x00000000, phy_base + PHY_RANKIDR);
	writel(0x00000C00, phy_base + PHY_MR6);
	writel(0x092B140A, phy_base + PHY_DTPR0);
	writel(0x28280010, phy_base + PHY_DTPR1);
	writel(0x10080356, phy_base + PHY_DTPR2);
	writel(0x03560101, phy_base + PHY_DTPR3);
	writel(0x01D30F08, phy_base + PHY_DTPR4);
	writel(0x003F140A, phy_base + PHY_DTPR5);
	writel(0x00000505, phy_base + PHY_DTPR6);
	writel(0x20C01004, phy_base + PHY_DXCCR);
	writel(0x0064401E, phy_base + PHY_DSGCR);
	writel(0x00000000, phy_base + PHY_RANKIDR);
	writel(0x00010000, phy_base + PHY_ODTCR);
	writel(0x00000001, phy_base + PHY_RANKIDR);
	writel(0x00020000, phy_base + PHY_ODTCR);
	writel(0x000000FF, phy_base + PHY_AACR);
	writel(0x0000B0C7, phy_base + PHY_DTCR0);
	writel(0x00030A31, phy_base + PHY_DTCR1);
	writel(0x30000410, phy_base + PHY_ACIOCR0);
	writel(0x00000000, phy_base + PHY_ACIOCR1);
	writel(0x00000000, phy_base + PHY_ACIOCR2);
	writel(0x00000000, phy_base + PHY_ACIOCR3);
	writel(0x00000000, phy_base + PHY_ACIOCR4);

	for (i = 0; i < 4; i += 1) {
		writel(0x40400205, phy_base + PHY_DXGCR0(i));
		writel(0x00000000, phy_base + PHY_DXGCR1(i));
		writel(0x00000000, phy_base + PHY_DXGCR2(i));
		writel(0xF7FC0000, phy_base + PHY_DXGCR3(i));
		writel(0x0E00000C, phy_base + PHY_DXGCR4(i));
		writel(0x19191919, phy_base + PHY_DXGCR5(i));
		writel(0x09090909, phy_base + PHY_DXGCR6(i));
	}

	writel(0x70032020, phy_base + PHY_VTCR0);
	writel(0x0CA88094, phy_base + PHY_VTCR1);

	writel(PHY_PIR_PHYRST | PHY_PIR_DCAL | PHY_PIR_PLLINIT |
	       PHY_PIR_ZCAL | PHY_PIR_INIT,
	       phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();

	if (resume) {
		/* recovery from retention mode */

		/* DRAM INIT bypass */
		writel(PHY_PIR_CTLDINIT | PHY_PIR_INIT, phy_base + PHY_PIR);
		while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
			cpu_relax();

		/* ZQ/ODT override -> force update */
		writel(0x00003030, phy_base + PHY_ZQ0DR);
		writel(0x00003030, phy_base + PHY_ZQ1DR);
		writel(0x00003030, phy_base + PHY_ZQ2DR);
		writel(0xD007DD00, phy_base + PHY_ZQ0PR);
		writel(0xD007DD00, phy_base + PHY_ZQ1PR);
		writel(0xD007DD00, phy_base + PHY_ZQ2PR);
		writel(0x08058F00, phy_base + PHY_ZQCR);
		writel(0x00058F00, phy_base + PHY_ZQCR);

	} else {	/* normal flow */
		/* DRAM INIT */
		writel(0x00000181, phy_base + PHY_PIR);
		while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
			cpu_relax();

		/* VREF initialization */
		writel(0x0200C6E0, phy_base + PHY_PGCR1);
		writel(0x000C8064, phy_base + PHY_SCHCR1);
		writel(0x00000011, phy_base + PHY_SCHCR0);
		/* need to add wait(100ns) here */
		udelay(1);
		writel(0x0200C6A0, phy_base + PHY_PGCR1);
		/* need to add wait(150ns) here */
		udelay(1);
		writel(0x0200C6E0, phy_base + PHY_PGCR1);
		writel(0x000C9F64, phy_base + PHY_SCHCR1);
		writel(0x00000011, phy_base + PHY_SCHCR0);
		/* need to add wait(100ns) here */
		udelay(1);
		writel(0x0200C6A0, phy_base + PHY_PGCR1);
		writel(0x0200C6E0, phy_base + PHY_PGCR1);
		writel(0x000C0064, phy_base + PHY_SCHCR1);
		writel(0x00000011, phy_base + PHY_SCHCR0);
		/* need to add wait(100ns) here */
		udelay(1);
		writel(0x0200C6A0, phy_base + PHY_PGCR1);
		/* need to add wait(150ns) here*/
		udelay(1);

	}
}

static void ddrphy_shift_rof_hws(void __iomem *phy_base, const int *pos_shift,
				 const int *neg_shift, unsigned int freq)
{
	int dx;
	u32 rankrid, rankwid;
	int rdqsd, rdqnsd;

	ddrphy_vt_ctrl(phy_base, 0);

	for (dx = 0; dx < 4; dx++) {
		rankrid = (u32)(dx / 2);
		ddrphy_maskwritel(rankrid << PHY_RANKRID_SHIFT,
				PHY_RANKRID_MASK, phy_base + PHY_RANKIDR);
		readl(phy_base + PHY_RANKIDR); /* relax */

		rdqsd = ddrphy_maskreadl(PHY_LCDL_MASK,
					 phy_base + PHY_DXLCDLR3(dx));
		rdqnsd = ddrphy_maskreadl(PHY_LCDL_MASK,
					  phy_base + PHY_DXLCDLR4(dx));

		rdqsd = min(max(rdqsd + ddrphy_hpstep(pos_shift[dx], dx,
					freq, phy_base), 0), 511);
		rdqnsd = min(max(rdqnsd + ddrphy_hpstep(neg_shift[dx], dx,
					freq, phy_base), 0), 511);

		for (rankwid = 0; rankwid < PHY_RANK_NR; rankwid++) {
			ddrphy_maskwritel(rankwid,
				PHY_RANKWID_MASK, phy_base + PHY_RANKIDR);
			readl(phy_base + PHY_RANKIDR);	/* relax */

			ddrphy_maskwritel(rdqsd, PHY_LCDL_MASK,
					  phy_base + PHY_DXLCDLR3(dx));
			ddrphy_maskwritel(rdqnsd, PHY_LCDL_MASK,
					  phy_base + PHY_DXLCDLR4(dx));
		}
		readl(phy_base + PHY_DXLCDLR3(dx));	/* relax */
		readl(phy_base + PHY_DXLCDLR4(dx));	/* relax */
	}

	ddrphy_vt_ctrl(phy_base, 1);
}

/* DCU run for PHY training */
static void ddrphy_rundcu(void __iomem *phy_base)
{
	writel(0x020046A0, phy_base + PHY_PGCR1);
	writel(0x00000004, phy_base + PHY_DCURR);
	writel(0x00000001, phy_base + PHY_DCURR);
	while (!(readl(phy_base + PHY_DCUSR0) & PHY_DCUSR0_RDONE))
		cpu_relax();
	writel(0x0200C6A0, phy_base + PHY_PGCR1);
}

static bool ddrphy_training(void __iomem *phy_base, int resume)
{
	u32 dxgcr5_val[4];
	u32 pgsr0_val;

	/* set DCU */
	writel(0x00000800, phy_base + PHY_DCUTPR);
	writel(0x00000400, phy_base + PHY_DCUAR);
	writel(0x000001e0, phy_base + PHY_DCUDR);
	writel(0x00003213, phy_base + PHY_DCUDR);
	writel(0x00000000, phy_base + PHY_DCULR);

	writel(PHY_PIR_WL | PHY_PIR_INIT, phy_base + PHY_PIR);
	while (!((pgsr0_val = readl(phy_base + PHY_PGSR0)) & PHY_PGSR0_IDONE))
		cpu_relax();
	if (pgsr0_val & PHY_PGSR0_ERR_MASK)
		return false;

	writel(0x0000B0C7, phy_base + PHY_DTCR0);
	writel(0x00030A37, phy_base + PHY_DTCR1);
	writel(0x20801004, phy_base + PHY_DXCCR);
	/* Exec DQS Gating Training(1) */
	writel(0x00000401, phy_base + PHY_PIR);
	while (!((pgsr0_val = readl(phy_base + PHY_PGSR0)) & PHY_PGSR0_IDONE))
		cpu_relax();
	if (pgsr0_val & PHY_PGSR0_ERR_MASK)
		return false;

	writel(0x20C01004, phy_base + PHY_DXCCR);
	/* Exec WLADJ, RDDSKW, WRDSKW Training */

	writel(0x00003801, phy_base + PHY_PIR);
	while (!((pgsr0_val = readl(phy_base + PHY_PGSR0)) & PHY_PGSR0_IDONE))
		cpu_relax();
	if (pgsr0_val & PHY_PGSR0_ERR_MASK)
		return false;

	/* run DCU (1) */
	ddrphy_rundcu(phy_base);

	writel(0x0000B0C7, phy_base + PHY_DTCR0);
	/* Exec RDEYE Training */
	writel(PHY_PIR_RDEYE | PHY_PIR_INIT, phy_base + PHY_PIR);
	while (!((pgsr0_val = readl(phy_base + PHY_PGSR0)) & PHY_PGSR0_IDONE))
		cpu_relax();
	if (pgsr0_val & PHY_PGSR0_ERR_MASK)
		return false;

	writel(0x0000B0C7, phy_base + PHY_DTCR0);

	/* run DCU (2) */
	ddrphy_rundcu(phy_base);

	/* Exec WREYE Training */
	writel(PHY_PIR_WREYE | PHY_PIR_INIT, phy_base + PHY_PIR);
	while (!((pgsr0_val = readl(phy_base + PHY_PGSR0)) & PHY_PGSR0_IDONE))
		cpu_relax();
	if (pgsr0_val & PHY_PGSR0_ERR_MASK)
		return false;

	/* Static read mode */
	writel(0xF1AA0048, phy_base + PHY_PGCR3);

	/* VREF Calibration */
	writel(0x0000B0C7, phy_base + PHY_DTCR0);
	writel(0xA5A5A5A5, phy_base + PHY_BISTUDPR);
	writel(0x03dc0000, phy_base + PHY_BISTRR);

	/* skip DRAM VREF training if recovery from retention mode */
	if (resume)
		writel(0x60032020, phy_base + PHY_VTCR0);

	writel(0x40400205, phy_base + PHY_DXGCR0(0));
	writel(0x40400205, phy_base + PHY_DXGCR0(1));
	writel(0x40400204, phy_base + PHY_DXGCR0(2));
	writel(0x40400204, phy_base + PHY_DXGCR0(3));
	writel(0x19191919, phy_base + PHY_DXGCR5(0));
	writel(0x19191919, phy_base + PHY_DXGCR5(1));
	writel(0x00010A31, phy_base + PHY_DTCR1);
	writel(0x00000000, phy_base + PHY_BISTAR1);

	/* run DCU (3) */
	ddrphy_rundcu(phy_base);

	/* VREF training for normal or skip from retention */
	if (!resume) {
		writel(0x00020001, phy_base + PHY_PIR);
		while (!((pgsr0_val = readl(phy_base + PHY_PGSR0))
			& PHY_PGSR0_IDONE))
			cpu_relax();
		if (pgsr0_val & PHY_PGSR0_ERR_MASK)
			return false;
	}

	dxgcr5_val[0] = readl(phy_base + PHY_DXGCR5(0));
	dxgcr5_val[1] = readl(phy_base + PHY_DXGCR5(1));

	writel(0x40400204, phy_base + PHY_DXGCR0(0));
	writel(0x40400204, phy_base + PHY_DXGCR0(1));
	writel(0x40400205, phy_base + PHY_DXGCR0(2));
	writel(0x40400205, phy_base + PHY_DXGCR0(3));
	writel(0x19191919, phy_base + PHY_DXGCR5(2));
	writel(0x19191919, phy_base + PHY_DXGCR5(3));
	writel(0x00020A31, phy_base + PHY_DTCR1);
	writel(0x00010001, phy_base + PHY_BISTAR1);

	/* run DCU (4) */
	ddrphy_rundcu(phy_base);

	/* VREF training for normal or skip from retention */
	if (!resume) {
		writel(0x00020001, phy_base + PHY_PIR);
		while (!((pgsr0_val = readl(phy_base + PHY_PGSR0))
			& PHY_PGSR0_IDONE))
			cpu_relax();
		if (pgsr0_val & PHY_PGSR0_ERR_MASK)
			return false;
	}

	dxgcr5_val[2] = readl(phy_base + PHY_DXGCR5(2));
	dxgcr5_val[3] = readl(phy_base + PHY_DXGCR5(3));

	writel(dxgcr5_val[0], phy_base + PHY_DXGCR5(0));
	writel(dxgcr5_val[1], phy_base + PHY_DXGCR5(1));
	writel(dxgcr5_val[2], phy_base + PHY_DXGCR5(2));
	writel(dxgcr5_val[3], phy_base + PHY_DXGCR5(3));
	writel(0x0000B0C7, phy_base + PHY_DTCR0);
	writel(0x40400205, phy_base + PHY_DXGCR0(0));
	writel(0x40400205, phy_base + PHY_DXGCR0(1));
	writel(0x40400205, phy_base + PHY_DXGCR0(2));
	writel(0x40400205, phy_base + PHY_DXGCR0(3));

	return true;
}

#define RSTCTL6			(IOMEM(0x11842018))
#define DDRPHY_RST_SHIFT	(24)
#define DDRPHY_RST_MASK		(BIT(DDRPHY_RST_SHIFT))

static void ddrphy_reset(void)
{
	/* ddrphy reset */
	ddrphy_maskwritel((0x0 << DDRPHY_RST_SHIFT), DDRPHY_RST_MASK, RSTCTL6);
	udelay(1);
	readl(RSTCTL6);

	/* release ddrphy reset */
	ddrphy_maskwritel((u32)(0x1 << DDRPHY_RST_SHIFT), DDRPHY_RST_MASK, RSTCTL6);
	udelay(1);
	readl(RSTCTL6);
}

/* UMC */

/* check if recovery from retention mode */
static int umc_is_instant_boot(void __iomem *dc_base)
{
	return !!(readl(dc_base + UMC_SPCSTATC) & BIT(0));
}

/* stop retention mode */
static void umc_instant_boot_retention_ctl(void __iomem *dc_base, int resume)
{
	if (resume)
		writel(0x80100002, dc_base + UMC_SPCSETD);
}

static void umc_init_set(void __iomem *dc_base)
{
	/* initialize UMC internal state */
	writel(0x00000100, dc_base + UMC_INITSET);

	/* wait for init completion */
	while ((readl(dc_base + UMC_INITSTAT) & BIT(8)))
		cpu_relax();

	/* dfi_lp_req OFF Setting */
	writel(0x80000000, dc_base + UMC_DFILPCTLA);

	/* dfi_lp_req is linkd with power down */
	// writel(0x00000000, dc_base + UMC_DFILPCTLA);

	/* dfi_cke LOW Setting */
	writel(0x10000000, dc_base + UMC_SPCSETE);

	/* dfi_cke HIGH Setting */
	writel(0x00000000, dc_base + UMC_SPCSETE);

	/* dfi_reset_n HIGH Setting */
	writel(0x00000001, dc_base + UMC_SPCSETF);

	/* dfi_init_start HIGH Setting */
	writel(0x40000000, dc_base + UMC_DFISTCTLC);
}

static void umc_ch_init(void __iomem *dc_base, void __iomem *phy_base,
			int resume)
{
	if (resume) {
		/* set dfi_lp_req=0 */
		writel(0x80000000, dc_base + UMC_DFILPCTLA);

		/* wait for UMCDFILPCTLB[0] = 1 */
		while ((readl(dc_base + UMC_DFILPCTLB) & BIT(0)))
			cpu_relax();

		ddrphy_zqcalib(phy_base);

		writel(0x80000001, dc_base + UMC_DFILPCTLA);

		/* wait for UMCDFILPCTLB[0] = 1 */
		while ((readl(dc_base + UMC_DFILPCTLB) & BIT(0)))
			cpu_relax();

		writel(0x00000000, dc_base + UMC_DFILPCTLA);
	}

	/* set AC Spec for Write/Read */
	writel(0x0814142B, dc_base + UMC_CMDCTLA);
	writel(0x3F282A0A, dc_base + UMC_CMDCTLB);
	writel(0x201A0C04, dc_base + UMC_CMDCTLC);

	/*set DDR4 ACspec*/
	writel(0x00000009, dc_base + UMC_CMDCTLDDR4A);
	writel(0x00200007, dc_base + UMC_CMDCTLDDR4B);

	/* set AC Spec for SPC Com(Ref/PWDN/SLFR) */
	writel(0x017109F0, dc_base + UMC_CMDCTLE);
	writel(0x00030176, dc_base + UMC_CMDCTLF);
	writel(0x04040207, dc_base + UMC_CMDCTLG);

	/* set ZPCS */
	writel(0x03F8FF90, dc_base + UMC_CMDCTLH);

	/* DFI I/F Timing Setting */
	writel(0x00000010, dc_base + UMC_RDATACTL_D0);
	writel(0x00000010, dc_base + UMC_RDATACTL_D1);
	writel(0x00000C10, dc_base + UMC_WDATACTL_D0);
	writel(0x00000C10, dc_base + UMC_WDATACTL_D1);
	writel(0x00000000, dc_base + UMC_DATASET_D0);
	writel(0x00000000, dc_base + UMC_DATASET_D1);

	/* DBI read only setting */
	writel(0x00000100, dc_base + UMC_DBICTL_D0);
	writel(0x00000100, dc_base + UMC_DBICTL_D1);

	/* CS Timing Setting */
	writel(0x00001111, dc_base + UMC_ACCESSCTL_D0);
	writel(0x00001111, dc_base + UMC_ACCESSCTL_D1);

	/* ODT Length */
	writel(0x42000203, dc_base + UMC_ODTCTL_D0);
	writel(0x42000203, dc_base + UMC_ODTCTL_D1);

	/* Mem Conf 32Gb x32 BL8 */
	writel(0x00000C01, dc_base + UMC_MEMCONF0A);
	writel(0x00001450, dc_base + UMC_MEMCONF0B);
	writel(0x00033C04, dc_base + UMC_MEMCONFCH);

	/* Bank type*/
	writel(0x00000001, dc_base + UMC_MEMMAPSETA);

	/* Data threshold*/
	writel(0x00006048, dc_base + UMC_CELLCTRL);

	/* Bank policy */
	writel(0x00010001, dc_base + UMC_CACSSETA);

	/* Precharge issue Mode */
	writel(0x00000103, dc_base + UMC_FLOWCTLG);

	/* dfi_lp_reg Mode */
	writel(0x00000000, dc_base + UMC_DFILPCTLA);

	/* ZQSC AREF Auto Control Setting */
	writel(0x00020200, dc_base + UMC_SPCSETB);
}

static void um_init(void __iomem *um_base)
{
	writel(0x0000ffff, um_base + UMC_MBUS0);
	writel(0x0000ffff, um_base + UMC_MBUS1);
	writel(0x0000ffff, um_base + UMC_MBUS2);
	writel(0x0000ffff, um_base + UMC_MBUS3);
}

static void um_init_reset(void __iomem *um_base)
{
	writel(0x00000000, um_base + UMC_MBUS0);
	writel(0x00000000, um_base + UMC_MBUS1);
	writel(0x00000000, um_base + UMC_MBUS2);
	writel(0x00000000, um_base + UMC_MBUS3);
}

#define UM_BASE			(IOMEM(0x1b600000))
#define UMC_BASE		(IOMEM(0x1b800000))
#define UMC_DC_BASE		(UMC_BASE + 0x00007000)
#define PHY_BASE		(IOMEM(0x1e200000))

int nx1_umc_init(const struct board_data *bd)
{
	int num_calib_retry, max_calib_retry, resume;
	bool ddrphy_enable;

	switch (bd->dram_freq) {
	case 2666:
		break;
	default:
		pr_err("unsupported DRAM frequency %ud MHz\n", bd->dram_freq);
		return -EINVAL;
	}

	/* define max retry number of ddrphy training */
	max_calib_retry = DDRPHY_MAX_CALIB_RETRY;

	/* initialization */
	ddrphy_enable = false;
	num_calib_retry = 0;

	resume = umc_is_instant_boot(UMC_DC_BASE);

	while (ddrphy_enable == false && num_calib_retry < max_calib_retry) {

		/* initial setting */
		if (!resume)
			umc_init_set(UMC_DC_BASE);
		ddrphy_ch_init(PHY_BASE, resume);

		/* stop retention mode */
		umc_instant_boot_retention_ctl(UMC_DC_BASE, resume);

		/* UMC initial setting */
		umc_ch_init(UMC_DC_BASE, PHY_BASE, resume);

		/* set MBUS */
		um_init(UM_BASE);

		/* calibration for DDRPHY parameter */
		/* memory dummy read */
		readl(IOMEM(bd->dram_ch[0].base));	/* rank0 */
		readl(IOMEM(bd->dram_ch[0].base + 8));	/* rank1 */

		/* stop auto-refresh, ZQ calibration */
		writel(0x02020202, UMC_DC_BASE + UMC_SPCSETB);

		/* DDRPHY Training */
		ddrphy_enable = ddrphy_training(PHY_BASE, resume);

		/* DIO roffset shift after training */
		ddrphy_shift_rof_hws(PHY_BASE, ddrphy_rof_pos_shift,
				     ddrphy_rof_neg_shift, bd->dram_freq);

		/* restart auto-refresh, ZQ calibration */
		writel(0x00020200, UMC_DC_BASE + UMC_SPCSETB);

		/* Static read mode */
		writel(0xF1AA0048, PHY_BASE + PHY_PGCR3);

		/* increment number of training nun */
		num_calib_retry++;

		/* If training fail, reset MBUS and DDRPHY */
		if (ddrphy_enable == false) {
			/* reset MBUS */
			um_init_reset(UM_BASE);

			/* reset DDRPHY */
			ddrphy_reset();

			/* set retention if resume = 1 */
			if (resume)
				writel(0x00100002, UMC_DC_BASE + UMC_SPCSETD);
		}
	}

	return 0;
}
