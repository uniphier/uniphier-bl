VERSION := 1.0

# Check make version
MIN_MAKE_VERSION = 3.81
ifneq ($(firstword $(sort $(MAKE_VERSION) $(MIN_MAKE_VERSION))),$(MIN_MAKE_VERSION))
$(error GNU Make >= $(MIN_MAKE_VERSION) is required.  You are running version $(MAKE_VERSION))
endif

PHONY += all
all:

# Do not use make's built-in rules and variables; this increases performance
# and avoids hard-to-debug behavior;
MAKEFLAGS += -rR

ifeq ("$(origin V)", "command line")
  UNPH_VERBOSE = $V
endif
ifndef UNPH_VERBOSE
  UNPH_VERBOSE = 0
endif

ifneq ($(findstring s,$(filter-out --%,$(MAKEFLAGS))),)
  quiet := silent_
  Q = @
else ifneq ($(UNPH_VERBOSE),1)
  quiet := quiet_
  Q = @
else
  quiet :=
  Q =
endif

export quiet Q

SRCDIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

ifeq ("$(origin O)", "command line")
  UNPH_OUTPUT := $O
endif

ifneq ($(UNPH_OUTPUT),)
saved-output := $(UNPH_OUTPUT)
$(shell mkdir -p $(UNPH_OUTPUT))
UNPH_OUTPUT := $(realpath $(UNPH_OUTPUT))
$(if $(UNPH_OUTPUT),,$(error cannot create output directory "$(saved-output)"))
endif

ifneq ($(filter-out $(CURDIR),$(UNPH_OUTPUT)),)

PHONY += $(MAKECMDGOALS)
$(filter-out all, $(MAKECMDGOALS)): all
all:
	$(Q)$(MAKE) -C $(UNPH_OUTPUT) O=$(UNPH_OUTPUT) \
	-f $(SRCDIR)/Makefile $(MAKECMDGOALS)

else # ($(filter-out $(CURDIR),$(UNPH_OUTPUT)),)

# Do not print "Entering directory ...",
# but we want to display it when entering to the output directory
# so that IDEs/editors are able to understand relative filenames.
MAKEFLAGS += --no-print-directory

ifeq ("$(origin C)", "command line")
  UNPH_CHECKSRC = $(C)
endif
ifndef UNPH_CHECKSRC
  UNPH_CHECKSRC = 0
endif
export UNPH_CHECKSRC

ifeq ($(SRCDIR),$(CURDIR))
srctree := .
else
$(shell ln -fns $(SRCDIR) srctree)
srctree := srctree
endif

export srctree

include $(srctree)/config.mk

CROSS_COMPILE	?= aarch64-linux-gnu-

AR		:= $(CROSS_COMPILE)ar
AS		:= $(CROSS_COMPILE)as
CC		:= $(CROSS_COMPILE)gcc
CPP		:= $(CC) -E
LD		:= $(CROSS_COMPILE)ld
OBJCOPY		:= $(CROSS_COMPILE)objcopy
CHECK		:= sparse

UNPH_CPPFLAGS	:= -include include/generated/config.h \
		   -Iinclude -I$(srctree)/include \
		   -mlittle-endian -fdata-sections -ffunction-sections
UNPH_CFLAGS	:= -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs \
		   -fno-common -Werror-implicit-function-declaration \
		   -Werror=date-time \
		   -std=gnu89 -ffreestanding -Os -mgeneral-regs-only \
		   -mstrict-align
UNPH_AFLAGS	:= -D__ASSEMBLY__
OBJCOPYFLAGS	:= -O binary -R .note -R .note.gnu.build-id -R .comment -S
CHECKFLAGS	:= -Wbitwise -Wno-return-void -Wcast-to-as

export CROSS_COMPILE AR AS CC CPP LD CHECK
export UNPH_CPPFLAGS UNPH_CFLAGS UNPH_AFLAGS CHECKFLAGS

include $(srctree)/scripts/common.mk

