/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Based on include/uapi/linux/swab.h, Linux 4.10
 */

#ifndef __SWAB_H__
#define __SWAB_H__

#include <compiler.h>
#include <types.h>

/*
 * casts are necessary for constants, because we never know how for sure
 * how U/UL/ULL map to u16, u32, u64. At least not in a portable way.
 */
#define __swab16(x) ((u16)(					\
	(((u16)(x) & (u16)0x00ffU) << 8) |			\
	(((u16)(x) & (u16)0xff00U) >> 8)))

#define __swab32(x) ((u32)(					\
	(((u32)(x) & (u32)0x000000ffUL) << 24) |		\
	(((u32)(x) & (u32)0x0000ff00UL) <<  8) |		\
	(((u32)(x) & (u32)0x00ff0000UL) >>  8) |		\
	(((u32)(x) & (u32)0xff000000UL) >> 24)))

#define __swab64(x) ((u64)(					\
	(((u64)(x) & (u64)0x00000000000000ffULL) << 56) |	\
	(((u64)(x) & (u64)0x000000000000ff00ULL) << 40) |	\
	(((u64)(x) & (u64)0x0000000000ff0000ULL) << 24) |	\
	(((u64)(x) & (u64)0x00000000ff000000ULL) <<  8) |	\
	(((u64)(x) & (u64)0x000000ff00000000ULL) >>  8) |	\
	(((u64)(x) & (u64)0x0000ff0000000000ULL) >> 24) |	\
	(((u64)(x) & (u64)0x00ff000000000000ULL) >> 40) |	\
	(((u64)(x) & (u64)0xff00000000000000ULL) >> 56)))

/**
 * swab16 - return a byteswapped 16-bit value
 * @x: value to byteswap
 */
#ifdef __HAVE_BUILTIN_BSWAP16__
#define swab16(x) (u16)__builtin_bswap16((u16)(x))
#else
#define swab16(x) (__swab16(x))
#endif

/**
 * swab32 - return a byteswapped 32-bit value
 * @x: value to byteswap
 */
#ifdef __HAVE_BUILTIN_BSWAP32__
#define swab32(x) (u32)__builtin_bswap32((u32)(x))
#else
#define swab32(x) (__swab32(x))
#endif

/**
 * swab64 - return a byteswapped 64-bit value
 * @x: value to byteswap
 */
#ifdef __HAVE_BUILTIN_BSWAP64__
#define swab64(x) (u64)__builtin_bswap64((u64)(x))
#else
#define swab64(x) (__swab64(x))
#endif

#endif /* __SWAB_H__ */
