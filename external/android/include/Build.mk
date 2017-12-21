LOCAL_PATH := $(my-dir)

#===================================================
# copy include
# #
include $(CLEAR_VARS)

LOCAL_MODULE := android-include
LOCAL_MODULE_TAGS :=optional
LOCAL_MODULE_PATH :=$(TARGET_INSTALL_PATH)
LOCAL_MODULE_CLASS := DIR
LOCAL_MODULE_DIR := android-include
LOCAL_MODULE_OUT_DIR:=android/include

include $(BUILD_MULTI_COPY)
