// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <board-data.h>
#include <entry.h>
#include <io.h>
#include <soc-data.h>
#include <umc.h>

#define SC_PVTCTRL_BASE		(IOMEM(0x6184e000))

static int ld20_soc_init(const struct board_data *bd)
{
	/* increase the VDD09 voltage line up to 0.93V to suppress jitter */
	writel(0x00000002, SC_PVTCTRL_BASE + 0x4);
	writel(0x0000001e, SC_PVTCTRL_BASE + 0x78);
	writel(0x00000001, SC_PVTCTRL_BASE + 0x0);

	return 0;
}

static const struct soc_data ld20_data = {
	.soc_id = 0x32,
	.uart_clk_rate = 58823529,
	.uart_clk_regmap = { .reg = 0x0c, .mask = 0x00000080 },
	.uart_pinmux = {
		{ .pin = 54, .mux = 0 },
		{ .pin = 58, .mux = 1 },
		{ .pin = 90, .mux = 1 },
		{ .pin = 94, .mux = 1 },
	},
	.stack_base = 0x3001c000,
	.timer_clk_rate = 50000000,
	.dram_rst_regmap = { .reg = 0x18, .mask = 0x00010707 },
	.dram_clk_regmap = { .reg = 0x18, .mask = 0x00010007 },
	.dram_default_freq = 1866,
	.dram_default_width = { 32, 32, 32 },
	.dram_have_ch2 = 1,
	.dpll = { 6, 7, 8 },
	.umc_init = ld20_umc_init,
	.soc_init = ld20_soc_init,
};

static const struct board_data ld20_ref_data = {
	.board_name = "LD20 Reference",
	.soc_data = &ld20_data,
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
BOARD_DECLARE(ld20_ref, ld20_ref_data);

static const struct board_data ld20_global_data = {
	.board_name = "LD20 Global",
	.soc_data = &ld20_data,
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
BOARD_DECLARE(ld20_global, ld20_global_data);

static const struct board_data ld21_ref_data = {
	.board_name = "LD21 Reference",
	.soc_data = &ld20_data,
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x20000000,
	},
	.dram_ch[1] = {
		.size = 0x40000000,
	},
	.flags = BD_DRAM_SPARSE | BD_BOARD_LD21_REF,
};
BOARD_DECLARE(ld21_ref, ld21_ref_data);

static const struct board_data ld21_global_data = {
	.board_name = "LD21 Global",
	.soc_data = &ld20_data,
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x20000000,
	},
	.dram_ch[1] = {
		.size = 0x40000000,
	},
	.flags = BD_DRAM_SPARSE | BD_BOARD_LD21_GLOBAL,
};
BOARD_DECLARE(ld21_global, ld21_global_data);
