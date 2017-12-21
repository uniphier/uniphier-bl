/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __HEAP_H__
#define __HEAP_H__

#include <types.h>

int heap_init(unsigned long base, unsigned long end);
void *sbrk(ptrdiff_t increment);

#endif /* __HEAP_H__ */
