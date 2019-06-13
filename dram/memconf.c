// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <board-data.h>
#include <errno.h>
#include <io.h>
#include <memconf.h>
#include <printk.h>
#include <sizes.h>
#include <soc-data.h>

#define SG_MEMCONF		0x400

#define SG_MEMCONF_CH0_SZ_MASK		((0x1 << 10) | (0x03 << 0))
#define SG_MEMCONF_CH0_SZ_64M		((0x0 << 10) | (0x01 << 0))
#define SG_MEMCONF_CH0_SZ_128M		((0x0 << 10) | (0x02 << 0))
#define SG_MEMCONF_CH0_SZ_256M		((0x0 << 10) | (0x03 << 0))
#define SG_MEMCONF_CH0_SZ_512M		((0x1 << 10) | (0x00 << 0))
#define SG_MEMCONF_CH0_SZ_1G		((0x1 << 10) | (0x01 << 0))
#define SG_MEMCONF_CH0_NUM_MASK		(0x1 << 8)
#define SG_MEMCONF_CH0_NUM_1		(0x1 << 8)
#define SG_MEMCONF_CH0_NUM_2		(0x0 << 8)

#define SG_MEMCONF_CH1_SZ_MASK		((0x1 << 11) | (0x03 << 2))
#define SG_MEMCONF_CH1_SZ_64M		((0x0 << 11) | (0x01 << 2))
#define SG_MEMCONF_CH1_SZ_128M		((0x0 << 11) | (0x02 << 2))
#define SG_MEMCONF_CH1_SZ_256M		((0x0 << 11) | (0x03 << 2))
#define SG_MEMCONF_CH1_SZ_512M		((0x1 << 11) | (0x00 << 2))
#define SG_MEMCONF_CH1_SZ_1G		((0x1 << 11) | (0x01 << 2))
#define SG_MEMCONF_CH1_NUM_MASK		(0x1 << 9)
#define SG_MEMCONF_CH1_NUM_1		(0x1 << 9)
#define SG_MEMCONF_CH1_NUM_2		(0x0 << 9)

#define SG_MEMCONF_CH2_SZ_MASK		((0x1 << 26) | (0x03 << 16))
#define SG_MEMCONF_CH2_SZ_64M		((0x0 << 26) | (0x01 << 16))
#define SG_MEMCONF_CH2_SZ_128M		((0x0 << 26) | (0x02 << 16))
#define SG_MEMCONF_CH2_SZ_256M		((0x0 << 26) | (0x03 << 16))
#define SG_MEMCONF_CH2_SZ_512M		((0x1 << 26) | (0x00 << 16))
#define SG_MEMCONF_CH2_SZ_1G		((0x1 << 26) | (0x01 << 16))
#define SG_MEMCONF_CH2_NUM_MASK		(0x1 << 24)
#define SG_MEMCONF_CH2_NUM_1		(0x1 << 24)
#define SG_MEMCONF_CH2_NUM_2		(0x0 << 24)

#define SG_MEMCONF_CH2_DISABLE		(0x1 << 21)
#define SG_MEMCONF_SPARSEMEM		(0x1 << 4)

static int memconf_init(const struct board_data *bd, bool have_ch2)
{
	const struct soc_data *sd = bd->soc_data;
	u32 val = 0;
	unsigned long size_per_word;

	/* set up ch0 */
	switch (bd->dram_ch[0].width) {
	case 16:
		val |= SG_MEMCONF_CH0_NUM_1;
		size_per_word = bd->dram_ch[0].size;
		break;
	case 32:
		val |= SG_MEMCONF_CH0_NUM_2;
		size_per_word = bd->dram_ch[0].size >> 1;
		break;
	default:
		pr_err("memconf: unsupported DRAM ch0 width\n");
		return -EINVAL;
	}

	switch (size_per_word) {
	case SZ_64M:
		val |= SG_MEMCONF_CH0_SZ_64M;
		break;
	case SZ_128M:
		val |= SG_MEMCONF_CH0_SZ_128M;
		break;
	case SZ_256M:
		val |= SG_MEMCONF_CH0_SZ_256M;
		break;
	case SZ_512M:
		val |= SG_MEMCONF_CH0_SZ_512M;
		break;
	case SZ_1G:
		val |= SG_MEMCONF_CH0_SZ_1G;
		break;
	default:
		pr_err("memconf: unsupported DRAM ch0 size\n");
		return -EINVAL;
	}

	/* set up ch1 */
	switch (bd->dram_ch[1].width) {
	case 16:
		val |= SG_MEMCONF_CH1_NUM_1;
		size_per_word = bd->dram_ch[1].size;
		break;
	case 32:
		val |= SG_MEMCONF_CH1_NUM_2;
		size_per_word = bd->dram_ch[1].size >> 1;
		break;
	default:
		pr_err("memconf: unsupported DRAM ch1 width\n");
		return -EINVAL;
	}

	switch (size_per_word) {
	case SZ_64M:
		val |= SG_MEMCONF_CH1_SZ_64M;
		break;
	case SZ_128M:
		val |= SG_MEMCONF_CH1_SZ_128M;
		break;
	case SZ_256M:
		val |= SG_MEMCONF_CH1_SZ_256M;
		break;
	case SZ_512M:
		val |= SG_MEMCONF_CH1_SZ_512M;
		break;
	case SZ_1G:
		val |= SG_MEMCONF_CH1_SZ_1G;
		break;
	default:
		pr_err("memconf: unsupported DRAM ch1 size\n");
		return -EINVAL;
	}

	/* is sparse mem? */
	if (bd->flags & BD_DRAM_SPARSE)
		val |= SG_MEMCONF_SPARSEMEM;

	if (!have_ch2)
		goto out;

	if (!bd->dram_ch[2].size) {
		val |= SG_MEMCONF_CH2_DISABLE;
		goto out;
	}

	/* set up ch2 */
	switch (bd->dram_ch[2].width) {
	case 16:
		val |= SG_MEMCONF_CH2_NUM_1;
		size_per_word = bd->dram_ch[2].size;
		break;
	case 32:
		val |= SG_MEMCONF_CH2_NUM_2;
		size_per_word = bd->dram_ch[2].size >> 1;
		break;
	default:
		pr_err("memconf: unsupported DRAM ch2 width\n");
		return -EINVAL;
	}

	switch (size_per_word) {
	case SZ_64M:
		val |= SG_MEMCONF_CH2_SZ_64M;
		break;
	case SZ_128M:
		val |= SG_MEMCONF_CH2_SZ_128M;
		break;
	case SZ_256M:
		val |= SG_MEMCONF_CH2_SZ_256M;
		break;
	case SZ_512M:
		val |= SG_MEMCONF_CH2_SZ_512M;
		break;
	case SZ_1G:
		val |= SG_MEMCONF_CH2_SZ_1G;
		break;
	default:
		pr_err("memconf: unsupported DRAM ch2 size\n");
		return -EINVAL;
	}

out:
	writel(val, sd->socglue_base + SG_MEMCONF);

	return 0;
}

int memconf_2ch_init(const struct board_data *bd)
{
	return memconf_init(bd, false);
}

int memconf_3ch_init(const struct board_data *bd)
{
	return memconf_init(bd, true);
}

int memconf_dummy_init(const struct board_data *bd)
{
	return 0;
}
