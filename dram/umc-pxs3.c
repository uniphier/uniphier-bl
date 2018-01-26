/*
 * Copyright (C) 2017 Socionext Inc.
 *
 * based on commit 40ca68de3bc947d49b7da6ad864ffb7cebcb2e0c of Diag
 */

#include <errno.h>
#include <init.h>
#include <io.h>
#include <printk.h>
#include <sizes.h>
#include <types.h>

#include "ddr4phy-regs.h"
#include "umc-legacy-regs.h"

#define DRAM_CH_NR	3

enum dram_freq {
	DRAM_FREQ_2133M,
	DRAM_FREQ_NR,
};

enum dram_size {
	DRAM_SZ_256M,
	DRAM_SZ_512M,
	DRAM_SZ_1G,
	DRAM_SZ_NR,
};

enum dram_board {		/* board type */
	DRAM_BOARD_PXS3_REF,	/* PXs3 reference */
	DRAM_BOARD_NR,
};

/* PHY */
#define PHY_RANK_NR	2

static const int ddrphy_ckoffset[DRAM_BOARD_NR][DRAM_CH_NR] = {
	{0, 0, 0},		/* PXs3 reference */
};

static const int ddrphy_rof_pos_shift[DRAM_BOARD_NR][DRAM_CH_NR][4] = {
	{	/* PXS3 reference board */
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	},
};
static const int ddrphy_rof_neg_shift[DRAM_BOARD_NR][DRAM_CH_NR][4] = {
	{	/* PXS3 reference board */
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	},
};

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
	/* ZQ/ODT Override OFF */
	writel(0x00000000, phy_base + PHY_ZQ0DR);
	writel(0x00000000, phy_base + PHY_ZQ1DR);
	writel(0x00000000, phy_base + PHY_ZQ2DR);

	/* ZQ calibration */
	writel(PHY_PIR_ZCAL | PHY_PIR_INIT, phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();

	/* ZQ Force Update */
	writel(0x0C058F00, phy_base + PHY_ZQCR);
	writel(0x04058F00, phy_base + PHY_ZQCR);
}

