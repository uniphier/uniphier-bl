/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __LINKAGE_H__
#define __LINKAGE_H__

#define __ALIGN		.align 4

/* Some toolchains use other characters (e.g. '`') to mark new line in macro */
#ifndef ASM_NL
#define ASM_NL		 ;
#endif

#define ENTRY(name) \
	.globl name ASM_NL \
	name:

#define END(name) \
	.size name, .-name

/* If symbol 'name' is treated as a subroutine (gets called, and returns)
 * then please use ENDPROC to mark 'name' as STT_FUNC for the benefit of
 * static analysis tools such as stack depth analyzer.
 */
#define ENDPROC(name) \
	.type name, @function ; \
	END(name)

#endif /* __LINKAGE_H__ */
