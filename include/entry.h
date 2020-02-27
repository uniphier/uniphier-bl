/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __ENTRY_H__
#define __ENTRY_H__

#include <board-data.h>
#include <build_bug.h>
#include <compiler.h>
#include <soc-data.h>

#define BOARD_DECLARE(name, bd)					\
	/* declaration to suppress "Should it be static?" warning of sparse */ \
	void __always_unused __check_##name(void);			\
	void __always_unused __check_##name(void) {			\
		const struct soc_data *sd = bd.soc_data;		\
									\
		BUILD_BUG_ON(!bd.board_name);				\
		BUILD_BUG_ON(bd.uart_port >= MAX_NR_UART_PORTS);	\
		BUILD_BUG_ON(!sd);					\
		BUILD_BUG_ON(!sd->soc_id);				\
		BUILD_BUG_ON(!sd->cntctrl_base);			\
		BUILD_BUG_ON(!sd->socglue_base);			\
		BUILD_BUG_ON(!sd->sysctrl_base);			\
		BUILD_BUG_ON(!sd->uart_base);				\
		BUILD_BUG_ON(!sd->uart_clk_rate);			\
		BUILD_BUG_ON(!sd->stack_base);				\
		BUILD_BUG_ON(!sd->timer_clk_rate);			\
		BUILD_BUG_ON(!sd->dram_base);				\
		BUILD_BUG_ON(!sd->dram_default_freq);			\
		BUILD_BUG_ON(!sd->memconf_init);			\
		BUILD_BUG_ON(!sd->umc_init);				\
	}								\
	/* declaration to suppress "Should it be static?" warning of sparse */ \
	extern const struct board_data *entry_##name;			\
	const struct board_data *entry_##name __section(.entry_##name) = &bd

#endif /* __ENTRY_H__ */
