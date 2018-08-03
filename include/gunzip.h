/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __GUNZIP_H__
#define __GUNZIP_H__

int gunzip(const void *in_buf, unsigned long in_len,
	   void *out_buf, void **out_pos);

#endif /* __GUNZIP_H__ */
