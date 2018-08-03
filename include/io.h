/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __IO_H__
#define __IO_H__

#include <compiler.h>
#include <types.h>

#define IOMEM(addr)		((void __force __iomem *)(addr))

static inline void writeb(u8 val, void __iomem *addr)
{
	*(volatile u8 __force *)addr = val;
}

static inline void writew(u16 val, void __iomem *addr)
{
	*(volatile u16 __force *)addr = val;
}

static inline void writel(u32 val, void __iomem *addr)
{
	*(volatile u32 __force *)addr = val;
}

static inline void writeq(u64 val, void __iomem *addr)
{
	*(volatile u64 __force *)addr = val;
}

static inline u8 readb(void __iomem *addr)
{
	return *(volatile u8 __force *)addr;
}

static inline u16 readw(void __iomem *addr)
{
	return *(volatile u16 __force *)addr;
}

static inline u32 readl(void __iomem *addr)
{
	return *(volatile u32 __force *)addr;
}

static inline u64 readq(void __iomem *addr)
{
	return *(volatile u64 __force *)addr;
}

#endif /* __IO_H__ */
