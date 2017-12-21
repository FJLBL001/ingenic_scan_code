LOCAL_PATH:= $(call my-dir)

common_SRC_FILES := \
	src/booleans.c \
	src/canonicalize_context.c \
	src/disable.c \
	src/enabled.c \
	src/fgetfilecon.c \
	src/fsetfilecon.c \
	src/getenforce.c \
	src/getfilecon.c \
	src/getpeercon.c \
	src/lgetfilecon.c \
	src/load_policy.c \
	src/lsetfilecon.c \
	src/policyvers.c \
	src/procattr.c \
	src/setenforce.c \
	src/setfilecon.c \
	src/context.c \
	src/mapping.c \
	src/stringrep.c \
	src/compute_create.c \
	src/compute_av.c \
	src/avc.c \
	src/avc_internal.c \
	src/avc_sidtab.c \
	src/get_initial_context.c \
	src/checkAccess.c \
	src/sestatus.c \
	src/deny_unknown.c

common_HOST_FILES := \
	src/callbacks.c \
	src/check_context.c \
	src/freecon.c \
	src/init.c \
	src/label.c \
	src/label_file.c \
	src/label_android_property.c



include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(common_SRC_FILES) $(common_HOST_FILES) src/android.c
LOCAL_MODULE:= libselinux
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS :=-lc -DAUDITD_LOG_TAG=1003
LOCAL_DEPANNER_MODULES :=  \
	android-include \
	libselinux-include \
	libpcre \
	libdlog
include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(common_SRC_FILES) $(common_HOST_FILES) src/android.c
LOCAL_MODULE:= libselinux
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS := -lc -fPIC -DAUDITD_LOG_TAG=1003
LOCAL_DEPANNER_MODULES :=  \
	android-include \
	libselinux-include \
	libpcre \
	libdlog
include $(BUILD_SHARED_LIBRARY)


#==============
# copy header file

include $(CLEAR_VARS)
LOCAL_MODULE:=libselinux-include
LOCAL_MODULE_TAGS :=optional
LOCAL_MODULE_PATH :=$(TARGET_INSTALL_PATH)/include/selinux
LOCAL_COPY_FILES := \
	include/selinux/selinux.h \
	include/selinux/label.h \
	include/selinux/context.h \
	include/selinux/avc.h \
	include/selinux/android.h
include $(BUILD_MULTI_PREBUILT)
