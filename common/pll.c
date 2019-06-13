// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <bitfield.h>
#include <bits.h>
#include <board-data.h>
#include <delay.h>
#include <io.h>
#include <pll.h>
#include <soc-data.h>
#include <utils.h>

#define SC_PLLCTRL_BASE		0x1400
#define   SC_PLLCTRL_SSC_DK_MASK	GENMASK(14, 0)
#define   SC_PLLCTRL2_NRSTDS		BIT(28)
#define   SC_PLLCTRL2_SSC_JK_MASK	GENMASK(26, 0)

/* SSC rate = 1% is a reasonable default */
#define SSC_RATE_DEFAULT	1

static void pll_set_ssc_mod_val(const struct soc_data *sd, int pll_id,
				unsigned int freq, unsigned int ssc_rate,
				unsigned int divn)
{
	void __iomem *reg;
	u32 tmp;

	reg = sd->sysctrl_base + SC_PLLCTRL_BASE + 0x10 * pll_id;

	tmp = readl(reg);		/* SSCPLLCTRL */
	tmp &= ~SC_PLLCTRL_SSC_DK_MASK;
	tmp |= FIELD_PREP(SC_PLLCTRL_SSC_DK_MASK,
			  DIV_ROUND_CLOSEST(487UL * freq * ssc_rate,
					    divn * 512));
	writel(tmp, reg);

	tmp = readl(reg + 4);		/* SSCPLLCTRL2 */
	tmp &= ~SC_PLLCTRL2_SSC_JK_MASK;
	tmp |= FIELD_PREP(SC_PLLCTRL2_SSC_JK_MASK,
			  DIV_ROUND_CLOSEST(21431887UL * freq, divn * 512));
	writel(tmp, reg + 4);
}

static void pll_enable_ssc_mod(const struct soc_data *sd, int pll_id)
{
	void __iomem *reg;
	u32 tmp;

	reg = sd->sysctrl_base + SC_PLLCTRL_BASE + 0x10 * pll_id + 4;

	tmp = readl(reg);		/* SSCPLLCTRL2 */
	tmp |= SC_PLLCTRL2_NRSTDS;
	writel(tmp, reg);
}

void dpll_init(const struct board_data *bd)
{
	const struct soc_data *sd = bd->soc_data;
	unsigned int ssc_rate;
	int pll_id, i;

	/* use the default register values unless we need to change them */
	if (sd->dram_default_freq != bd->dram_freq ||
	    bd->flags & BD_DPLL_SSC_CHANGE_RATE) {

		ssc_rate = (bd->flags & BD_DPLL_SSC_CHANGE_RATE) ?
			BD_DPLL_SSC_GET_RATE(bd->flags) : SSC_RATE_DEFAULT;

		for (i = 0; i < MAX_NR_DRAM_CH; i++) {
			pll_id = sd->dpll[i];
			if (pll_id < 0)
				break;

			pll_set_ssc_mod_val(sd, pll_id, bd->dram_freq,
					    ssc_rate, 2);
		}
		udelay(50);
	}

	for (i = 0; i < MAX_NR_DRAM_CH; i++) {
		pll_id = sd->dpll[i];

		if (pll_id < 0)
			break;

		pll_enable_ssc_mod(sd, pll_id);
	}
}

void pll_set_freq(const struct soc_data *sd, int pll_id,
		  unsigned int freq, unsigned int divn)
{
	pll_set_ssc_mod_val(sd, pll_id, freq, SSC_RATE_DEFAULT, divn);
	udelay(50);
	pll_enable_ssc_mod(sd, pll_id);
}
