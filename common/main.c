// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <board-data.h>
#include <cache.h>
#include <clk.h>
#include <errno.h>
#include <gunzip.h>
#include <init.h>
#include <pll.h>
#include <pinctrl.h>
#include <printk.h>
#include <soc-data.h>
#include <soc-info.h>
#include <timer.h>
#include <timestamp.h>
#include <uart.h>
#include <utils.h>
#include <version.h>

static void uart_soc_init(const struct board_data *bd)
{
	const struct soc_data *sd = bd->soc_data;

	pinctrl_set_mux(sd, &sd->uart_pinmux[bd->uart_port]);
	clk_enable(sd, &sd->uart_clk_regmap);
	uart_init(sd, bd->uart_port);
}

static int soc_id_check(const struct soc_data *sd)
{
	unsigned int soc_id;

	soc_id = get_soc_id(sd);
	if (soc_id != sd->soc_id) {
		pr_err("SoC ID unmatch: (read as: %x, expected: %x)\n"
		       "You are running an image built for a different SoC\n",
		       soc_id, sd->soc_id);
		return -EINVAL;
	}

	return 0;
}

static void dram_param_check(struct board_data *bd)
{
	const struct soc_data *sd = bd->soc_data;
	unsigned long base = sd->dram_base;
	int i;

	/* If dram_freq is not set, use the SoC default value. */
	if (!bd->dram_freq)
		bd->dram_freq = sd->dram_default_freq;

	for (i = 0; i < ARRAY_SIZE(bd->dram_ch); i++) {
		unsigned long size = bd->dram_ch[i].size;

		if (!size)
			continue;

		if (i == 1 && bd->flags & BD_DRAM_SPARSE) {
			if (base > sd->dram_ch1_sparse_base) {
				pr_warn("DRAM ch1 size is too large.\n");
				pr_warn("Sparse mem flag is ignored.\n");
				bd->flags &= ~BD_DRAM_SPARSE;
			} else {
				base = sd->dram_ch1_sparse_base;
			}
		}

		bd->dram_ch[i].base = base;

		pr_info("DRAM ch%d: 0x%lx - ", i, base);

		base += size;

		pr_info("0x%lx\n", base - 1);

		/* If width is not set, use the SoC default value. */
		if (!bd->dram_ch[i].width)
			bd->dram_ch[i].width = sd->dram_default_width[i];
	}
}

static int dram_init(const struct board_data *bd)
{
	const struct soc_data *sd = bd->soc_data;
	int ret, i;

	pr_info("Initializing DRAM... ");

	dpll_init(bd);

	ret = sd->memconf_init(bd);
	if (ret) {
		pr_err("failed to init MEMCONF (%d)\n", ret);
		return ret;
	}

	rst_deassert(sd, &sd->dram_rst_regmap);
	clk_enable(sd, &sd->dram_clk_regmap);

	ret = sd->umc_init(bd);
	if (ret) {
		pr_err("failed to init UMC (%d)\n", ret);
		return ret;
	}

	pr_info("Done\n");

	pr_info("Testing DRAM access... ");

	for (i = 0; i < ARRAY_SIZE(bd->dram_ch); i++) {
		if (!bd->dram_ch[i].size)
			continue;
		ret = memtest(bd->dram_ch[i].base, 0x100);
		if (ret) {
			pr_err("DRAM access test failed in ch%d (%d)\n", i, ret);
			return ret;
		}
	}

	pr_info("Done\n");

	return 0;
}

extern char __image_end;
extern char _sbss;

static int uncompress_piggy(unsigned long dest)
{
	int ret;
	void *out_pos;

	pr_debug("Offset to uncompressed image: %p\n", &__image_end);
	pr_info("Uncompressing next image... ");
	ret = gunzip(&__image_end, &_sbss - &__image_end,
		     (void *)dest, &out_pos);
	if (ret) {
		pr_err("failed to decompress image\n");
		return ret;
	}

	pr_info("Done\n");

	pr_debug("End of decompressor pointer: %p\n", out_pos);

	flush_dcache_range(dest, (unsigned long)out_pos);

	return 0;
}

void __noreturn main(const struct board_data *bd)
{
	const struct soc_data *sd = bd->soc_data;
	struct board_data bd_dup;
	unsigned long next_image_base;
	int ret;

	uart_soc_init(bd);

	/* The console is ready now; print welcome message */
	pr_info("\nUniPhier BL version " VERSION "\n");
	if (git_head[0])
		pr_info("Git: %s\n", git_head);
	if (time_stamp[0])
		pr_info("Built: %s\n", time_stamp);

	if (bd->board_name)
		pr_info("Board: %s\n", bd->board_name);

	ret = soc_id_check(sd);
	if (ret)
		goto die;

	ret = timer_init(sd);
	if (ret)
		goto die;

	if (sd->soc_init) {
		ret = sd->soc_init(bd);
		if (ret)
			goto die;
	}

	/* Use a non-const copy to modify */
	bd_dup = *bd;

	dram_param_check(&bd_dup);

	ret = dram_init(&bd_dup);
	if (ret)
		goto die;

	next_image_base = sd->dram_base + CONFIG_NEXT_IMAGE_OFFSET;

	ret = uncompress_piggy(next_image_base);
	if (ret)
		goto die;

	pr_info("Jumping to next image (0x%lx)\n", next_image_base);
	next_entry(next_image_base);

die:
	while (1)
		;
}
