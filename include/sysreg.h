/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Based on from arch/arm64/include/asm/sysreg.h, Linux 4.14
 */

#ifndef __SYSREG_H__
#define __SYSREG_H__

#include <bits.h>
#include <stringify.h>
#include <types.h>

/* Common SCTLR_ELx flags. */
#define SCTLR_ELx_DSSBS	(BIT(44))
#define SCTLR_ELx_ENIA	(BIT(31))
#define SCTLR_ELx_ENIB	(BIT(30))
#define SCTLR_ELx_ENDA	(BIT(27))
#define SCTLR_ELx_EE    (BIT(25))
#define SCTLR_ELx_IESB	(BIT(21))
#define SCTLR_ELx_WXN	(BIT(19))
#define SCTLR_ELx_ENDB	(BIT(13))
#define SCTLR_ELx_I	(BIT(12))
#define SCTLR_ELx_SA	(BIT(3))
#define SCTLR_ELx_C	(BIT(2))
#define SCTLR_ELx_A	(BIT(1))
#define SCTLR_ELx_M	(BIT(0))

/*
 * Unlike read_cpuid, calls to read_sysreg are never expected to be
 * optimized away or replaced with synthetic values.
 */
#define read_sysreg(r) ({					\
	u64 __val;						\
	asm volatile("mrs %0, " __stringify(r) : "=r" (__val));	\
	__val;							\
})

/*
 * The "Z" constraint normally means a zero immediate, but when combined with
 * the "%x0" template means XZR.
 */
#define write_sysreg(v, r) do {					\
	u64 __val = (u64)(v);					\
	asm volatile("msr " __stringify(r) ", %x0"		\
		     : : "rZ" (__val));				\
} while (0)

#endif	/* __SYSREG_H__ */
