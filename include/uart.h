/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __UART_H__
#define __UART_H__

void uart_init(unsigned int port, unsigned int clk_rate);
void uart_putc(char c);

#endif /* __UART_H__ */
