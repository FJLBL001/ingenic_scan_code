LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE=minigui_ths
LOCAL_MODULE_TAGS :=optional
LOCAL_MODULE_GEN_SHARED_FILES= src/.libs/libminigui_ths.so            \
			       src/.libs/libminigui_ths-3.0.so.12     \
			       src/.libs/libminigui_ths-3.0.so.12.0.0

LOCAL_MODULE_CONFIG_FILES:= config.log
LOCAL_MODULE_CONFIG:=./configure --enable-shared --host=$(DEVICE_COMPILER_PREFIX)	\
			LDFLAGS=-L$(ABS_DEVICE_SHARED_DIR)	\
			CFLAGS="-I$(ABS_DEVICE_INCLUDE_DIR) -std=gnu89"	\
			LIBS="-ljpeg -lpng -lz"

LOCAL_MODULE_COMPILE=make -j$(MAKE_JLEVEL)
LOCAL_MODULE_COMPILE_CLEAN=make distclean
LOCAL_DEPANNER_MODULES:=libjpeg libpng
#LOCAL_DEPANNER_MODULES=freetype

LOCAL_EXPORT_C_INCLUDE_FILES:= include/common.h          \
			       include/minigui.h         \
			       include/gdi.h             \
			       include/window.h		 \
			       include/endianrw.h	 \
			       mgconfig.h

include $(BUILD_THIRDPART)


