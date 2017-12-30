/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __INIT_H__
#define __INIT_H__

#include <bitops.h>
#include <compiler.h>

#define MAX_NR_DRAM_CH		3

struct dram_ch {
	unsigned long size;
	unsigned int width;
};

struct board_data {
	const char *board_name;
	unsigned int uart_port;
	unsigned int dram_freq;
	struct dram_ch dram_ch[MAX_NR_DRAM_CH];
	unsigned int flags;

/* board type */
#define BD_BOARD_GET_TYPE(f)		((f) & 0x7)
#define BD_BOARD_LD20_REF		0	/* LD20 reference */
#define BD_BOARD_LD20_GLOBAL		1	/* LD20 TV Set */
#define BD_BOARD_LD20_C1		2	/* LD20 TV Set C1 */
#define BD_BOARD_LD21_REF		3	/* LD21 reference */
#define BD_BOARD_LD21_GLOBAL		4	/* LD21 TV Set */

/* DRAM layout */
#define BD_DRAM_SPARSE			BIT(8)

/* DRAM PLL */
#define BD_DPLL_SSC_CHANGE_RATE		BIT(9)
#define BD_DPLL_SSC_GET_RATE(f)		(((f) >> 10) & 0x7)

#define BD_DPLL_SSC_RATE_0		((0 << 10) | BD_DPLL_SSC_CHANGE_RATE)
#define BD_DPLL_SSC_RATE_1		((1 << 10) | BD_DPLL_SSC_CHANGE_RATE)
#define BD_DPLL_SSC_RATE_2		((2 << 10) | BD_DPLL_SSC_CHANGE_RATE)
#define BD_DPLL_SSC_RATE_3		((3 << 10) | BD_DPLL_SSC_CHANGE_RATE)
};

#define MAX_NR_UART_PORTS	4

struct uart_pinmux {
	unsigned int pin;
	unsigned int mux;
};

struct soc_data {
	unsigned int soc_id;
	unsigned int uart_clk_bits;
	unsigned int uart_clk_rate;
	const struct uart_pinmux uart_pinmux[MAX_NR_UART_PORTS];
	unsigned long heap_start;
	unsigned long heap_end;
	unsigned int timer_clk_rate;
	unsigned int dram_rst_bits;
	unsigned int dram_clk_bits;
	unsigned int dram_default_freq;
	unsigned int dram_default_width[MAX_NR_DRAM_CH];
	int dram_have_ch2;
	int dpll[MAX_NR_DRAM_CH];
	int (*umc_init)(const struct board_data *); /* DRAM controller init */
	int (*soc_init)(const struct board_data *); /* SoC-specific init */
};

void __noreturn main(const struct soc_data *sd, const struct board_data *bd);

int timer_init(unsigned int clk_rate);

void dpll_init(const struct soc_data *sd, const struct board_data *bd);
void pll_set_freq(int pll_id, unsigned int freq, unsigned int divn);

void clk_enable_uart(unsigned int clk_bits);
void clk_enable_dram(unsigned int clk_bits, unsigned int rst_bits);

void pinctrl_set_mux(unsigned int pin, unsigned int mux);

int memconf_init(const struct board_data *bd, int have_ch2);

int ld11_umc_init(const struct board_data *bd);
int ld20_umc_init(const struct board_data *bd);

void __noreturn next_entry(unsigned long entry_addr);

#endif /* __INIT_H__ */
