LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE=libpng
LOCAL_MODULE_TAGS :=optional
LOCAL_MODULE_GEN_SHARED_FILES=.libs/libpng.so.3.37.0	\
			      .libs/libpng.so.3		\
			      .libs/libpng.so

LOCAL_MODULE_CONFIG_FILES:= config.log
LOCAL_MODULE_CONFIG:=./configure --enable-shared --host=$(DEVICE_COMPILER_PREFIX)	\
		      CPPFLAGS="-I$(ABS_DEVICE_INCLUDE_DIR)"				\
		      LDFLAGS="-L$(ABS_DEVICE_SHARED_DIR)"

LOCAL_MODULE_COMPILE=make -j$(MAKE_JLEVEL)
LOCAL_MODULE_COMPILE_CLEAN=make distclean
LOCAL_DEPANNER_MODULES:=zlib_target

LOCAL_EXPORT_C_INCLUDE_FILES:=png.h    \
			      pngconf.h

include $(BUILD_THIRDPART)

