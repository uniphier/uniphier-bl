// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <clk.h>
#include <io.h>
#include <soc-data.h>

#define SC_RSTCTRL_BASE		(IOMEM(0x61842000))
#define SC_CLKCTRL_BASE		(IOMEM(0x61842100))

static void regmap_update(void __iomem *base, const struct regmap *regmap)
{
	void __iomem *reg;
	u32 tmp;

	reg = base + regmap->reg;

	tmp = readl(reg);
	tmp |= regmap->mask;
	writel(tmp, reg);
}

void clk_enable(const struct regmap *regmap)
{
	regmap_update(SC_CLKCTRL_BASE, regmap);
}

void rst_deassert(const struct regmap *regmap)
{
	regmap_update(SC_RSTCTRL_BASE, regmap);
}
