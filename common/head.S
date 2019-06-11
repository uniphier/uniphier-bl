/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#include <linkage.h>
#include <sysreg.h>

	.section .head,"ax"
ENTRY(head)
	/* The boot ROM has enabled D-cache.  Disable it first. */
	mrs	x0, sctlr_el3
	orr	x0, x0, #SCTLR_ELx_I
	bic	x0, x0, #SCTLR_ELx_C
	bic	x0, x0, #SCTLR_ELx_M
	msr	sctlr_el3, x0
	isb
	dsb	sy

#ifdef CONFIG_PIE
	adr	x0, here
	ldr	x1, here
	sub	x2, x0, x1		/* run-time-address - link-address */
	adr	x0, __rela_dyn_start
	adr	x1, __rela_dyn_end
	b	1f
0:	ldp	x4, x5, [x0], #16	/* x4: r_offset, x5: r_info */
	ldr	x6, [x0], #8		/* x6: r_addend */
	cmp	w5, #1027		/* R_AARCH64_RELATIVE */
	bne	1f
	add	x4, x4, x2
	add	x6, x6, x2
	str	x6, [x4]
1:	cmp	x0, x1
	b.lo	0b

	ic	iallu			/* invalidate icache */
	isb	sy
#endif

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

#ifdef CONFIG_PIE
	.section .rodata,"a"
	.align	3
here:
	.quad	.
#endif
