/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <byteorder.h>
#include <compiler.h>
#include <types.h>

/*
 * The first 512 bytes should form the MBR when the boot image is created for
 * the USB boot mode.  The struct mbr_tail represents byte 256-511 of the MBR.
 * The byte 256-259 is the SoC-specific signature (0xaa5555aa) for the USB boot.
 */

struct __packed partition_table {
	u8 boot_flag;
	u8 start_chs[3];
	u8 type;
	u8 end_chs[3];
	le32 lba_start;
	le32 lba_size;
};

struct __packed mbr_tail {
	u8 usb_boot_sig[4];
	u8 reserve[180];
	le32 disk_id;
	u8 reserve2[2];
	struct partition_table partition[4];
	u8 signature[2];
};

static const struct mbr_tail mbr_tail __used __section(.mbr) = {
	.usb_boot_sig = {0xaa, 0x55, 0x55, 0xaa},
	.disk_id = cpu_to_le32(0xdeadbeef),
	.partition[0] = {
		.start_chs = {0xfe, 0xff, 0xff},
		.type = 0x0c,
		.end_chs = {0xfe, 0xff, 0xff},
		.lba_start = cpu_to_le32(0x400),
		.lba_size = cpu_to_le32(0xf600),
	},
	.signature = {0x55, 0xaa},
};
