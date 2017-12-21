LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := debuggerd
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES:= \
    backtrace.cpp \
    debuggerd.cpp \
    getevent.cpp \
    tombstone.cpp \
    utility.cpp \
	mips/machine.cpp

LOCAL_CPPFLAGS := \
    -std=gnu++11 \
    -W -Wall -Wextra \
    -Wunused \
    -Wno-error \
	-DHAVE_SYS_SOCKET_H \
	-D__USE_POSIX199309

LOCAL_LDLIBS := -lstdc++ -lrt -lpthread -lm -lc -ldl

LOCAL_C_INCLUDES := ./

LOCAL_DEPANNER_MODULES := \
	libbacktrace \
	libcutils \
	libselinux \
	libstlport

LOCAL_STATIC_LIBRARIES :=libcutils.a
LOCAL_SHARED_LIBRARIES := \
	libbacktrace.so \
	libselinux.so \
	libstlport.so \
	libdlog.so \
	libpcre.so \
	libmincrypt.so

include $(BUILD_EXECUTABLE)

