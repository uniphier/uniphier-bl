/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CACHE_H__
#define __CACHE_H__

/**
 * flush_dcache_range - flush data or unified cache by VA to PoC
 * @start: virtual start address, inclusive
 * @end: virtual end address, exclusive
 */
void flush_dcache_range(unsigned long start, unsigned long end);

#endif /* __CACHE_H__ */
