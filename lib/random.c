/*
 * Copyright (C) 2018 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <random.h>
#include <types.h>

static unsigned int next;

void srand(unsigned int seed)
{
	next = seed;
}

static unsigned int __rand(void)
{
	next = next * 1103515245 + 12345;

	return next;
}

void get_random_bytes(void *buf, int nbytes)
{
	u8 *byte = buf;

	while (nbytes--)
		*byte++ = __rand();
}
