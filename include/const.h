/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Based on include/uapi/linux/const.h, Linux 5.1
 */
/* const.h: Macros for dealing with constants.  */

#ifndef __CONST_H
#define __CONST_H

/*
 * Some constant macros are used in both assembler and
 * C code.  Therefore we cannot annotate them always with
 * 'UL' and other type specifiers unilaterally.  We
 * use the following macros to deal with this.
 *
 * Similarly, _AT() will cast an expression with a type in C, but
 * leave it unchanged in asm.
 */

#ifdef __ASSEMBLY__
#define AC(X,Y)		X
#define AT(T,X)		X
#else
#define __AC(X,Y)	(X##Y)
#define AC(X,Y)		__AC(X,Y)
#define AT(T,X)		((T)(X))
#endif

#define UL(x)		(AC(x, UL))
#define ULL(x)		(AC(x, ULL))

#endif /* __CONST_H */
