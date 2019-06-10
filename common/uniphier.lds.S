/* SPDX-License-Identifier: GPL-2.0+ */
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

	.text : {
		*(.text*)
	}

	.rodata : {
		*(.rodata*)
	}

	.note : {
		*(.note.*)
	}

	.data : {
		_sdata = .;
		*(.data*)
		_edata = .;
	}

	__image_end = .;

	. = CONFIG_BSS_BASE;

	.bss : {
		_sbss = .;
		*(.bss*)
		*(COMMON)
		_ebss = .;
	}
}
