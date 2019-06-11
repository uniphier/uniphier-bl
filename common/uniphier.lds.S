/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
SECTIONS
{
	. = CONFIG_TEXT_BASE;
	__image_start = .;

	.head : {
		KEEP(*(.head))
		__entry = .;
		*(.entry*)
	}

	. = __image_start + 0x100;

	.mbr : {
		KEEP(*(.mbr))
	}

	. = __image_start + 0x200;
	__early_stack = .;

	.text : {
		*(.text*)
	}

	.rodata : {
		*(.rodata*)
	}

	.note : {
		*(.note.*)
	}

	.rela.dyn : {
		__rela_dyn_start = .;
		*(.rela*)
		__rela_dyn_end = .;
	}

	.data : {
		_sdata = .;
		*(.data*)
		_edata = .;
	}

	/*
	 * make sure __image_end points to the end of image.
	 * Without this, .got, .got.plt are layouted after __image_end.
	 */
	. = .;

	__image_end = .;

	. = __image_start + CONFIG_BSS_OFFSET;

	.bss : {
		_sbss = .;
		*(.bss*)
		*(COMMON)
		_ebss = .;
	}

	/DISCARD/ : {
		*(.dynamic)
	}
}

ASSERT(. - __image_start <= CONFIG_MAX_SIZE, ".bss section is too big.");
