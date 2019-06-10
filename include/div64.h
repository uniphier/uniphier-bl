/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Based on include/asm-generic/div64.h, Linux 4.20
 */

#ifndef DIV64_H
#define DIV64_H

#include <types.h>

/**
 * do_div - returns 2 values: calculate remainder and update new dividend
 * @n: pointer to u64 dividend (will be updated)
 * @base: u32 divisor
 *
 * Summary:
 * ``u32 remainder = *n % base;``
 * ``*n = *n / base;``
 *
 * Return: (u32)remainder
 *
 * NOTE: macro parameter @n is evaluated multiple times,
 * beware of side effects!
 */
#define do_div(n, base) ({					\
	u32 __base = (base);					\
	u32 __rem;						\
	__rem = ((u64)(n)) % __base;				\
	(n) = ((u64)(n)) / __base;				\
	__rem;							\
})

#endif /* DIV64_H */
