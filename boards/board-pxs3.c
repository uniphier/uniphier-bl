// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <board-data.h>
#include <entry.h>
#include <io.h>
#include <memconf.h>
#include <soc-data.h>
#include <umc.h>

static const struct soc_data pxs3_data = {
	.soc_id = 0x35,
	.cntctrl_base = IOMEM(0x60e00000),
	.socglue_base = IOMEM(0x5f800000),
	.sysctrl_base = IOMEM(0x61840000),
	.uart_base = IOMEM(0x54006800),
	.uart_clk_rate = 58823529,
	.uart_clk_regmap = { .reg = 0x0c, .mask = 0x00000800 },
	.uart_pinmux = {
		{ .pin = 92, .mux = 0 },
		{ .pin = 94, .mux = 0 },
		{ .pin = 96, .mux = 0 },
		{ .pin = 98, .mux = 0 },
	},
	.stack_base = 0x30020000,
	.timer_clk_rate = 50000000,
	.dram_base = 0x80000000,
	.dram_ch1_sparse_base = 0xc0000000,
	.dram_rst_regmap = { .reg = 0x18, .mask = 0x07010707 },
	.dram_clk_regmap = { .reg = 0x18, .mask = 0x00010007 },
	.dram_default_freq = 2133,
	.dram_default_width = { 32, 32, 32 },
	.dpll = { 8, 9, 10 },
	.memconf_init = memconf_3ch_init,
	.umc_init = pxs3_umc_init,
};

static const struct board_data pxs3_ref_data = {
	.board_name = "PXs3 Reference",
	.soc_data = &pxs3_data,
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x40000000,
	},
	.dram_ch[1] = {
		.size = 0x20000000,
	},
	.dram_ch[2] = {
		.size = 0x40000000,
	},
	.flags = BD_BOARD_PXS3_REF,
};
BOARD_DECLARE(pxs3_ref, pxs3_ref_data);