static void ddrphy_ch_init(void __iomem *phy_base, int resume)
{
	/* DDRPHY Initialization  */
	writel(0x07D81E00, phy_base + PHY_PGCR0);
	writel(0x0200C6A0, phy_base + PHY_PGCR1);
	writel(0x000090B0, phy_base + PHY_PGCR2);
	writel(0xF0AA0040, phy_base + PHY_PGCR3);
	writel(0x40000000, phy_base + PHY_PGCR4);
	writel(0x01010000, phy_base + PHY_PGCR5);
	writel(0x00010000, phy_base + PHY_PGCR6);
	writel(0x00040000, phy_base + PHY_PGCR7);
	writel(0x00000100, phy_base + PHY_PGCR8);
	writel(0x42C21590, phy_base + PHY_PTR0);
	writel(0x1A0B12C1, phy_base + PHY_PTR1);
	writel(0x00083DEF, phy_base + PHY_PTR2);
	writel(0x25682781, phy_base + PHY_PTR3);
	writel(0x10034156, phy_base + PHY_PTR4);
	writel(0x203581AB, phy_base + PHY_PTR5);
	writel(0x00000A6B, phy_base + PHY_PTR6);
	writel(0x00038000, phy_base + PHY_PLLCR);
	writel(0x04058F00, phy_base + PHY_ZQCR);
	writel(0x00000059, phy_base + PHY_ZQ0PR);
	writel(0x0000005D, phy_base + PHY_ZQ1PR);
	writel(0x0000005D, phy_base + PHY_ZQ2PR);
	writel(0x0000040B, phy_base + PHY_DCR);
	writel(0x00000125, phy_base + PHY_MR0);
	writel(0x00000000, phy_base + PHY_RANKIDR);
	writel(0x00000006, phy_base + PHY_MR1);
	writel(0x00000000, phy_base + PHY_RANKIDR);
	writel(0x000002A8, phy_base + PHY_MR2);
	writel(0x08240E08, phy_base + PHY_DTPR0);
	writel(0x28260404, phy_base + PHY_DTPR1);
	writel(0x10080200, phy_base + PHY_DTPR2);
	writel(0x02000101, phy_base + PHY_DTPR3);
	writel(0x01760F1A, phy_base + PHY_DTPR4);
	writel(0x00320E08, phy_base + PHY_DTPR5);
	writel(0x00000505, phy_base + PHY_DTPR6);
	writel(0x20401884, phy_base + PHY_DXCCR);
	writel(0x0064401E, phy_base + PHY_DSGCR);
	writel(0x00000000, phy_base + PHY_RANKIDR);
	writel(0x00010000, phy_base + PHY_ODTCR);
	writel(0x00000001, phy_base + PHY_RANKIDR);
	writel(0x00020000, phy_base + PHY_ODTCR);
	writel(0x000000FF, phy_base + PHY_AACR);
	writel(0x9000B0C7, phy_base + PHY_DTCR0);
	writel(0x00030231, phy_base + PHY_DTCR1);
	writel(0x30000410, phy_base + PHY_ACIOCR0);
	writel(0x00000000, phy_base + PHY_ACIOCR1);
	writel(0x00000000, phy_base + PHY_ACIOCR2);
	writel(0x00000000, phy_base + PHY_ACIOCR3);
	writel(0x00000000, phy_base + PHY_ACIOCR4);
	writel(0x40400205, phy_base + PHY_DXGCR0(0));
	writel(0x00000000, phy_base + PHY_DXGCR1(0));
	writel(0x00000000, phy_base + PHY_DXGCR2(0));
	writel(0xF7FC0000, phy_base + PHY_DXGCR3(0));
	writel(0x0E00000C, phy_base + PHY_DXGCR4(0));
	writel(0x40400205, phy_base + PHY_DXGCR0(1));
	writel(0x00000000, phy_base + PHY_DXGCR1(1));
	writel(0x00000000, phy_base + PHY_DXGCR2(1));
	writel(0xF7FC0000, phy_base + PHY_DXGCR3(1));
	writel(0x0E00000C, phy_base + PHY_DXGCR4(1));
	writel(0x40400205, phy_base + PHY_DXGCR0(2));
	writel(0x00000000, phy_base + PHY_DXGCR1(2));
	writel(0x00000000, phy_base + PHY_DXGCR2(2));
	writel(0xF7FC0000, phy_base + PHY_DXGCR3(2));
	writel(0x0E00000C, phy_base + PHY_DXGCR4(2));
	writel(0x40400205, phy_base + PHY_DXGCR0(3));
	writel(0x00000000, phy_base + PHY_DXGCR1(3));
	writel(0x00000000, phy_base + PHY_DXGCR2(3));
	writel(0xF7FC0000, phy_base + PHY_DXGCR3(3));
	writel(0x0E00000C, phy_base + PHY_DXGCR4(3));
	writel(PHY_PIR_PHYRST | PHY_PIR_DCAL | PHY_PIR_PLLINIT |
	       PHY_PIR_ZCAL | PHY_PIR_INIT,
	       phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();

	if (resume) {
		/* DRAM INIT Bypass */
		writel(PHY_PIR_CTLDINIT | PHY_PIR_INIT, phy_base + PHY_PIR);
		while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
			cpu_relax();

		/* ZQ/ODT Override -> Force Update */
		writel(0xC0E1C891, phy_base + PHY_ZQ0DR);
		writel(0xC0E1C891, phy_base + PHY_ZQ1DR);
		writel(0xC0E1C891, phy_base + PHY_ZQ2DR);
		writel(0x0C058F00, phy_base + PHY_ZQCR);
		writel(0x04058F00, phy_base + PHY_ZQCR);
	} else {
		/* DRAM INIT */
		writel(PHY_PIR_DRAMINIT | PHY_PIR_DRAMRST | PHY_PIR_INIT,
		       phy_base + PHY_PIR);
		while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
			cpu_relax();
	}
}

static int ddrphy_hpstep(int delay, int dx, unsigned int freq, void __iomem *phy_base)
{
	int mdl;

	if (dx == -1)
		mdl = ddrphy_maskreadl(PHY_IPRD_MASK, phy_base + PHY_ACMDLR0);
	else
		mdl = ddrphy_maskreadl(PHY_IPRD_MASK, phy_base + PHY_DXMDLR0(dx));

	return DIV_ROUND_CLOSEST((long)freq * delay * mdl, 1000000L);
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

static void ddrphy_set_ckoffset_qoffset(int delay_ckoffset, int delay_qoffset,
					unsigned int freq, void __iomem *phy_base)
{
	u32 ck_step;	/* ckoffset_step for clock */
	u32 q_step;	/*  qoffset_step for clock */
	u32 q_step_bitdly01, q_step_bitdly02, q_step_bitdly03;

	ck_step = min(max(ddrphy_hpstep(delay_ckoffset, -1, freq, phy_base), 0),
		      (int)PHY_CK0BD_MASK);     /* CK-Offset */
	q_step = min(max(ddrphy_hpstep(delay_qoffset, -1, freq, phy_base), 0),
		     (int)PHY_BITDLY_MASK);     /*  Q-Offset */

	q_step_bitdly01 = q_step          | (q_step << PHY_BIT_DLY_SHIFT);
	q_step_bitdly02 = q_step_bitdly01 | (q_step << (2 * PHY_BIT_DLY_SHIFT));
	q_step_bitdly03 = q_step_bitdly02 | (q_step << (3 * PHY_BIT_DLY_SHIFT));

	ddrphy_vt_ctrl(phy_base, 0);

	ddrphy_maskwritel(ck_step, PHY_CK0BD_MASK, phy_base + PHY_ACBDLR0);
	/* ACBDLR1 [21:16], [13:8], [5:0] */
	ddrphy_maskwritel(q_step_bitdly02, PHY_BITDLY02_MASK,
			  phy_base + PHY_ACBDLR1);
	/* ACBDLR2 [21:16], [13:8], [5:0] */
	ddrphy_maskwritel(q_step_bitdly02, PHY_BITDLY02_MASK,
			  phy_base + PHY_ACBDLR2);
	/* ACBDLR3 [13:8], [5:0] */
	ddrphy_maskwritel(q_step_bitdly01, PHY_BITDLY01_MASK,
			  phy_base + PHY_ACBDLR3);
	/* ACBDLR4 [13:8], [5:0] */
	ddrphy_maskwritel(q_step_bitdly01, PHY_BITDLY01_MASK,
			  phy_base + PHY_ACBDLR4);
	/* ACBDLR5 [13:8], [5:0] */
	ddrphy_maskwritel(q_step_bitdly01, PHY_BITDLY01_MASK,
			  phy_base + PHY_ACBDLR5);
	/* ACBDLR6 [29:24], [21:16], [13:8], [5:0] */
	ddrphy_maskwritel(q_step_bitdly03, PHY_BITDLY03_MASK,
			  phy_base + PHY_ACBDLR6);
	/* ACBDLR7 [29:24], [21:16], [13:8], [5:0] */
	ddrphy_maskwritel(q_step_bitdly03, PHY_BITDLY03_MASK,
			  phy_base + PHY_ACBDLR7);
	/* ACBDLR8 [29:24], [21:16], [13:8], [5:0] */
	ddrphy_maskwritel(q_step_bitdly03, PHY_BITDLY03_MASK,
			  phy_base + PHY_ACBDLR8);
	/* ACBDLR9 [29:24], [21:16], [13:8], [5:0] */
	ddrphy_maskwritel(q_step_bitdly03, PHY_BITDLY03_MASK,
			  phy_base + PHY_ACBDLR9);

	ddrphy_vt_ctrl(phy_base, 1);
}

static void ddrphy_training(void __iomem *phy_base)
{
	/* Beginning of  DCU setting */
	writel(0x00000800, phy_base + PHY_DCUTPR);
	writel(0x00000400, phy_base + PHY_DCUAR);
	writel(0x80000000, phy_base + PHY_DCUDR);
	writel(0x00001909, phy_base + PHY_DCUDR);
	writel(0x00000000, phy_base + PHY_DCULR);
	/* End of DCU setting */

	writel(PHY_PIR_WL | PHY_PIR_INIT, phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();
	writel(0x9000B0C7, phy_base + PHY_DTCR0);
	writel(0x20001884, phy_base + PHY_DXCCR);
	/*
	 * To work around the hardware bug, run Read DQS Gate Training twice;
	 * first with the Basic Gate Training, second with Read Leveling.
	 */
	writel(0x00030231, phy_base + PHY_DTCR1);
	writel(0x00000401, phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();
	writel(0x00030232, phy_base + PHY_DTCR1);
	writel(0x00000401, phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();
	writel(0x20401884, phy_base + PHY_DXCCR);
	writel(PHY_PIR_WRDSKW | PHY_PIR_RDDSKW | PHY_PIR_WLADJ | PHY_PIR_INIT,
	       phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();

	/* Beginning of 1st DCU run */
	writel(0x020046A0, phy_base + PHY_PGCR1);
	writel(0x00000004, phy_base + PHY_DCURR);
	writel(0x00000001, phy_base + PHY_DCURR);
	while (!(readl(phy_base + PHY_DCUSR0) & BIT(0)))
		cpu_relax();
	writel(0x0200C6A0, phy_base + PHY_PGCR1);
	/* End of 1st DCU run */

	writel(0x0000B0C7, phy_base + PHY_DTCR0);
	writel(PHY_PIR_RDEYE | PHY_PIR_INIT, phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();
	writel(0x9000B0C7, phy_base + PHY_DTCR0);

	/* Beginning of 2nd DCU run */
	writel(0x020046A0, phy_base + PHY_PGCR1);
	writel(0x00000004, phy_base + PHY_DCURR);
	writel(0x00000001, phy_base + PHY_DCURR);
	while (!(readl(phy_base + PHY_DCUSR0) & BIT(0)))
		cpu_relax();
	writel(0x0200C6A0, phy_base + PHY_PGCR1);

	/* End of 2nd DCU run */
	writel(PHY_PIR_WREYE | PHY_PIR_INIT, phy_base + PHY_PIR);
	while (!(readl(phy_base + PHY_PGSR0) & PHY_PGSR0_IDONE))
		cpu_relax();
}

static void ddrphy_shift_rof_hws(void __iomem *phy_base, const int *pos_shift,
			const int *neg_shift, unsigned int freq)
{
	int dx, rankrid, rankwid;
	int rdqsd, rdqnsd;

	ddrphy_vt_ctrl(phy_base, 0);

	for (dx = 0; dx < 4; dx++) {
		rankrid = dx / 2;
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
	}

	ddrphy_vt_ctrl(phy_base, 1);
}

/* UMC */
static const u32 umc_cmdctla[DRAM_FREQ_NR] = {0x77EE1722};
static const u32 umc_cmdctlb[DRAM_FREQ_NR] = {0x18F88C44};

static const u32 umc_spcctla[DRAM_FREQ_NR][DRAM_SZ_NR] = {
	/*  256MB       512MB      1G  */
	{0x0055081E, 0x0089081E, 0x00B9081E}	/* 2133 MHz */
};
static const u32 umc_spcctlb[DRAM_FREQ_NR] = {0x00FF000B};
static const u32 umc_zqcsctlb[DRAM_FREQ_NR] = {0x0000002B};
static const u32 umc_zqcsctlc[DRAM_FREQ_NR] = {0x0003E99B};
static const u32 umc_rdatactl[DRAM_FREQ_NR] = {0x000A0000};
static const u32 umc_wdatactl[DRAM_FREQ_NR] = {0x04020800};
static const u32 umc_dataset[DRAM_FREQ_NR]  = {0x00000000};
static const u32 umc_flowctla[DRAM_FREQ_NR] = {0x0401E026};

/* check if recovery from power supply cut off */
static int umc_is_instant_boot(void __iomem *dc_base)
{
	return !!(readl(dc_base + UMC_SPCSTATC) & BIT(0));
}

static void umc_init_set(void __iomem *dc_base)
{
	/* initialize UMC internal state */
	writel(0x00000002, dc_base + UMC_INITSET);

	/* wait for init completion */
	while ((readl(dc_base + UMC_INITSTAT) & BIT(1)))
		cpu_relax();
}

static void umc_stop_retention(void __iomem *umc_base)
{
	void __iomem *dc_base = umc_base + 0x00011000;

	writel(0x80100002, dc_base + UMC_SPCSETC);
}

static int umc_ch_init(void __iomem *umc_ch_base, void __iomem *phy_base,
		       unsigned int freq, unsigned long size, int ch,
		       int resume)
{
	void __iomem *dc_base = umc_ch_base + 0x00011000;
	enum dram_freq freq_e;
	enum dram_size size_e;

	switch (freq) {
	case 2133:
		freq_e = DRAM_FREQ_2133M;
		break;
	default:
		pr_err("unsupported DRAM frequency %ud MHz\n", freq);
		return -EINVAL;
	}

	switch (size) {
	case 0:
		return 0;
	case SZ_256M:
		size_e = DRAM_SZ_256M;
		break;
	case SZ_512M:
		size_e = DRAM_SZ_512M;
		break;
	case SZ_1G:
		size_e = DRAM_SZ_1G;
		break;
	default:
		pr_err("unsupported DRAM size 0x%08lx (per 16bit) for ch%d\n",
		       size, ch);
		return -EINVAL;
	}

	if (resume) {
		/* LP_REQ: SW CONTROL MODE */
		writel(0x8c000000, dc_base + UMC_DFILPCTLA);

		/* DEL LP_REQ -> polling */
		while ((readl(dc_base + UMC_DFILPCTLA) & 0x3))
			cpu_relax();

		ddrphy_zqcalib(phy_base);

		/* set LP_REQ  */
		writel(0x8c000300, dc_base + UMC_DFILPCTLA);
		while (((readl(dc_base + UMC_DFILPCTLA) & 0x3) != 0x3))
			cpu_relax();

		/* LP_REQ: HW CONTROL MODE */
		writel(0x0c000000, dc_base + UMC_DFILPCTLA);
	}

	/* set AC Spec for Write/Read */
	writel(umc_cmdctla[freq_e], dc_base + UMC_CMDCTLA);
	writel(umc_cmdctlb[freq_e], dc_base + UMC_CMDCTLB);

	/* set AC Spec for SPC Com(Ref/PWDN/SLFR) */
	writel(umc_spcctla[freq_e][size_e], dc_base + UMC_SPCCTLA);
	writel(umc_spcctlb[freq_e], dc_base + UMC_SPCCTLB);

	/* DFI I/F Timing Setting */
	writel(umc_rdatactl[freq_e], dc_base + UMC_RDATACTL_D0);
	writel(umc_rdatactl[freq_e], dc_base + UMC_RDATACTL_D1);
	writel(umc_wdatactl[freq_e], dc_base + UMC_WDATACTL_D0);
	writel(umc_wdatactl[freq_e], dc_base + UMC_WDATACTL_D1);
	writel(umc_dataset[freq_e],  dc_base + UMC_DATASET);

	/* DC Clock Gating Mode */
	writel(0x00400020, dc_base + UMC_DCCGCTL);

	/* DCB Control Setting */
	writel(0x00000084, dc_base + UMC_FLOWCTLG);

	/* SPC Command Slot Setting */
	writel(0x00000000, dc_base + UMC_ACSSETA);

	/* DCF Control Setting */
	writel(umc_flowctla[freq_e], dc_base + UMC_FLOWCTLA);
	writel(0x00202000, dc_base + UMC_FLOWCTLB);
	writel(0x00004444, dc_base + UMC_FLOWCTLC);

	/* ZQ Calibration Short Control Start */
	writel(umc_zqcsctlb[freq_e], dc_base + UMC_ZQCSCTLB);
	writel(umc_zqcsctlc[freq_e], dc_base + UMC_ZQCSCTLC);
	writel(0x00000000, dc_base + UMC_ZQCSCTLA);

	/* Refresh Control Start (Controlled by Arbiter) */
	writel(0x2A0A0A00, dc_base + UMC_SPCSETB);

	/* DFI update Setting(ctrl updata) */
	writel(0x00000520, dc_base + UMC_DFICUPDCTLA);

	/* OpenBank Config Setting */
	writel(0xFDBFFFFF, dc_base + UMC_FLOWCTLOB0);
	writel(0xFFFFFFFF, dc_base + UMC_FLOWCTLOB1);
	/* temporary Closed Bank Setting */
	writel(0x00000001, dc_base + UMC_FLOWCTLOB3);
	/* OpenBank Fetch Setting (Fetch Size Max = 1024Byte / Depth = 8) */
	writel(0x0020FF88, dc_base + UMC_FLOWCTLOB4);

	/* DRAM Mapping Setting (Z or Horizonal) */
	writel(0x00080700, dc_base + UMC_BSICMAPSET);

	/* Interrupt Mask Disable */
	writel(0x00000000, dc_base + UMC_ERRMASKA);
	writel(0x00000000, dc_base + UMC_ERRMASKB);

	/* BitPerPixel Mode Setting */
	writel(0x00000003, umc_ch_base + BITPERPIXELMODE_D0);

	return 0;
}

static void um_init(void __iomem *um_base)
{
	writel(0x000000ff, um_base + UMC_MBUS0);
	writel(0x000000ff, um_base + UMC_MBUS1);
	writel(0x000000ff, um_base + UMC_MBUS2);
	writel(0x000000ff, um_base + UMC_MBUS3);
	writel(0x000000ff, um_base + UMC_MBUS4);
}

#define UM_BASE			(IOMEM(0x5b600000))
#define UMC_BASE(ch)		(IOMEM(0x5b800000) + 0x00200000 * (ch))
#define UMC_DC_BASE(ch)		(UMC_BASE(ch) + 0x00011000)
#define PHY_BASE(ch)		(IOMEM(0x6e200000) + 0x00004000 * (ch))

static int pxs3_umc_is_resuming(void)
{
	int resume, ch;

	resume = umc_is_instant_boot(UMC_DC_BASE(0));

	for (ch = 1; ch < DRAM_CH_NR; ch++) {
		if (resume != umc_is_instant_boot(UMC_DC_BASE(ch))) {
			pr_err("mismatch for resume among channels\n");
			return -EINVAL;
		}
	}

	return resume;
}

int pxs3_umc_init(const struct board_data *bd)
{
	enum dram_board board;
	int ch, ret;
	int resume;

	switch (BD_BOARD_GET_TYPE(bd->flags)) {
	case BD_BOARD_PXS3_REF:
		board = DRAM_BOARD_PXS3_REF;
		break;
	default:
		pr_err("unsupported board type %d\n",
		       BD_BOARD_GET_TYPE(bd->flags));
		return -EINVAL;
	}

	resume = pxs3_umc_is_resuming();
	if (resume < 0)
		return resume;

	/* DDRPHY initialization */
	for (ch = 0; ch < DRAM_CH_NR; ch++) {
		/* Initial setting  */
		if (!resume)
			umc_init_set(UMC_DC_BASE(ch));

		ddrphy_ch_init(PHY_BASE(ch), resume);
	}

	/* control retention */
	if (resume) {
		for (ch = 0; ch < DRAM_CH_NR; ch++)
			umc_stop_retention(UMC_BASE(ch));
	}

	for (ch = 0; ch < DRAM_CH_NR; ch++) {
		unsigned long size = bd->dram_ch[ch].size;
		unsigned int width = bd->dram_ch[ch].width;

		ret = umc_ch_init(UMC_BASE(ch), PHY_BASE(ch), bd->dram_freq,
				  size / (width / 16), ch, resume);
		if (ret) {
			pr_err("failed to initialize UMC ch%d\n", ch);
			return ret;
		}
	}

	/* MBUS */
	um_init(UM_BASE);

	/* DDRPHY CKOFFSET */
	for (ch = 0; ch < DRAM_CH_NR; ch++)
		ddrphy_set_ckoffset_qoffset(ddrphy_ckoffset[board][ch], 0,
					    bd->dram_freq, PHY_BASE(ch));

	/* DDRPHY Training */
	for (ch = 0; ch < DRAM_CH_NR; ch++) {

		/* dummy read to exit self-refresh */
		readl(IOMEM(bd->dram_ch[ch].base));	/* rank0 */
		readl(IOMEM(bd->dram_ch[ch].base + 8));	/* rank1 */

		ddrphy_training(PHY_BASE(ch));
	}

	for (ch = 0; ch < DRAM_CH_NR; ch++) {
		/* DIO roffset shift after training */
		ddrphy_shift_rof_hws(PHY_BASE(ch),
				     ddrphy_rof_pos_shift[board][ch],
				     ddrphy_rof_neg_shift[board][ch],
				     bd->dram_freq);

		writel(0xF1AA0048, PHY_BASE(ch) + PHY_PGCR3);
	}

	return 0;
}
