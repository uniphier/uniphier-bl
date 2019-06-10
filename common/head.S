/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#include <linkage.h>

	.section .head,"ax"
ENTRY(head)
	/* The boot ROM has enabled D-cache.  Disable it first. */
	mrs	x1, sctlr_el3
	bic	x1, x1, #4		/* clear C */
	bic	x1, x1, #1		/* clear M */
	msr	sctlr_el3, x1
	isb
	dsb	sy

	/* zero out .bss section */
	ldr	x0, =_sbss
	ldr	x1, =_ebss
	b	1f
0:	stp	xzr, xzr, [x0], #16
1:	cmp	x0, x1
	b.lo	0b

	/* set temporary sp for calling get_stack() */
	ldr	x0, =__early_stack
	mov	sp, x0

	ldr	x0, =__entry
	ldr	x0, [x0]		/* x0 = (struct board_data *) */
	mov	x19, x0			/* x19: callee-saved */

	bl	get_stack

	/* set real stack pointer */
	mov	sp, x0

	mov	x0, x19
	b	main
ENDPROC(head)
