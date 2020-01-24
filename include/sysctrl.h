/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __SYSCTRL_H__
#define __SYSCTRL_H__

void rst_deassert(const struct soc_data *sd, unsigned int id);
void rst_assert(const struct soc_data *sd, unsigned int id);
void clk_enable(const struct soc_data *sd, unsigned int id);
void dpll_init(const struct board_data *bd);
void pll_set_freq(const struct soc_data *sd, int pll_id,
		  unsigned int freq, unsigned int divn);

#endif /* __SYSCTRL_H__ */
