/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __IO_H__
#define __IO_H__

#include <compiler.h>
#include <types.h>

static inline void writeb(u8 val, void *__iomem addr)
{
	*(volatile u8 *)addr = val;
}

static inline void writew(u16 val, void *__iomem addr)
{
	*(volatile u16 *)addr = val;
}

static inline void writel(u32 val, void *__iomem addr)
{
	*(volatile u32 *)addr = val;
}

static inline void writeq(u64 val, void *__iomem addr)
{
	*(volatile u64 *)addr = val;
}

static inline u8 readb(void *addr)
{
	return *(volatile u8 *)addr;
}

static inline u16 readw(void *addr)
{
	return *(volatile u16 *)addr;
}

static inline u32 readl(void *addr)
{
	return *(volatile u32 *)addr;
}

static inline u64 readq(void *addr)
{
	return *(volatile u64 *)addr;
}

#endif /* __IO_H__ */
