/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Based on include/linux/ctype.h, Linux 4.20
 */
#ifndef CTYPE_H
#define CTYPE_H

static inline int isdigit(int c)
{
	return '0' <= c && c <= '9';
}

#endif /* CTYPE_H */
