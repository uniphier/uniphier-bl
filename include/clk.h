/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __CLK_H__
#define __CLK_H__

void clk_enable_uart(unsigned int clk_bits);
void clk_enable_dram(unsigned int clk_bits, unsigned int rst_bits);

#endif /* __CLK_H__ */
