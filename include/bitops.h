/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Based on include/uapi/linux/swab.h, Linux 4.14
 */

#ifndef __BITOPS_H__
#define __BITOPS_H__

#define BITS_PER_LONG		64

#define BIT(nr)			(1UL << (nr))

/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l) \
	(((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#endif /* __BITOPS_H__ */
