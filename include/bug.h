/*
 * Based on include/asm-generic/bug.h, Linux 4.14
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef __BUG_H__
#define __BUG_H__

#include <compiler.h>

#define __WARN()

#define WARN_ON(condition) ({						\
	int __ret_warn_on = !!(condition);				\
	if (unlikely(__ret_warn_on))					\
		__WARN();						\
	unlikely(__ret_warn_on);					\
})

#endif /* __BUG_H__ */
