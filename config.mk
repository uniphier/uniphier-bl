# SPDX-License-Identifier: GPL-2.0-only

# uncomment for debug
#CONFIG_DEBUG=y

# printk loglevel
ifdef CONFIG_DEBUG
CONFIG_LOGLEVEL=7
else
CONFIG_LOGLEVEL=6
endif

# Build with clang, llvm assember, linker.
#CONFIG_LLVM=y

# baudrate of serial console
CONFIG_UART_BAUDRATE=115200

# enable the SoC support
CONFIG_SOC_LD11=y
CONFIG_SOC_LD20=y
CONFIG_SOC_PXS3=y
CONFIG_SOC_NX1=y

# enable the DRAM scramble feature in UMC
#CONFIG_DRAM_SCRAMBLE=y

# Support a full set of format specifies for formatted print functions,
# such as printk(), pr_*(), s*printf(), and vs*printf().
#
# The following are supported by default:
#  '0', '-', 'l', 'll', 'L', 'z', 'd', 'i', 'o', 'u', 'x', 'X', 'c', 's',
#  'p', '%'
#
# In addition, the following are supported by enabling this option.
#  '#', ' ', '+', '*', '.', 'h', 't', 'n'
#
#CONFIG_PRINTF_FULL=y

# Produce position independent executables. CONFIG_TEXT_BASE still specifies
# the link address, but you can load the binaries anywhere.
CONFIG_PIE=y

# no need to change
CONFIG_TEXT_BASE=0x00000000
CONFIG_BSS_OFFSET=0x0000ff00
CONFIG_MAX_SIZE=0x00010000

# offset from the DRAM base to the entry address of the next image
CONFIG_NEXT_IMAGE_OFFSET=0x00000000

# disable time-stamp and git hash for reproducible build
#CONFIG_REPRODUCIBLE_BUILD=y

# Include optional user-config.mk if exists, so you can tweak configuration
-include user-config.mk
