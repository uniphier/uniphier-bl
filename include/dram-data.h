/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __DRAM_DATA_H__
#define __DRAM_DATA_H__

#define MAX_NR_DRAM_CH		3

struct dram_ch {
	unsigned long base;
	unsigned long size;
	unsigned int width;
};

#endif /* __DRAM_DATA_H__ */
