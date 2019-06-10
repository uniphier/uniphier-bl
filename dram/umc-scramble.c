// SPDX-License-Identifier: GPL-2.0-only
//
// Copyright (C) 2018 Socionext Inc.

#include <bits.h>
#include <compiler.h>
#include <io.h>

#define UMC_SEC_EN		0x000
#define UMC_SEC_AREA7_EN	0x100

void umc_scramble_init(void __iomem *sec_base)
{
	/*
	 * For LD11, at least one arbitrary bit of UMC_SEC_EN must be set
	 * to make UMC_SEC_AREA7_EN effective.
	 */
	writel(BIT(5), sec_base + UMC_SEC_EN);
	writel(BIT(0), sec_base + UMC_SEC_AREA7_EN);
}
