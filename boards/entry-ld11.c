/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <compiler.h>
#include <entry.h>
#include <init.h>
#include <io.h>

#define SC_CPU_GEAR_BASE	(IOMEM(0x61848000))
#define SC_CA53_GEAR_SET	((SC_CPU_GEAR_BASE) + 0x84)
#define SC_CA53_GEAR_UPD	((SC_CPU_GEAR_BASE) + 0x88)

static int ld11_soc_init(const struct board_data *bd)
{
	writel(0, SC_CA53_GEAR_SET);	/* Gear0: CPLL/2 */
	writel(1, SC_CA53_GEAR_UPD);	/* update */

	return 0;
}

static const struct soc_data ld11_data = {
	.soc_id = 0x31,
	.uart_clk_bits = 0x80,
	.uart_clk_rate = 58823529,
	.uart_pinmux = {
		{ .pin = 54, .mux = 0, },
		{ .pin = 58, .mux = 1, },
		{ .pin = 90, .mux = 1, },
		{ .pin = 94, .mux = 1, },
	},
	.dram_rst_bits = 0x00000003,
	.dram_clk_bits = 0x00000003,
	.dram_default_freq = 1600,
	.dram_default_width = { 16, 16, },
	.dram_have_ch2 = 0,
	.dpll = { 6, -1, -1, },
	.umc_init = ld11_umc_init,
	.soc_init = ld11_soc_init,
};

static void __noreturn ld11_init(const struct board_data *bd)
{
	main(&ld11_data, bd);
}

static const struct board_data ld11_ref_data = {
	.board_name = "LD11 Reference",
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x20000000,
	},
	.dram_ch[1] = {
		.size = 0x20000000,
	},
};

ENTRY(ld11_ref)
{
	ld11_init(&ld11_ref_data);
}

static const struct board_data ld11_global_data = {
	.board_name = "LD11 Global",
	.uart_port = 0,
	.dram_ch[0] = {
		.size = 0x20000000,
	},
	.dram_ch[1] = {
		.size = 0x20000000,
	},
};

ENTRY(ld11_global)
{
	ld11_init(&ld11_global_data);
}
