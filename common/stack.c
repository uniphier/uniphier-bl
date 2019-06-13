// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2019 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <board-data.h>
#include <init.h>
#include <soc-data.h>

unsigned long get_stack(const struct board_data *bd)
{
	return bd->soc_data->stack_base;
}
