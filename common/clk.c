// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <bitops.h>
#include <init.h>
#include <io.h>

#define SC_RSTCTRL_BASE		(IOMEM(0x61842000))
#define SC_RSTCTRL7		((SC_RSTCTRL_BASE) + 0x18)

#define SC_CLKCTRL_BASE		(IOMEM(0x61842100))
#define SC_CLKCTRL4		((SC_CLKCTRL_BASE) + 0x0c)
#define SC_CLKCTRL7		((SC_CLKCTRL_BASE) + 0x18)

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
