/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __PRINTK_H__
#define __PRINTK_H__

#include <compiler.h>

#ifndef pr_fmt
#define pr_fmt(fmt) fmt
#endif

void __printf(1, 2) printk(const char *fmt, ...);

#define __printk(level, format, ...)			\
	do {						\
		if ((level) <= CONFIG_LOGLEVEL)		\
			printk(format, ##__VA_ARGS__);	\
	} while (0)

#define pr_emerg(fmt, ...)	__printk(0, pr_fmt(fmt), ##__VA_ARGS__)
#define pr_alert(fmt, ...)	__printk(1, pr_fmt(fmt), ##__VA_ARGS__)
#define pr_crit(fmt, ...)	__printk(2, pr_fmt(fmt), ##__VA_ARGS__)
#define pr_err(fmt, ...)	__printk(3, pr_fmt(fmt), ##__VA_ARGS__)
#define pr_warn(fmt, ...)	__printk(4, pr_fmt(fmt), ##__VA_ARGS__)
#define pr_notice(fmt, ...)	__printk(5, pr_fmt(fmt), ##__VA_ARGS__)
#define pr_info(fmt, ...)	__printk(6, pr_fmt(fmt), ##__VA_ARGS__)
#define pr_debug(fmt, ...)	__printk(7, pr_fmt(fmt), ##__VA_ARGS__)

#endif /* __PRINTK_H__ */
