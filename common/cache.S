/* SPDX-License-Identifier: GPL-2.0+ */

/*
 * x0: start address, inclusive
 * x1: end address, exclusive
 * x2, x3: corrupted
 */
#include <linkage.h>

ENTRY(flush_dcache_range)
	mrs	x3, ctr_el0
	ubfx	x3, x3, #16, #4
	mov	x2, #4
	lsl	x2, x2, x3	/* the smallest line size */

	sub	x3, x2, #1
	bic	x0, x0, x3
loop:
	dc	civac, x0
	add	x0, x0, x2
	cmp	x0, x1
	b.lo    loop
	dsb	sy
	ret
ENDPROC(flush_dcache_range)
