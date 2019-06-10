// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <io.h>
#include <soc-info.h>

#define SG_REVISION		(IOMEM(0x5f800000))
#define   SG_REVISION_TYPE_SHIFT	16
#define   SG_REVISION_TYPE_MASK		(0xff << SG_REVISION_TYPE_SHIFT)

unsigned int get_soc_id(void)
{
	u32 rev = readl(SG_REVISION);

	return (rev & SG_REVISION_TYPE_MASK) >> SG_REVISION_TYPE_SHIFT;
}