define filechk_config
	($(foreach v, $(filter CONFIG_%, $(.VARIABLES)), \
	 $(if $($v), echo \#define $v $(if $(filter y,$($v)),1,'$($v)');)))
endef

include/generated/config.h: $(srctree)/config.mk FORCE
	$(call filechk,config)

define filechk_version
	echo \#define VERSION \"$(VERSION)\"
endef

include/generated/version.h: $(srctree)/Makefile FORCE
	$(call filechk,version)

PHONY += prepare
prepare: include/generated/config.h include/generated/version.h

dir-y		+= boards
dir-y		+= common
dir-y		+= dram
dir-y		+= lib

board-$(CONFIG_SOC_LD11)	+= ld11_ref ld11_global
board-$(CONFIG_SOC_LD20)	+= ld20_ref ld20_global \
				   ld21_ref ld21_global

timestamp := common/timestamp.o
lds := common/uniphier.lds
objs := $(addsuffix /link.o, $(dir-y))
elfs := $(patsubst %,bl_%.elf, $(board-y))
bins := $(patsubst %,bl_%.bin, $(board-y))
have-cmd-files := $(elfs) $(bins)
all: $(bins)
	@:

quiet_cmd_link = LD      $@
      cmd_link = $(LD) $(LDFLAGS) --gc-sections -o $@ \
	-e $(patsubst bl_%.elf,entry_%,$@) -T $(lds) \
	$(timestamp) --whole-archive $(objs)

quiet_cmd_objcopy = OBJCOPY $@
      cmd_objcopy = $(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(bins): %.bin: %.elf FORCE
	$(call if_changed,objcopy)

$(elfs): $(objs) $(timestamp) $(lds) FORCE
	$(call if_changed,link)

$(timestamp): $(objs) $(lds)
	$(Q)$(MAKE) $(build)=$(@D) $@

$(objs): %/link.o: % ;
$(lds): $(patsubst %/,%,$(dir $(lds))) ;

PHONY += $(dir-y)
$(dir-y): prepare
	$(Q)$(MAKE) $(build)=$@

FIND_IGNORE := -name .git -prune -o

quiet_cmd_clean = $(if $(wildcard $($2)),CLEAN   $(wildcard $($2)))
      cmd_clean = rm -rf $($2)

clean-files := $(wildcard include/generated)
clean-dirs := $(addprefix _clean_,$(dir-y) $(dir-))
PHONY += $(clean-dirs)
$(clean-dirs):
	$(Q)$(MAKE) $(clean)=$(patsubst _clean_%,%,$@)
PHONY += clean
clean: $(clean-dirs)
	$(call cmd,clean,clean-files)
	@find . $(FIND_IGNORE) \
		\( -name '*.[aios]' -o -name '*.elf' -o -name '*.bin' \
		-o -name '.*.cmd' -o -name '.*.d' \) \
		-type f -print | xargs rm -f

distclean-files := tags TAGS cscope* GPATH GTAGS GRTAGS GSYMS

PHONY += distclean
distclean: clean
	$(call cmd,clean,distclean-files)
	@find . $(FIND_IGNORE) \
		\( -name '*.orig' -o -name '*.rej' -o -name '*~' \
		-o -name '*.bak' -o -name '#*#' \) \
		-type f -print | xargs rm -f

PHONY += help
help:
	@echo "Generic targets:"
	@echo "  all       - Build all board images"
	@echo "  clean     - Remove all build artifacts"
	@echo "  distclean - clean + remove editor back, patch, tag files"
ifneq ($(bins),)
	@echo ""
	@echo "Individual board targets:"
	@$(foreach b, $(bins), echo "  $b";)
endif

# Single targets
single-targets := $(filter-out $(objs), $(filter %.i %o %s, $(MAKECMDGOALS)))
$(single-targets): prepare FORCE
	$(Q)$(MAKE) $(build)=$(patsubst %/,%, $(dir $@)) $@

cmd_files := $(wildcard $(foreach f,$(have-cmd-files),$(dir $f).$(notdir $f).cmd))
ifneq ($(cmd_files),)
  include $(cmd_files)
endif

endif # ($(filter-out $(CURDIR),$(UNPH_OUTPUT)),)

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
