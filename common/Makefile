obj-y			+= cache.o
obj-y			+= clk.o
obj-$(CONFIG_DEBUG)	+= debug.o
obj-y			+= head.o
obj-y			+= main.o
obj-y			+= next-entry.o
obj-y			+= pinctrl.o
obj-y			+= printk.o
obj-y			+= soc-info.o
obj-y			+= uart.o

extra-y			+= uniphier.lds

# time stamp
quiet_cmd_timestamp = GEN     $@
define cmd_timestamp
	( \
	echo \#define TIME_STAMP \"$$(LC_ALL=C date)\"; \
	echo \#define GIT_HEAD \"$$(git describe --always --dirty --tags 2> /dev/null)\"; \
	) > $@
endef

$(obj)/timestamp-inc.h: FORCE
	$(call cmd,timestamp)

$(obj)/timestamp.o: $(obj)/timestamp-inc.h
CFLAGS_timestamp.o = -include $(obj)/timestamp-inc.h

clean-files := timestamp-inc.h
