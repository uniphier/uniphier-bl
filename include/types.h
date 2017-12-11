/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#define NULL			((void *)0)

enum {
	false	= 0,
	true	= 1
};

typedef _Bool			bool;
typedef unsigned int		size_t;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

#ifdef __CHECKER__
#define __bitwise		__attribute__((bitwise))
#else
#define __bitwise
#endif

typedef u16 __bitwise		le16;
typedef u16 __bitwise		be16;
typedef u32 __bitwise		le32;
typedef u32 __bitwise		be32;
typedef u64 __bitwise		le64;
typedef u64 __bitwise		be64;

#endif /* __TYPES_H__ */
