/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <init.h>
#include <io.h>
#include <utils.h>

#define SC_PLLCTRL_BASE		(IOMEM(0x61841400))
#define   SC_PLLCTRL2_NRSTDS		BIT(28)

#define SC_RSTCTRL_BASE		(IOMEM(0x61842000))
#define SC_RSTCTRL7		((SC_RSTCTRL_BASE) + 0x18)

#define SC_CLKCTRL_BASE		(IOMEM(0x61842100))
#define SC_CLKCTRL4		((SC_CLKCTRL_BASE) + 0x0c)
#define SC_CLKCTRL7		((SC_CLKCTRL_BASE) + 0x18)

void dpll_init(const int *dpll)
{
	void __iomem *reg;
	u32 tmp;
	int i;

	for (i = 0; i < MAX_NR_DRAM_CH; i++) {
		if (dpll[i] < 0)
			break;

		/* DPLLCTRL2 */
		reg = SC_PLLCTRL_BASE + 0x10 * dpll[i] + 4;

		tmp = readl(reg);
		tmp |= SC_PLLCTRL2_NRSTDS;
		writel(tmp, reg);
	}
}

void clk_enable_uart(unsigned int clk_bits)
{
	u32 tmp;

	tmp = readl(SC_CLKCTRL4);
	tmp |= clk_bits;
	writel(tmp, SC_CLKCTRL4);
}

void clk_enable_dram(unsigned int clk_bits, unsigned int rst_bits)
{
	u32 tmp;

	tmp = readl(SC_RSTCTRL7);
	tmp |= rst_bits;
	writel(tmp, SC_RSTCTRL7);

	tmp = readl(SC_CLKCTRL7);
	tmp |= clk_bits;
	writel(tmp, SC_CLKCTRL7);
}
