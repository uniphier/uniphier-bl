// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <printk.h>
#include <sprintf.h>
#include <stdarg.h>
#include <uart.h>

static void __putc(char c)
{
	if (c == '\n')
		uart_putc('\r');
	uart_putc(c);
}

static void __puts(const char *s)
{
	char c;

	while ((c = *s++))
		__putc(c);
}

void printk(const char *fmt, ...)
{
	va_list args;
	char buf[128];

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	__puts(buf);
	va_end(args);
}
