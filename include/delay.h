/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef __DELAY_H__
#define __DELAY_H__

void udelay(unsigned long usecs);
#define mdelay(msecs)	udelay((msecs) * 1000)

#endif /* __DELAY_H__ */
