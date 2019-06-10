/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2018 Socionext Inc.
 */

#include <compiler.h>

#ifdef CONFIG_DRAM_SCRAMBLE
void umc_scramble_init(void __iomem *sec_base);
#else
static inline void umc_scramble_init(void __iomem *sec_base)
{
}
#endif
