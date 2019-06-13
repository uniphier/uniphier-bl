/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __INIT_H__
#define __INIT_H__

#include <compiler.h>

struct board_data;

unsigned long get_stack(const struct board_data *bd);
void __noreturn main(const struct board_data *bd);

int memconf_init(const struct board_data *bd, int have_ch2);

int memtest(unsigned long start, unsigned long size);

void __noreturn next_entry(unsigned long entry_addr);

#endif /* __INIT_H__ */
