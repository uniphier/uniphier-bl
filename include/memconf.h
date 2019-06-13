/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __MEMCONF_H__
#define __MEMCONF_H__

struct board_data;

int memconf_2ch_init(const struct board_data *bd);
int memconf_3ch_init(const struct board_data *bd);
int memconf_dummy_init(const struct board_data *bd);

#endif /* __MEMCONF_H__ */
