/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __TIMER_H__
#define __TIMER_H__

struct soc_data;

int timer_init(const struct soc_data *sd);

#endif /* __TIMER_H__ */
