// SPDX-License-Identifier: GPL-2.0-or-later
//
// Copyright (C) 2017 Socionext Inc.
//   Author: Masahiro Yamada <yamada.masahiro@socionext.com>

#include <io.h>
#include <pinctrl.h>
#include <soc-data.h>

#define SG_PINCTRL_PINMUX_BASE	0x1000
#define SG_PINCTRL_IECTRL_BASE	0x1d00

static void pinctrl_update_field(void __iomem *base, int field_width,
				 int pin, int val)
{
	u32 mask, tmp;
	void __iomem *reg;
	int shift;

	reg = base + pin * field_width / 32 * 4;
	shift = pin * field_width % 32;
	mask = (1 << field_width) - 1;

	tmp = readl(reg);
	tmp &= ~(mask << shift);
	tmp |= (mask & val) << shift;
	writel(tmp, reg);
}

void pinctrl_set_mux(const struct soc_data *sd, const struct pinmux *mux)
{
	/* mux */
	pinctrl_update_field(sd->socglue_base + SG_PINCTRL_PINMUX_BASE,
			     8, mux->pin, mux->mux);

	/* enable input */
	pinctrl_update_field(sd->socglue_base + SG_PINCTRL_IECTRL_BASE,
			     1, mux->pin, 1);
}
