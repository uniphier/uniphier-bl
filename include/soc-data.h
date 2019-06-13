/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __SOC_DATA_H__
#define __SOC_DATA_H__

#include <dram-data.h>

struct board_data;

#define MAX_NR_UART_PORTS	4

struct regmap {
	unsigned int reg;
	unsigned int mask;
};

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
	struct regmap uart_clk_regmap;
	struct pinmux uart_pinmux[MAX_NR_UART_PORTS];
	unsigned long stack_base;
	unsigned int timer_clk_rate;
	unsigned long dram_base;
	unsigned long dram_ch1_sparse_base;
	struct regmap dram_rst_regmap;
	struct regmap dram_clk_regmap;
	unsigned int dram_default_freq;
	unsigned int dram_default_width[MAX_NR_DRAM_CH];
	int dpll[MAX_NR_DRAM_CH];
	int (*memconf_init)(const struct board_data *); /* MEMCONF register */
	int (*umc_init)(const struct board_data *); /* DRAM controller init */
	int (*soc_init)(const struct board_data *); /* SoC-specific init */
};

#endif /* __SOC_DATA_H__ */
