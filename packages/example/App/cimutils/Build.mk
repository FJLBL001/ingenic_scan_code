LOCAL_PATH := $(my-dir)
#==================================================
# build cimutils
include $(CLEAR_VARS)
LOCAL_MODULE=cimutils
LOCAL_MODULE_TAGS:= optional

LOCAL_SRC_FILES:= main.c      \
	process_picture/saveraw.c   \
	process_picture/savebmp.c   \
	process_picture/savejpeg.c  \
	lcd/framebuffer.c   \
	cim/cim_fmt.c   \
	cim/video.c \
	cim/process.c   \
	cim/convert.c   \
	color_convert/convert_soft_mxu.c \
	color_convert/convert_soft_C.c

LOCAL_CFLAGS := -mmxu
# LOCAL_CFLAGS := -mmxu -DUSE_V4L2
LOCAL_LDLIBS := -lc -ljpeg-hw -ljpeg

LOCAL_C_INCLUDES:= include

LOCAL_DEPANNER_MODULES := CameraDemo.sh libjpeg-hw libjpeg uyuv_to_nv12

include $(BUILD_EXECUTABLE)
#===================================================
# copy the LOCAL_SRC_FILES
#
include $(CLEAR_VARS)
LOCAL_MODULE := CameraDemo.sh
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/CameraDemo
ifeq ($(TARGET_BOARD_PLATFORM),"m200s")
LOCAL_COPY_FILES := CameraDemo.sh:CameraDemo_TLB.sh
else
LOCAL_COPY_FILES := CameraDemo.sh:CameraDemo_NoTLB.sh
endif
include $(BUILD_PREBUILT)
#
#uyuv_to_nv12
include $(CLEAR_VARS)
LOCAL_MODULE=uyuv_to_nv12
LOCAL_MODULE_TAGS:= optional

LOCAL_SRC_FILES:=	color_convert/uyuv_to_nv12.c

LOCAL_LDLIBS := -lc

include $(BUILD_EXECUTABLE)
