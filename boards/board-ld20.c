// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <board-data.h>
#include <entry.h>
#include <memconf.h>
#include <io.h>
#include <soc-data.h>
#include <umc.h>

#define SC_PVTCTRL_BASE		0xe000

static int ld20_soc_init(const struct board_data *bd)
{
	const struct soc_data *sd = bd->soc_data;
	void __iomem *pvtctrl_base = sd->sysctrl_base + SC_PVTCTRL_BASE;

	/* increase the VDD09 voltage line up to 0.93V to suppress jitter */
	writel(0x00000002, pvtctrl_base + 0x4);
	writel(0x0000001e, pvtctrl_base + 0x78);
	writel(0x00000001, pvtctrl_base + 0x0);

	return 0;
}

static const struct soc_data ld20_data = {
	.soc_id = 0x32,
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
		[CLK_DRAM] = { .reg = 0x18, .mask = 0x00010007 },
	},
	.rsts = {
		[RST_DRAM] = { .reg = 0x18, .mask = 0x00010707 },
	},
	.stack_base = 0x3001c000,
	.timer_clk_rate = 50000000,
	.dram_base = 0x80000000,
	.dram_ch1_sparse_base = 0xc0000000,
	.dram_default_freq = 1866,
	.dram_default_width = { 32, 32, 32 },
	.dpll = { 6, 7, 8 },
	.memconf_init = memconf_3ch_init,
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
