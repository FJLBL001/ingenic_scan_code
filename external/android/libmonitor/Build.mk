LOCAL_PATH := $(my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE = libmonitor
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES :=  debugger.cpp ProcessMonitor.cpp

LOCAL_CFLAGS += -fPIC -DHAVE_PTHREADS -DHAVE_ANDROID_OS
LOCAL_LDFLAGS := -rpath-link=$(OUT_DEVICE_SHARED_DIR)
LOCAL_LDLIBS := -lstdc++ -lrt -lpthread -lm -lc


LOCAL_DEPANNER_MODULES += \
	libdlog \
	libcutils

LOCAL_SHARED_LIBRARIES += \
	libdlog.so
LOCAL_STATIC_LIBRARIES += \
	libcutils.a

LOCAL_EXPORT_C_INCLUDE_FILES := 		\
		include/monitor/ProcessMonitor.h

include $(BUILD_SHARED_LIBRARY)
