// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <bug.h>
#include <bits.h>
#include <delay.h>
#include <errno.h>
#include <io.h>
#include <sysreg.h>
#include <soc-data.h>
#include <timer.h>
#include <types.h>
#include <utils.h>

#define CNTCR			0
#define   CNTCR_EN			BIT(0)

static unsigned int timer_clk_rate;

int timer_init(const struct soc_data *sd)
{
	void __iomem *reg;
	u32 tmp;

	timer_clk_rate = sd->timer_clk_rate;

	if (WARN_ON(!timer_clk_rate))
		return -EINVAL;

	reg = sd->cntctrl_base + CNTCR;
	tmp = readl(reg);
	tmp |= CNTCR_EN;
	writel(tmp, reg);

	write_sysreg(timer_clk_rate, cntfrq_el0);

	return 0;
}

static u64 timer_cnt_get(void)
{
	return read_sysreg(cntpct_el0);
}

void udelay(unsigned long usecs)
{
	u64 cnt;

	cnt = timer_cnt_get();

	cnt += (u64)usecs * timer_clk_rate / 1000000;

	while (timer_cnt_get() < cnt)
		cpu_relax();
}
