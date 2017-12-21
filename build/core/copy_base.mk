###########################################################
## Standard rules for copying files.
###########################################################

__local_path := $(LOCAL_PATH)
include $(BUILD_SYSTEM)/choose_src_target_path.mk
include $(BUILD_SYSTEM)/module_install.mk
include $(BUILD_SYSTEM)/mma_build.mk

$(call choose_src_target_path, $(LOCAL_COPY_FILES))

__out_module_file := $(LOCAL_MODULE_PATH)/$(LOCAL_TARGET_FILES)
__local_module_file :=$(LOCAL_PATH)/$(LOCAL_SRC_FILES)

ifeq (x$(LOCAL_MODULE_OUT_DIR),x)
__out_module_dir :=$(LOCAL_MODULE_PATH)
else
__out_module_dir :=$(LOCAL_MODULE_PATH)/$(LOCAL_MODULE_OUT_DIR)
endif

CLEAN_DEP_FILES.$(LOCAL_MODULE_BUILD) += $(__out_module_file)


ifneq ($(LOCAL_FILTER_MODULE),)
ALL_COPY_MODULES += $(__out_module_file)
ALL_MODULES += $(COPY_PREFIX_LOCAL_DEPANNER_MODULES)
endif

ifeq ($(LOCAL_MODULE_CLASS),DIR)
ifeq (x$(LOCAL_MODULE_OUT_DIR),x)
__out_module_dir := $(__out_module_dir).dir
$(LOCAL_MODULE_BUILD):$(__out_module_dir)
$(__local_module_file):$(COPY_PREFIX_LOCAL_DEPANNER_MODULES)
$(__out_module_dir):$(__local_module_file)
	$(hide) mkdir -p $(subst .dir,,$@)
	$(hide) cp -adrf $< $(subst .dir,,$@)
else
__out_module_dir := $(__out_module_dir).dir
$(LOCAL_MODULE_BUILD):$(__out_module_dir)
$(__local_module_file):$(COPY_PREFIX_LOCAL_DEPANNER_MODULES)
$(__out_module_dir):$(__local_module_file)
	$(hide) mkdir -p $(subst .dir,,$@)
	$(hide) cp -adrf $</* $(subst .dir,,$@)
endif
else
$(LOCAL_MODULE_BUILD):$(__out_module_file)
$(__local_module_file):$(COPY_PREFIX_LOCAL_DEPANNER_MODULES)
$(__out_module_file):$(__local_module_file)
	$(hide) mkdir -p $(dir $@)
	$(hide) cp -adr $< $@
endif
