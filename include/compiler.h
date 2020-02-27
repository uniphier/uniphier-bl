/* SPDX-License-Identifier: GPL-2.0-or-later */

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
#define __used			__attribute__((used))
#define __always_unused		__attribute__((unused))
#define __maybe_unused		__attribute__((unused))

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

#if GCC_VERSION >= 40300
#ifndef __CHECKER__
# define __compiletime_warning(message) __attribute__((warning(message)))
# define __compiletime_error(message) __attribute__((error(message)))
#endif
#endif

#ifndef __compiletime_warning
# define __compiletime_warning(message)
#endif
#ifndef __compiletime_error
# define __compiletime_error(message)
#endif

#ifdef __OPTIMIZE__
# define __compiletime_assert(condition, msg, prefix, suffix)		\
	do {								\
		extern void prefix ## suffix(void) __compiletime_error(msg); \
		if (!(condition))					\
			prefix ## suffix();				\
	} while (0)
#else
# define __compiletime_assert(condition, msg, prefix, suffix) do { } while (0)
#endif

#define _compiletime_assert(condition, msg, prefix, suffix) \
	__compiletime_assert(condition, msg, prefix, suffix)

/**
 * compiletime_assert - break build and emit msg if condition is false
 * @condition: a compile-time constant condition to check
 * @msg:       a message to emit if condition is false
 *
 * In tradition of POSIX assert, this macro will break the build if the
 * supplied condition is *false*, emitting the supplied error message if the
 * compiler has support to do so.
 */
#define compiletime_assert(condition, msg) \
	_compiletime_assert(condition, msg, __compiletime_assert_, __LINE__)

#endif /* __COMPILER_H__ */
