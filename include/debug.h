/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef CONFIG_DEBUG
void memdump(unsigned long start, unsigned long size);
#else
static inline void memdump(unsigned long start, unsigned long size)
{
}
#endif

#endif /* __DEBUG_H__ */
