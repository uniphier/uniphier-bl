src := $(srctree)/$(obj)

PHONY := __build
__build:

subdir-y	:=
obj-y		:=

include $(srctree)/scripts/common.mk
include $(srctree)/config.mk
include $(src)/Makefile

subdir-y	+= $(patsubst %/,%,$(filter %/, $(obj-y)))
obj-y		:= $(patsubst %/,%/link.o, $(obj-y))

extra-y		:= $(strip $(addprefix $(obj)/,$(extra-y)))
subdir-y	:= $(strip $(addprefix $(obj)/,$(subdir-y)))
obj-y		:= $(strip $(addprefix $(obj)/,$(obj-y)))
subdir-obj-y	:= $(filter %/link.o, $(obj-y))
link-target	:= $(obj)/link.o
have-cmd-files	:= $(obj-y) $(extra-y) $(link-target)

ifneq ($(srctree),.)
$(shell mkdir -p $(sort $(patsubst %/,%,$(obj) $(dir $(obj-y) $(extra-y)))))
endif

__build: $(if $(strip $(obj-y) $(obj-)),$(link-target)) $(extra-y) $(subdir-y)
	@:

dep_flags	= -Wp,-MD,$(depfile),-MP -MT $@
c_flags		= $(dep_flags) $(UNPH_CPPFLAGS) $(UNPH_CFLAGS) \
		  $(CFLAGS_$(basetarget).o)
a_flags		= $(dep_flags) $(UNPH_CPPFLAGS) $(UNPH_AFLAGS) \
		  $(AFLAGS_$(basetarget).o)
cpp_flags	= $(dep_flags) $(UNPH_CPPFLAGS) $(UNPH_CPPFLAGS) \
		  $(CPPFLAGS_$($F))

ifeq ($(UNPH_CHECKSRC),1)
quiet_cmd_checksrc = CHECK   $<
      cmd_checksrc = $(CHECK) $(CHECKFLAGS) $(c_flags) $<
endif

quiet_cmd_cpp_i_c = CPP     $@
      cmd_cpp_i_c = $(CPP) $(c_flags) -o $@ $<

$(obj)/%.i: $(src)/%.c FORCE
	$(call cmd,checksrc)
	$(call if_changed_dep,cpp_i_c)

quiet_cmd_cpp_s_S = CPP     $@
      cmd_cpp_s_S = $(CPP) $(a_flags) -o $@ $<

$(obj)/%.s: $(src)/%.S FORCE
	$(call if_changed_dep,cpp_s_S)

quiet_cmd_cc_s_c = CC      $@
      cmd_cc_s_c = $(CC) $(c_flags) -S -o $@ $<

$(obj)/%.s: $(src)/%.c FORCE
	$(call cmd,checksrc)
	$(call if_changed_dep,cc_s_c)

quiet_cmd_cc_o_c = CC      $@
      cmd_cc_o_c = $(CC) $(c_flags) -c -o $@ $<

$(obj)/%.o: $(src)/%.c FORCE
	$(call cmd,checksrc)
	$(call if_changed_dep,cc_o_c)

quiet_cmd_as_o_S = AS      $@
      cmd_as_o_S = $(CC) $(a_flags) -c -o $@ $<

$(obj)/%.o: $(src)/%.S FORCE
	$(call if_changed_dep,as_o_S)

# Linker scripts preprocessor (.lds.S -> .lds)
# ---------------------------------------------------------------------------
quiet_cmd_cpp_lds_S = CPP     $@
      cmd_cpp_lds_S = $(CPP) $(cpp_flags) -P \
			-D__ASSEMBLY__ -DLINKER_SCRIPT -o $@ $<

$(obj)/%.lds: $(src)/%.lds.S FORCE
	$(call if_changed_dep,cpp_lds_S)

# Rule to generate an intermediate .o file
# ---------------------------------------------------------------------------
quiet_cmd_ar_link_o = AR      $@
      cmd_ar_link_o = rm -f $@; $(AR) rcPST $@ $(obj-y)

$(link-target): $(obj-y) FORCE
	$(call if_changed,ar_link_o)

$(sort $(subdir-obj-y)): $(subdir-y) ;

# Descending
# ---------------------------------------------------------------------------
PHONY += $(subdir-y)
$(subdir-y):
	$(Q)$(MAKE) $(build)=$@

PHONY += FORCE

FORCE:
cmd_files := $(wildcard $(foreach f,$(have-cmd-files) $(MAKECMDGOALS), \
					$(dir $(f)).$(notdir $(f)).cmd))
ifneq ($(cmd_files),)
  include $(cmd_files)
endif

PHONY += FORCE
FORCE:

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable se we can use it in if_changed and friends.
.PHONY: $(PHONY)
