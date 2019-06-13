/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __UMC_H__
#define __UMC_H__

struct board_data;

int ld11_umc_init(const struct board_data *bd);
int ld20_umc_init(const struct board_data *bd);
int pxs3_umc_init(const struct board_data *bd);

#endif /* __UMC_H__ */
