/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <cache.h>
#include <errno.h>
#include <gunzip.h>
#include <init.h>
#include <printk.h>
#include <timestamp.h>
#include <version.h>

#define DRAM_CH0_BASE		0x80000000
#define DRAM_CH1_SPARSE_BASE	0xc0000000

static void uart_soc_init(const struct soc_data *sd,
			  const struct board_data *bd)
{
	const struct uart_pinmux *pinmux;

	pinmux = &sd->uart_pinmux[bd->uart_port];
	pinctrl_set_mux(pinmux->pin, pinmux->mux);
	clk_enable_uart(sd->uart_clk_bits);
	uart_init(bd->uart_port, sd->uart_clk_rate);
}

static int soc_id_check(const struct soc_data *sd)
{
	unsigned int soc_id;

	soc_id = get_soc_id();
	if (soc_id != sd->soc_id) {
		pr_err("SoC ID unmatch: (read as: %x, expected: %x)\n"
		       "You are running an image built for a different SoC\n",
		       soc_id, sd->soc_id);
		return -EINVAL;
	}

	return 0;
}

static void dram_param_check(const struct soc_data *sd, struct board_data *bd)
{
	unsigned long base = DRAM_CH0_BASE;
	int i;

	/* If dram_freq is not set, use the SoC default value. */
	if (!bd->dram_freq)
		bd->dram_freq = sd->dram_default_freq;

	for (i = 0; i < ARRAY_SIZE(bd->dram_ch); i++) {
		unsigned long size = bd->dram_ch[i].size;

		if (!size)
			break;

		if (i == 1 && bd->flags & BD_DRAM_SPARSE) {
			if (base > DRAM_CH1_SPARSE_BASE) {
				pr_warn("DRAM ch1 size is too large.\n");
				pr_warn("Sparse mem flag is ignored.\n");
				bd->flags &= ~BD_DRAM_SPARSE;
			} else {
				base = DRAM_CH1_SPARSE_BASE;
			}
		}

		pr_info("DRAM ch%d: 0x%lx - ", i, base);

		base += size;

		pr_info("0x%lx\n", base - 1);

		/* If width is not set, use the SoC default value. */
		if (!bd->dram_ch[i].width)
			bd->dram_ch[i].width = sd->dram_default_width[i];
	}
}

static int dram_init(const struct soc_data *sd, const struct board_data *bd)
{
	int ret;

	pr_info("Initializing DRAM... ");

	ret = memconf_init(bd, sd->dram_have_ch2);
	if (ret) {
		pr_err("failed to init MEMCONF (%d)\n", ret);
		return ret;
	}

	clk_enable_dram(sd->dram_clk_bits, sd->dram_rst_bits);

	ret = sd->umc_init(bd);
	if (ret) {
		pr_err("failed to init UMC (%d)\n", ret);
		return ret;
	}

	pr_info("Done\n");

	return 0;
}

extern char __image_end[];

static int uncompress_piggy(void)
{
	int ret;
	void *out_pos;

	pr_debug("Offset to uncompressed image: %p\n", &__image_end);
	pr_info("Uncompressing next image... ");
	ret = gunzip(&__image_end,
		     CONFIG_BSS_BASE - (unsigned long)&__image_end,
		     (void *)CONFIG_NEXT_IMAGE_BASE, &out_pos);
	if (ret) {
		pr_err("failed to decompress image\n");
		return ret;
	}

	pr_info("Done\n");

	pr_debug("End of decompressor pointer: %p\n", out_pos);

	flush_dcache_range(CONFIG_NEXT_IMAGE_BASE, (unsigned long)out_pos);

	return 0;
}

void main(const struct soc_data *sd, const struct board_data *bd)
{
	struct board_data bd_dup;
	int ret;

	uart_soc_init(sd, bd);

	/* The console is ready now; print welcome message */
	pr_info("\nUniPhier BL version " VERSION " (");
	if (git_head[0])
		pr_info("git: %s, ", git_head);
	pr_info("built: %s)\n", time_stamp);

	if (bd->board_name)
		pr_info("Board: %s\n", bd->board_name);

	ret = soc_id_check(sd);
	if (ret)
		goto die;

	/* Use a non-const copy to modify */
	bd_dup = *bd;

	dram_param_check(sd, &bd_dup);

	ret = dram_init(sd, &bd_dup);
	if (ret)
		goto die;

	ret = uncompress_piggy();
	if (ret)
		goto die;

	pr_info("Jumping to next image (0x%x)\n", CONFIG_NEXT_IMAGE_BASE);
	next_entry(CONFIG_NEXT_IMAGE_BASE);

die:
	while (1)
		;
}
