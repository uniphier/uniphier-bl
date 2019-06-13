/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __SOC_INFO_H__
#define __SOC_INFO_H__

struct soc_data;

unsigned int get_soc_id(const struct soc_data *sd);

#endif /* __SOC_INFO_H__ */
