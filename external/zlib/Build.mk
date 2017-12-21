LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE=zlib_target
LOCAL_MODULE_TAGS :=optional
LOCAL_MODULE_GEN_SHARED_FILES= libz.so.1.2.8	\
			      libz.so.1		\
			      libz.so

LOCAL_MODULE_CONFIG_FILES:= configure.log
LOCAL_MODULE_CONFIG:= CHOST=$(DEVICE_COMPILER_PREFIX) ./configure --shared

LOCAL_MODULE_COMPILE=make -j$(MAKE_JLEVEL)
LOCAL_MODULE_COMPILE_CLEAN=make distclean

LOCAL_EXPORT_C_INCLUDE_FILES:= zlib.h		\
			       zconf.h

include $(BUILD_THIRDPART)
