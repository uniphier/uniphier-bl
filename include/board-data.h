/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __BOARD_DATA_H__
#define __BOARD_DATA_H__

#include <bits.h>
#include <dram-data.h>

struct board_data {
	const char *board_name;
	const struct soc_data *soc_data;
	unsigned int uart_port;
	unsigned int dram_freq;
	struct dram_ch dram_ch[MAX_NR_DRAM_CH];
	unsigned int flags;

/* board type */
#define BD_BOARD_GET_TYPE(f)		((f) & 0x7)

#define BD_BOARD_LD20_REF		0	/* LD20 reference */
#define BD_BOARD_LD20_GLOBAL		1	/* LD20 TV Set */
#define BD_BOARD_LD20_C1		2	/* LD20 TV Set C1 */
#define BD_BOARD_LD21_REF		3	/* LD21 reference */
#define BD_BOARD_LD21_GLOBAL		4	/* LD21 TV Set */

#define BD_BOARD_PXS3_REF		0	/* PXs3 reference */

/* DRAM layout */
#define BD_DRAM_SPARSE			BIT(8)

/* DRAM PLL */
#define BD_DPLL_SSC_CHANGE_RATE		BIT(9)
#define BD_DPLL_SSC_GET_RATE(f)		(((f) >> 10) & 0x7)

#define BD_DPLL_SSC_RATE_0		((0 << 10) | BD_DPLL_SSC_CHANGE_RATE)
#define BD_DPLL_SSC_RATE_1		((1 << 10) | BD_DPLL_SSC_CHANGE_RATE)
#define BD_DPLL_SSC_RATE_2		((2 << 10) | BD_DPLL_SSC_CHANGE_RATE)
#define BD_DPLL_SSC_RATE_3		((3 << 10) | BD_DPLL_SSC_CHANGE_RATE)
};

#endif /* __BOARD_DATA_H__ */
