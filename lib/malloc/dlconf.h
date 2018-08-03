/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2017 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 */

#ifndef __DLCONF_H__
#define __DLCONF_H__

#include <bug.h>
#include <heap.h>
#include <malloc.h>
#include <types.h>

#define ABORT			BUG()
#define MALLOC_FAILURE_ACTION	do {} while(0)

#define HAVE_MMAP		0
#define HAVE_MREMAP		0
#define NO_MALLOC_STATS		1

#define LACKS_TIME_H
#define LACKS_SCHED_H
#define LACKS_STDLIB_H
#define LACKS_UNISTD_H
#define LACKS_SYS_PARAM_H
#define LACKS_SYS_TYPES_H

#define DEFAULT_GRANULARITY	0x400

#endif /* __DLCONF_H__ */
