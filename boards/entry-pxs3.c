// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <entry.h>
#include <init.h>

static const struct soc_data pxs3_data = {
	.soc_id = 0x35,
	.uart_clk_bits = 0x800,
	.uart_clk_rate = 58823529,
	.uart_pinmux = {
		{ .pin = 92, .mux = 0, },
		{ .pin = 94, .mux = 0, },
		{ .pin = 96, .mux = 0, },
		{ .pin = 98, .mux = 0, },
	},
	.heap_start = 0x30016000,
	.heap_end = 0x30020000,
	.timer_clk_rate = 50000000,
	.dram_rst_bits = 0x07010707,
	.dram_clk_bits = 0x00010007,
	.dram_default_freq = 2133,
	.dram_default_width = { 32, 32, 32, },
	.dram_have_ch2 = 1,
	.dpll = { 8, 9, 10, },
	.umc_init = pxs3_umc_init,
};

static void __noreturn pxs3_init(const struct board_data *bd)
{
	main(&pxs3_data, bd);
}

static const struct board_data pxs3_ref_data = {
	.board_name = "PXs3 Reference",
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

ENTRY(pxs3_ref)
{
	pxs3_init(&pxs3_ref_data);
}
