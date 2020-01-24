// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <board-data.h>
#include <entry.h>
#include <io.h>
#include <memconf.h>
#include <soc-data.h>
#include <sysctrl.h>
#include <umc.h>

#define SC_CA53_GEAR_SET	0x8084
#define SC_CA53_GEAR_UPD	0x8088

static int ld11_soc_init(const struct board_data *bd)
{
	const struct soc_data *sd = bd->soc_data;

	pll_set_freq(sd, 0, 1960, 2);		/* CPLL: 2000MHz -> 1960MHz */
	writel(0, sd->sysctrl_base + SC_CA53_GEAR_SET);	/* Gear0: CPLL/2 */
	writel(1, sd->sysctrl_base + SC_CA53_GEAR_UPD);	/* update */

	return 0;
}

static const struct soc_data ld11_data = {
	.soc_id = 0x31,
	.cntctrl_base = IOMEM(0x60e00000),
	.socglue_base = IOMEM(0x5f800000),
	.sysctrl_base = IOMEM(0x61840000),
	.uart_base = IOMEM(0x54006800),
	.uart_clk_rate = 58823529,
	.uart_pinmux = {
		{ .pin = 54, .mux = 0 },
		{ .pin = 58, .mux = 1 },
		{ .pin = 90, .mux = 1 },
		{ .pin = 94, .mux = 1 },
	},
	.clks = {
		[CLK_UART] = { .reg = 0x0c, .mask = 0x00000080 },
		[CLK_DRAM] = { .reg = 0x18, .mask = 0x00000003 },
	},
	.rsts = {
		[RST_DRAM] = { .reg = 0x18, .mask = 0x00000003 },
	},
	.stack_base = 0x30014c00,
	.timer_clk_rate = 50000000,
	.dram_base = 0x80000000,
	.dram_ch1_sparse_base = 0xc0000000,
	.dram_default_freq = 1600,
	.dram_default_width = { 16, 16 },
	.dpll = { 6, -1, -1 },
	.memconf_init = memconf_2ch_init,
	.umc_init = ld11_umc_init,
	.soc_init = ld11_soc_init,
};

static const struct board_data ld11_ref_data = {
	.board_name = "LD11 Reference",
	.soc_data = &ld11_data,
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x20000000,
	},
	.dram_ch[1] = {
		.size = 0x20000000,
	},
};
BOARD_DECLARE(ld11_ref, ld11_ref_data);

static const struct board_data ld11_global_data = {
	.board_name = "LD11 Global",
	.soc_data = &ld11_data,
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x20000000,
	},
	.dram_ch[1] = {
		.size = 0x20000000,
	},
};
BOARD_DECLARE(ld11_global, ld11_global_data);
