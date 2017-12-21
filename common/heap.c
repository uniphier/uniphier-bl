/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <bug.h>
#include <errno.h>
#include <heap.h>

static void *brk_start;
static void *brk_end;
static void *brk;

int heap_init(unsigned long start, unsigned long end)
{
	if (WARN_ON(start > end))
		return -EINVAL;

	brk_start = (void *)start;
	brk_end = (void *)end;
	brk = brk_start;

	return 0;
}

void *sbrk(ptrdiff_t increment)
{
	void *prev, *new;

	new = brk + increment;

	if (new < brk_start || new > brk_end)
		return (void *)-1;

	prev = brk;
	brk = new;

	return prev;
}
