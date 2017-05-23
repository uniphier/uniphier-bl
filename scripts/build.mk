src := $(srctree)/$(obj)

PHONY := __build
__build:

subdir-y	:=
obj-y		:=

include $(srctree)/scripts/common.mk
include $(srctree)/config.mk
include $(src)/Makefile

subdir-y	+= $(patsubst %/,%,$(filter %/, $(obj-y)))
obj-y		:= $(patsubst %/,%/link.o.txt, $(obj-y))

extra-y		:= $(strip $(addprefix $(obj)/,$(extra-y)))
subdir-y	:= $(strip $(addprefix $(obj)/,$(subdir-y)))
obj-y		:= $(strip $(addprefix $(obj)/,$(obj-y)))
subdir-obj-y	:= $(filter %/link.o.txt, $(obj-y))
objlist		:= $(obj)/link.o.txt
have-cmd-files	:= $(obj-y) $(extra-y) $(objlist)

ifneq ($(srctree),.)
$(shell mkdir -p $(sort $(patsubst %/,%,$(obj) $(dir $(obj-y) $(extra-y)))))
endif

__build:  $(if $(strip $(obj-y) $(obj-)),$(objlist)) $(extra-y) $(subdir-y)
	@:

dep_flags	= -Wp,-MD,$(depfile),-MP -MT $@
c_flags		= $(dep_flags) $(UNPH_CPPFLAGS) $(UNPH_CFLAGS) \
		  $(CFLAGS_$(basetarget).o)
a_flags		= $(dep_flags) $(UNPH_CPPFLAGS) $(UNPH_AFLAGS) \
		  $(AFLAGS_$(basetarget).o)
cpp_flags	= $(dep_flags) $(UNPH_CPPFLAGS) $(UNPH_CPPFLAGS) \
		  $(CPPFLAGS_$($F))

quiet_cmd_checksrc = CHECK   $<
      cmd_checksrc = $(CHECK) $(CHECKFLAGS) $(c_flags) $<

quiet_cmd_cc_o_c = CC      $@
      cmd_cc_o_c = $(CC) $(c_flags) -c -o $@ $<

$(obj)/%.o: $(src)/%.c FORCE
ifeq ($(UNPH_CHECKSRC),1)
	$(call cmd,checksrc)
endif
	$(call if_changed_dep,cc_o_c)

quiet_cmd_as_o_S = AS      $@
      cmd_as_o_S = $(CC) $(a_flags) -c -o $@ $<

$(obj)/%.o: $(src)/%.S FORCE
	$(call if_changed_dep,as_o_S)

# Linker scripts preprocessor (.lds.S -> .lds)
# ---------------------------------------------------------------------------
quiet_cmd_cpp_lds_S = CPP     $@
      cmd_cpp_lds_S = $(CPP) $(cpp_flags) -P -C \
			-D__ASSEMBLY__ -DLINKER_SCRIPT -o $@ $<

$(obj)/%.lds: $(src)/%.lds.S FORCE
	$(call if_changed_dep,cpp_lds_S)

# Rule to generate a list of objects
# ---------------------------------------------------------------------------
quiet_cmd_gen_objlist = GEN     $@
      cmd_gen_objlist = ($(foreach o, $(filter-out $(PHONY), $^), \
			$(if $(filter %/link.o.txt, $o), cat $o, echo $o);)) > $@

$(objlist): $(obj-y) FORCE
	$(call if_changed,gen_objlist)

$(sort $(subdir-obj-y)): $(subdir-y) ;

# Descending
# ---------------------------------------------------------------------------
PHONY += $(subdir-y)
$(subdir-y):
	$(Q)$(MAKE) $(build)=$@

PHONY += FORCE

FORCE:
cmd_files := $(wildcard $(foreach f,$(have-cmd-files),$(dir $(f)).$(notdir $(f)).cmd))
ifneq ($(cmd_files),)
  include $(cmd_files)
endif

PHONY += FORCE
FORCE:

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable se we can use it in if_changed and friends.
.PHONY: $(PHONY)
