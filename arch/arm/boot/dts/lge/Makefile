ifeq ($(CONFIG_OF),y)

dtb-$(CONFIG_MACH_MSM8916_C50_MPCS_US)	+= msm8916-c50.dtb
dtb-$(CONFIG_MACH_MSM8916_C50_TMO_US)	+= msm8916-c50.dtb
dtb-$(CONFIG_MACH_MSM8916_G4STYLUSN_MPCS_US)	+= msm8916-g4stylusn.dtb
dtb-$(CONFIG_MACH_MSM8916_G4STYLUSN_TMO_US)	+= msm8916-g4stylusn.dtb

DTB_NAMES := $(subst $\",,$(CONFIG_BUILD_ARM_APPENDED_DTB_IMAGE_NAMES))
ifneq ($(DTB_NAMES),)
DTB_LIST := $(addsuffix .dtb,$(DTB_NAMES))
else
DTB_LIST := $(dtb-y)
endif

targets += dtbs
endif
#DTS_TARGET := $(patsubst "%",%,$(call CONFIG_DTS_TARGET,$(1)))
ifeq ($(CONFIG_MACH_MSM8916_C50_MPCS_US),y)
	DTS_TARGET := msm8916-c50_mpcs_us
else ifeq ($(CONFIG_MACH_MSM8916_C50_TMO_US),y)
	DTS_TARGET := msm8916-c50_tmo_us
else ifeq ($(CONFIG_MACH_MSM8916_G4STYLUSN_MPCS_US),y)
	DTS_TARGET := msm8916-g4stylusn_mpcs_us
else ifeq ($(CONFIG_MACH_MSM8916_G4STYLUSN_TMO_US),y)
	DTS_TARGET := msm8916-g4stylusn_tmo_us
else
	DTS_TARGET := msm8916-c50ds_global_com
endif

$(obj)/../%.dtb: $(src)/$(DTS_TARGET)/%.dts
	$(call if_changed_dep,dtc)

dtbs: $(addprefix $(obj)/../,$(DTB_LIST))

clean-files := *.dtb
