/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <compiler.h>
#include <heap.h>
#include <malloc.h>
#include <string.h>
#include <utils.h>

/* memory allocated by malloc() is aligned for any built-in type */
#define MALLOC_ALIGNMENT	(sizeof(void *))

#define ZERO_SIZE_PTR		((void *)1)
#define ZERO_OR_NULL_PTR(x)	((x) <= (ZERO_SIZE_PTR))

void *memalign(size_t alignment, size_t size)
{
	void *brk, *p;

	if (unlikely(!size))
		return ZERO_SIZE_PTR;

	brk = sbrk(0);

	/*
	 * REVISIT:
	 * 'alignment' must be a power of two.
	 *  If not, should it be rounded up to the nearest power of two?
	 */
	size += PTR_ALIGN(brk, alignment) - brk;

	p = sbrk(size);
	if (unlikely(p == (void *)-1))
		return NULL;

	return p;
}

void *malloc(size_t size)
{
	return memalign(MALLOC_ALIGNMENT, size);
}

void free(void *ptr)
{
}

void *calloc(size_t nmemb, size_t size)
{
	void *p;

	size *= nmemb;

	p = malloc(size);
	if (likely(!ZERO_OR_NULL_PTR(p)))
		memset(p, 0, size);

	return p;
}

void *realloc(void *ptr, size_t size)
{
	void *new;

	/* always move the memory */
	new = malloc(size);

	if (likely(new && !ZERO_OR_NULL_PTR(ptr))) {
		memcpy(new, ptr, size);
		free(ptr);
	}

	return new;
}
