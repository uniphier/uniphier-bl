# SPDX-License-Identifier: GPL-2.0

# stupid implemenation (memory is never freed)
obj-$(CONFIG_MALLOC_SIMPLE)	+= simple.o

# dlmalloc (Doug Lea's memory allocator)
obj-$(CONFIG_MALLOC_DL)		+= dlmalloc.o
CFLAGS_dlmalloc.o := -include $(src)/dlconf.h -Wno-strict-prototypes
