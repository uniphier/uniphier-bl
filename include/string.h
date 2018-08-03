/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef __STRING_H__
#define __STRING_H__

#include <types.h>

int memcmp(const void *cs, const void *ct, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);
void *memset(void *s, int c, size_t count);

#endif /* __STRING_H__ */
