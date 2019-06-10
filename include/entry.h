/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __ENTRY_H__
#define __ENTRY_H__

#include <compiler.h>

#define BOARD_DECLARE(name, data)					\
	/* declaration to suppress "Should it be static?" warning of sparse */ \
	extern typeof(data) *entry_##name;					\
	typeof(data) *entry_##name __section(.entry_##name) = &data

#endif /* __ENTRY_H__ */
