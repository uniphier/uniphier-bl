// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <io.h>
#include <soc-data.h>
#include <uart.h>
#include <utils.h>

#define UART_OFFSET	0x100

#define UART_TX		0x00	/* Out: Transmit buffer */
#define UART_LCR_MCR	0x10	/* Line/Modem Control Register */
#define   UART_LCR_WLEN8	0x03	/* Wordlength: 8 bits */
#define UART_LSR	0x14	/* Line Status Register */
#define   UART_LSR_THRE		0x20	/* Transmit-hold-register empty */
#define UART_DLR	0x24	/* Divisor Latch Register */

static void __iomem *uart_base;

void uart_init(const struct soc_data *sd, unsigned int port)
{
	unsigned int div = DIV_ROUND_CLOSEST(sd->uart_clk_rate,
					     16 * CONFIG_UART_BAUDRATE);

	uart_base = sd->uart_base + UART_OFFSET * port;

	writel(UART_LCR_WLEN8 << 8, uart_base + UART_LCR_MCR);
	writel(div, uart_base + UART_DLR);
}

void uart_putc(char c)
{
	while (!(readl(uart_base + UART_LSR) & UART_LSR_THRE))
		;

	writel(c, uart_base + UART_TX);
}
