/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <stdarg.h>

#include <printk.h>
#include <uart.h>

static void __putc(char c)
{
	uart_putc(c);
	if (c == '\n')
		uart_putc('\r');
}

static void __puts(const char *s)
{
	char c;

	while ((c = *s++))
		__putc(c);
}

#define get_num_va_args(args, lcount) \
	((lcount) > 1 ? va_arg(args, long long) :	\
	 ((lcount) ? va_arg(args, long) : va_arg(args, int)))

#define get_unum_va_args(args, lcount) \
	((lcount) > 1 ? va_arg(args, unsigned long long) :	\
	 ((lcount) ? va_arg(args, unsigned long) : va_arg(args, unsigned int)))

static void unsigned_num_print(unsigned long long unum, unsigned int radix)
{
	unsigned char buf[32];
	int i = 0, rem;

	do {
		rem = unum % radix;
		buf[i++] = rem > 9 ? rem - 0xa + 'a' : rem + '0';
	} while (unum /= radix);

	while (--i >= 0)
		__putc(buf[i]);
}

static void vprintk(const char *fmt, va_list args)
{
	int l_count;
	long long num;
	unsigned long long unum;
	char *str;
	char c;

	while ((c = *fmt++)) {
		l_count = 0;

		if (c != '%') {
			__putc(c);
			continue;
		}

		c = *fmt++;

		while (c == 'l') {
			l_count++;
			c = *fmt++;
		}

		if (c == 'z')
			l_count = 1;

		/* Check the format specifier */
		switch (c) {
		case '%':
			__putc('%');
			break;
		case 'i':
		case 'd':
			num = get_num_va_args(args, l_count);
			if (num < 0) {
				__putc('-');
				unum = -num;
			} else
				unum = num;

			unsigned_num_print(unum, 10);
			break;
		case 's':
			str = va_arg(args, char *);
			__puts(str);
			break;
		case 'p':
			unum = (unsigned long)va_arg(args, void *);
			if (unum)
				__puts("0x");

			unsigned_num_print(unum, 16);
			break;
		case 'x':
			unum = get_unum_va_args(args, l_count);
			unsigned_num_print(unum, 16);
			break;
		case 'u':
			unum = get_unum_va_args(args, l_count);
			unsigned_num_print(unum, 10);
			break;
		default:
			break;
		}
	}
}

void printk(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
