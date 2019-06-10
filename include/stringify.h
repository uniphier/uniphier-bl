/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copied from include/linux/stringify.h, Linux 4.14
 */

#ifndef __STRINGIFY_H__
#define __STRINGIFY_H__

/*
 * Indirect stringification.  Doing two levels allows the parameter to be a
 * macro itself.  For example, compile with -DFOO=bar, __stringify(FOO)
 * converts to "bar".
 */

#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

#endif	/* __STRINGIFY_H__ */
