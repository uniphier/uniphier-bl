/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2020 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __SOC_DATA_H__
#define __SOC_DATA_H__

#include <dram-data.h>

struct board_data;

#define MAX_NR_UART_PORTS	4

#define CLK_UART		0
#define CLK_DRAM		1
#define MAX_NR_CLKS		2

#define RST_DRAM		0
#define MAX_NR_RSTS		1

struct pinmux {
	unsigned int pin;
	unsigned int mux;
};

struct soc_data {
	unsigned int soc_id;
	void __iomem *cntctrl_base;
	void __iomem *socglue_base;
	void __iomem *sysctrl_base;
	void __iomem *uart_base;
	unsigned int uart_clk_rate;
	struct pinmux uart_pinmux[MAX_NR_UART_PORTS];
	struct {
		unsigned int reg;
		unsigned int mask;
	} clks[MAX_NR_CLKS];
	struct {
		unsigned int reg;
		unsigned int mask;
	} rsts[MAX_NR_RSTS];
	unsigned long stack_base;
	unsigned int timer_clk_rate;
	unsigned long dram_base;
	unsigned long dram_ch1_sparse_base;
	unsigned int dram_default_freq;
	unsigned int dram_default_width[MAX_NR_DRAM_CH];
	int dpll[MAX_NR_DRAM_CH];
	int (*memconf_init)(const struct board_data *); /* MEMCONF register */
	int (*umc_init)(const struct board_data *); /* DRAM controller init */
	int (*soc_init)(const struct board_data *); /* SoC-specific init */
};

#endif /* __SOC_DATA_H__ */
