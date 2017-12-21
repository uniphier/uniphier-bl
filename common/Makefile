obj-y			+= cache.o
obj-y			+= clk.o
obj-$(CONFIG_DEBUG)	+= debug.o
obj-y			+= head.o
obj-y			+= heap.o
obj-y			+= main.o
obj-y			+= mbr.o
obj-y			+= next-entry.o
obj-y			+= pinctrl.o
obj-y			+= pll.o
obj-y			+= printk.o
obj-y			+= soc-info.o
obj-y			+= timer.o
obj-y			+= uart.o

extra-y			+= uniphier.lds

# for <stdarg.h>
CFLAGS_REMOVE_printk.o	:= -nostdinc

# time stamp
quiet_cmd_timestamp = GEN     $@
define cmd_timestamp
	( \
	echo \#define TIME_STAMP \"$$(LC_ALL=C date)\"; \
	echo \#define GIT_HEAD \"$$(cd $(srctree); git describe --always --dirty --tags 2> /dev/null)\"; \
	) > $@
endef

$(obj)/timestamp-inc.h: FORCE
	$(call cmd,timestamp)

ifeq ($(CONFIG_REPRODUCIBLE_BUILD),y)
CFLAGS_timestamp.o = -include $(src)/timestamp-null.h
else
$(obj)/timestamp.o: $(obj)/timestamp-inc.h
CFLAGS_timestamp.o = -include $(obj)/timestamp-inc.h
endif

clean-files := timestamp-inc.h
