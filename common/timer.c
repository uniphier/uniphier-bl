// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <bug.h>
#include <delay.h>
#include <bits.h>
#include <errno.h>
#include <io.h>
#include <sysreg.h>
#include <timer.h>
#include <types.h>
#include <utils.h>

#define CNTCR			(IOMEM(0x60e00000))
#define   CNTCR_EN			BIT(0)

static unsigned int timer_clk_rate;

int timer_init(unsigned int clk_rate)
{
	u32 tmp;

	if (WARN_ON(!clk_rate))
		return -EINVAL;

	tmp = readl(CNTCR);
	tmp |= CNTCR_EN;
	writel(tmp, CNTCR);

	write_sysreg(clk_rate, cntfrq_el0);

	timer_clk_rate = clk_rate;

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
