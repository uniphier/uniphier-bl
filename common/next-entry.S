/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#include <linkage.h>
#include <sysreg.h>

ENTRY(next_entry)
	mrs	x1, sctlr_el3
	bic	x1, x1, #SCTLR_ELx_C
	bic	x1, x1, #SCTLR_ELx_M
	msr	sctlr_el3, x1
	ic	iallu			/* invalidate icache */
	isb	sy
	br	x0
ENDPROC(next_entry)
