// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2019 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <board-data.h>
#include <entry.h>
#include <io.h>
#include <memconf.h>
#include <soc-data.h>
#include <umc.h>

static const struct soc_data nx1_data = {
	.soc_id = 0x37,
	.cntctrl_base = IOMEM(0x10e00000),
	.socglue_base = IOMEM(0x1f800000),
	.sysctrl_base = IOMEM(0x11840000),
	.uart_base = IOMEM(0x14006800),
	.uart_clk_rate = 133333333,
	.uart_clk_regmap = { .reg = 0x08, .mask = 0x00010000 },
	.uart_pinmux = {
		{ .pin = 27, .mux = 0 },
		{ .pin = 29, .mux = 0 },
		{ .pin = 36, .mux = 0 },
		{ .pin = 40, .mux = 0 },
	},
	.stack_base = 0x04014000,
	.timer_clk_rate = 50000000,
	.dram_base = 0x20000000,
	.dram_rst_regmap = { .reg = 0x18, .mask = 0x01010001 },
	.dram_clk_regmap = { .reg = 0x18, .mask = 0x00010001 },
	.dram_default_freq = 2666,
	.dram_default_width = { 32 },
	.dpll = { 4, -1, -1, },
	.memconf_init = memconf_dummy_init,
	.umc_init = nx1_umc_init,
};

static const struct board_data nx1_ref_data = {
	.board_name = "NX1 Reference",
	.soc_data = &nx1_data,
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x100000000,
	},
};
BOARD_DECLARE(nx1_ref, nx1_ref_data);
