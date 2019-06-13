/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __PLL_H__
#define __PLL_H__

struct board_data;
struct soc_data;

void dpll_init(const struct board_data *bd);
void pll_set_freq(const struct soc_data *sd, int pll_id,
		  unsigned int freq, unsigned int divn);

#endif /* __PLL_H__ */
