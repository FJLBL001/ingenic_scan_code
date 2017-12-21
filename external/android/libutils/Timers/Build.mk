LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE=libtimer
LOCAL_MODULE_TAGS:=optional
LOCAL_MODULE_GEN_SHARED_FILES:=libtimer.so
LOCAL_SRC_FILES:=Timers.cpp
LOCAL_CPPFLAGS := -std=gnu++11
LOCAL_LDLIBS := -lc -lstdc++ -lpthread -lrt              #Link parameters
LOCAL_CFLAGS := -Wa,-mips32r2 -O2 -G 0 -Wall -fPIC -shared -D_GNU_SOURCE=1 -DHAVE_CONFIG_H
LOCAL_SHARED_LIBRARIES := libdlog.so
LOCAL_DEPANNER_MODULES:= dlog
include $(BUILD_SHARED_LIBRARY)
