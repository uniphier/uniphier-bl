/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2018 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <types.h>
#include <utils.h>

void srand(unsigned int seed);
void get_random_bytes(void *buf, int nbytes);

static inline u32 get_random_u32(void)
{
	u32 tmp;

	get_random_bytes(&tmp, sizeof(tmp));

	return tmp;
}

static inline u64 get_random_u64(void)
{
	u64 tmp;

	get_random_bytes(&tmp, sizeof(tmp));

	return tmp;
}

static inline unsigned int get_random_int(void)
{
	unsigned int tmp;

	get_random_bytes(&tmp, sizeof(tmp));

	return tmp;
}

static inline unsigned long get_random_long(void)
{
	unsigned long tmp;

	get_random_bytes(&tmp, sizeof(tmp));

	return tmp;
}

#define RAND_MAX	INT_MAX

static inline int rand(void)
{
	unsigned int tmp;

	tmp = get_random_int();
	/* drop the MSB to not return a negative value. */
	tmp <<= 1;
	tmp >>= 1;

	return tmp;
}

#endif /* __RANDOM_H__ */
