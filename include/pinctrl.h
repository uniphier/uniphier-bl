/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017-2019 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __PINCTRL_H__
#define __PINCTRL_H__

struct pinmux;

void pinctrl_set_mux(const struct pinmux *mux);

#endif /* __PINCTRL_H__ */
