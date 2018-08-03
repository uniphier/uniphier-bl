// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <debug.h>
#include <io.h>
#include <printk.h>

void memdump(unsigned long start, unsigned long size)
{
	unsigned long addr = start;
	unsigned long end = start + size;

	int column = 0;

	while (addr < end) {
		if (column == 0)
			printk("%lx:", addr);

		printk(" %x", readl((void __iomem *)addr));

		addr += 4;
		column++;

		if (column == 4) {
			printk("\n");
			column = 0;
		}
	}
}
