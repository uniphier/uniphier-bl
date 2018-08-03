/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Based on include/uapi/linux/byteorder/big_endian.h, Linux 4.10
 */

#ifndef __BYTEORDER_BIG_ENDIAN_H__
#define __BYTEORDER_BIG_ENDIAN_H__

#include <compiler.h>
#include <swab.h>
#include <types.h>

#define cpu_to_le16(x) ((__force le16)swab16((x)))
#define le16_to_cpu(x) swab16((__force u16)(le16)(x))
#define cpu_to_le32(x) ((__force le32)swab32((x)))
#define le32_to_cpu(x) swab32((__force u32)(le32)(x))
#define cpu_to_le64(x) ((__force le64)swab64((x)))
#define le64_to_cpu(x) swab64((__force u64)(le64)(x))

#define cpu_to_be16(x) ((__force be16)(u16)(x))
#define be16_to_cpu(x) ((__force u16)(be16)(x))
#define cpu_to_be32(x) ((__force be32)(u32)(x))
#define be32_to_cpu(x) ((__force u32)(be32)(x))
#define cpu_to_be64(x) ((__force be64)(u64)(x))
#define be64_to_cpu(x) ((__force u64)(be64)(x))

#endif /* __BYTEORDER_BIG_ENDIAN_H__ */




