# SPDX-License-Identifier: GPL-2.0

# uncomment for debug
#CONFIG_DEBUG=y

# printk loglevel
ifdef CONFIG_DEBUG
CONFIG_LOGLEVEL=7
else
CONFIG_LOGLEVEL=6
endif

# baudrate of serial console
CONFIG_UART_BAUDRATE=115200

# enable the SoC support
CONFIG_SOC_LD11=y
CONFIG_SOC_LD20=y
CONFIG_SOC_PXS3=y

# choose malloc implementation
#  malloc() is needed for zlib.  The simple malloc should be enough for our
#  purpose.  You can play with dlmalloc if you like, though.
CONFIG_MALLOC_SIMPLE=y
#CONFIG_MALLOC_DL=y

# no need to change
CONFIG_TEXT_BASE=0x30000000
CONFIG_BSS_BASE=0x3000f800
CONFIG_STACK_BASE=0x30010000

# base (entry) address of the next image
CONFIG_NEXT_IMAGE_BASE=0x80000000

# disable time-stamp and git hash for reproducible build
#CONFIG_REPRODUCIBLE_BUILD=y

# Include optional user-config.mk if exists, so you can tweak configuration
-include user-config.mk
