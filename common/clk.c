// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <clk.h>
#include <io.h>
#include <soc-data.h>

#define SC_RSTCTRL_BASE		0x2000
#define SC_CLKCTRL_BASE		0x2100

static void regmap_update(void __iomem *base, const struct regmap *regmap)
{
	void __iomem *reg;
	u32 tmp;

	reg = base + regmap->reg;

	tmp = readl(reg);
	tmp |= regmap->mask;
	writel(tmp, reg);
}

void clk_enable(const struct soc_data *sd, unsigned int id)
{
	BUG_ON(id >= ARRAY_SIZE(sd->clks));
	regmap_update(sd->sysctrl_base + SC_CLKCTRL_BASE, regmap);
}

void rst_deassert(const struct soc_data *sd, const struct regmap *regmap)
{
	BUG_ON(id >= ARRAY_SIZE(sd->rsts));
	regmap_update(sd->sysctrl_base + SC_RSTCTRL_BASE, regmap);
}
