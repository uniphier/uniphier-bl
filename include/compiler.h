/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __COMPILER_H__
#define __COMPILER_H__

#ifdef __CHECKER__
#define __force			__attribute__((force))
#define __iomem			__attribute__((noderef, address_space(2)))
#else
#define __force
#define __iomem
#endif

#define __noreturn		__attribute__((noreturn))
#define __packed		__attribute__((packed))
#define __printf(a, b)		__attribute__((format(printf, a, b)))
#define __section(S)		__attribute__((__section__(#S)))

#define likely(x)		__builtin_expect(!!(x), 1)
#define unlikely(x)		__builtin_expect(!!(x), 0)

/*
 * Common definitions for all gcc versions go here.
 */
#define GCC_VERSION (__GNUC__ * 10000		\
		     + __GNUC_MINOR__ * 100	\
		     + __GNUC_PATCHLEVEL__)

#if GCC_VERSION >= 40400
#define __HAVE_BUILTIN_BSWAP32__
#define __HAVE_BUILTIN_BSWAP64__
#endif
#if GCC_VERSION >= 40800
#define __HAVE_BUILTIN_BSWAP16__
#endif

#endif /* __COMPILER_H__ */
