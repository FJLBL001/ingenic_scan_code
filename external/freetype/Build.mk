LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE=freetype
LOCAL_MODULE_TAGS :=optional
LOCAL_MODULE_GEN_SHARED_FILES=objs/.libs/libfreetype.so.6.3.20     \
			      objs/.libs/libfreetype.so.6         \
			      objs/.libs/libfreetype.so

LOCAL_MODULE_CONFIG_FILES:= config.log
LOCAL_MODULE_CONFIG:=./configure --enable-static --host=$(DEVICE_COMPILER_PREFIX)


LOCAL_MODULE_COMPILE=make -j$(MAKE_JLEVEL);
LOCAL_MODULE_COMPILE_CLEAN=make distclean

LOCAL_EXPORT_C_INCLUDE_FILES:=builds/unix/ft2unix.h    \
			      builds/unix/ftconfig.h  \
			      objs/ftmodule.h

include $(BUILD_THIRDPART)

