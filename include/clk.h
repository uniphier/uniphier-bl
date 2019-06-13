/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __CLK_H__
#define __CLK_H__

struct regmap;
struct soc_data;

void clk_enable(const struct soc_data *sd, const struct regmap *regmap);
void rst_deassert(const struct soc_data *sd, const struct regmap *regmap);

#endif /* __CLK_H__ */
