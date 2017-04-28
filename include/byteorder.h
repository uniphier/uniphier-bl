/*
 * Based on arch/arm64/include/uapi/asm/byteorder.h, Linux 4.10
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __BYTEORDER_H__
#define __BYTEORDER_H__

#ifdef __AARCH64EB__
#include <byteorder/big_endian.h>
#else
#include <byteorder/little_endian.h>
#endif

#endif /* __BYTEORDER_H__ */
