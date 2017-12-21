LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := boa_build
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_GEN_BINRARY_FILES :=boa
LOCAL_MODULE_CONFIG_FILES:=Makefile
LOCAL_MODULE_COMPILE=make -j$(MAKE_JLEVEL)

LOCAL_MODULE_COMPILE_CLEAN=make clean

include $(BUILD_THIRDPART)

#***********************************
#copy file
#***********************************
include $(CLEAR_VARS)

LOCAL_MODULE := boa
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_FS_BUILD)
LOCAL_COPY_FILES :=	\
		/etc/boa/boa.conf:./boa.conf \
		/etc/mime.types:./mime.types \
		/var/www/index.html:./index.html \
		/usr/lib/cgi-bin/hello.cgi:./hello.cgi
LOCAL_DEPANNER_MODULES := boa_build
include $(BUILD_MULTI_COPY)


