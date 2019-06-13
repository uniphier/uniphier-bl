/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __UART_H__
#define __UART_H__

struct soc_data;

void uart_init(const struct soc_data *sd, unsigned int port);
void uart_putc(char c);

#endif /* __UART_H__ */
