/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <linkage.h>

	.section ".head","ax"
ENTRY(head)
	/* zero out .bss section */
	ldr	x0, =_sbss
	ldr	x1, =_ebss
	b	1f
0:	stp	xzr, xzr, [x0], #16
1:	cmp	x0, x1
	b.lo	0b

	/* set stack */
	ldr	x0, =CONFIG_STACK_BASE
	mov	sp, x0

	ldr	x0, =_stext
	br	x0
ENDPROC(head)
