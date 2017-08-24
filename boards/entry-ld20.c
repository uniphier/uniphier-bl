/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <compiler.h>
#include <entry.h>
#include <init.h>

static const struct soc_data ld20_data = {
	.soc_id = 0x32,
	.uart_clk_bits = 0x80,
	.uart_clk_rate = 58823529,
	.uart_pinmux = {
		{ .pin = 54, .mux = 0, },
		{ .pin = 58, .mux = 1, },
		{ .pin = 90, .mux = 1, },
		{ .pin = 94, .mux = 1, },
	},
	.dram_rst_bits = 0x00010707,
	.dram_clk_bits = 0x00010007,
	.dram_default_freq = 1866,
	.dram_default_width = { 32, 32, 32, },
	.dram_have_ch2 = 1,
	.dpll = { 6, 7, 8, },
	.umc_init = ld20_umc_init,
};

static void __noreturn ld20_init(const struct board_data *bd)
{
	main(&ld20_data, bd);
}

static const struct board_data ld20_ref_data = {
	.board_name = "LD20 Reference",
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x40000000,
	},
	.dram_ch[1] = {
		.size = 0x40000000,
	},
	.dram_ch[2] = {
		.size = 0x40000000,
	},
	.flags = BD_BOARD_LD20_REF,
};

ENTRY(ld20_ref)
{
	ld20_init(&ld20_ref_data);
}

static const struct board_data ld20_global_data = {
	.board_name = "LD20 Global",
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x40000000,
	},
	.dram_ch[1] = {
		.size = 0x40000000,
	},
	.dram_ch[2] = {
		.size = 0x40000000,
	},
	.flags = BD_BOARD_LD20_GLOBAL,
};

ENTRY(ld20_global)
{
	ld20_init(&ld20_global_data);
}

static const struct board_data ld21_ref_data = {
	.board_name = "LD21 Reference",
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x20000000,
	},
	.dram_ch[1] = {
		.size = 0x40000000,
	},
	.flags = BD_DRAM_SPARSE | BD_BOARD_LD21_REF,
};

ENTRY(ld21_ref)
{
	ld20_init(&ld21_ref_data);
}

static const struct board_data ld21_global_data = {
	.board_name = "LD21 Global",
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x20000000,
	},
	.dram_ch[1] = {
		.size = 0x40000000,
	},
	.flags = BD_DRAM_SPARSE | BD_BOARD_LD21_GLOBAL,
};

ENTRY(ld21_global)
{
	ld20_init(&ld21_global_data);
}
