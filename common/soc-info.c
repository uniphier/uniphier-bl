// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <bits.h>
#include <bitfield.h>
#include <io.h>
#include <soc-info.h>

#define SG_REVISION		(IOMEM(0x5f800000))
#define   SG_REVISION_TYPE_MASK		GENMASK(23, 16)

unsigned int get_soc_id(void)
{
	u32 rev = readl(SG_REVISION);

	return FIELD_GET(SG_REVISION_TYPE_MASK, rev);
}
