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

# no need to change
CONFIG_TEXT_BASE=0x30000000
CONFIG_BSS_BASE=0x30010000
CONFIG_STACK_BASE=0x30014000

# base (entry) address of the next image
CONFIG_NEXT_IMAGE_BASE=0x80000000

# Include optional user-config.mk if exists, so you can tweak configuration
-include user-config.mk
