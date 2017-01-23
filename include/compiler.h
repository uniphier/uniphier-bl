/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __COMPILER_H__
#define __COMPILER_H__

#define __iomem
#define __noreturn		__attribute__((noreturn))
#define __printf(a, b)		__attribute__((format(printf, a, b)))
#define __section(S)		__attribute__((__section__(#S)))

#endif /* __COMPILER_H__ */
