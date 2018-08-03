/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Based on from arch/arm64/include/asm/sysreg.h, Linux 4.14
 */

#ifndef __SYSREG_H__
#define __SYSREG_H__

#include <stringify.h>
#include <types.h>

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
