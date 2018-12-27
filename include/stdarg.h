/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copied from include/lib/libc/stdarg.h, Trusted Firmware-A v2.0
 */
/*
 * Copyright (c) 2012-2017 Roberto E. Vargas Caballero
 */
/*
 * Portions copyright (c) 2018, ARM Limited and Contributors.
 * All rights reserved.
 */

#ifndef __STDARG_H__
#define __STDARG_H__

#define va_list __builtin_va_list
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_end(ap) __builtin_va_end(ap)
#define va_copy(to, from) __builtin_va_copy(to, from)
#define va_arg(to, type) __builtin_va_arg(to, type)

#endif /* __STDARG_H__ */
