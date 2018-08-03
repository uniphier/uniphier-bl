# SPDX-License-Identifier: GPL-2.0

src := $(srctree)/$(obj)

PHONY := __clean
__clean:

include $(srctree)/scripts/common.mk
include $(src)/Makefile

__subdirs	:= $(subdir-y) $(subdir-) \
		   $(patsubst %/,%,$(filter %/, $(obj-y) $(obj-)))
__subdirs	:= $(addprefix $(obj)/,$(sort $(__subdirs)))

__clean-files	:= $(extra-y) $(extra-) $(targets) $(clean-files)
__clean-files	:= $(wildcard $(addprefix $(obj)/, $(__clean-files)))

quiet_cmd_clean    = CLEAN   $(__clean-files)
      cmd_clean    = rm -f $(__clean-files)

__clean: $(__subdirs)
ifneq ($(strip $(__clean-files)),)
	+$(call cmd,clean)
endif
	@:

PHONY += $(__subdirs)
$(__subdirs):
	$(Q)$(MAKE) $(clean)=$@

.PHONY: $(PHONY)
