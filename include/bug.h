/*
 * Based on include/asm-generic/bug.h, Linux 4.14
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef __BUG_H__
#define __BUG_H__

#include <compiler.h>
#include <printk.h>
#include <types.h>

#define BUG() do { \
	printk("BUG at %s():%d!\n", __func__, __LINE__); \
	do {} while (1); \
} while (0)

#define BUG_ON(condition) do { if (unlikely(condition)) BUG(); } while (0)

#define __WARN()	printk("WARNING at %s():%d!\n", __func__, __LINE__)

#define __WARN_printf(arg...)	do { printk(arg); __WARN(); } while (0)

#define WARN(condition, format...) ({					\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		__WARN_printf(format);					\
	unlikely(__ret_warn_on);					\
})

#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		__WARN();						\
	unlikely(__ret_warn_on);					\
})

#define WARN_ON_ONCE(condition)	({					\
	static bool __warned;						\
	int __ret_warn_once = !!(condition);				\
									\
	if (unlikely(__ret_warn_once && !__warned)) {			\
		__warned = true;					\
		WARN_ON(1);						\
	}								\
	unlikely(__ret_warn_once);					\
})

#endif /* __BUG_H__ */
