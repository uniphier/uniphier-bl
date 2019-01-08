// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (C) 2018 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <errno.h>
#include <init.h>
#include <io.h>
#include <printk.h>
#include <random.h>

int memtest(unsigned long start, unsigned long size)
{
	unsigned long addr;
	unsigned long end = start + size;
	int ret = 0;

	srand(start);
	addr = start;

	while (addr < end) {
		writel(get_random_u32(), IOMEM(addr));
		addr += 4;
	}

	srand(start);
	addr = start;

	while (addr < end) {
		u32 expected = get_random_u32();
		u32 data = readl(IOMEM(addr));

		if (data != expected) {
			pr_err("memory error (addess: %lx, expect: %08x, read as %08x)\n",
			       addr, expected, data);
			ret = -EIO;
		}
		addr += 4;
	}

	return ret;
}
