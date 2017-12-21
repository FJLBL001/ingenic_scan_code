LOCAL_PATH := $(my-dir)

#===================================================
#  Cross compile  python
#
include $(CLEAR_VARS)
LOCAL_MODULE :=python2
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_GEN_BINRARY_FILES := \
				./build_mips/usr/bin/python \
				./build_mips/usr/bin/python2 \
				./build_mips/usr/bin/python2.7

LOCAL_MODULE_GEN_SHARED_FILES := \
				./build_mips/libpython2.7.so.1.0 \
				./build_mips/usr/lib/libpython2.7.so

LOCAL_MODULE_CONFIG_FILES :=Makefile

LOCAL_MODULE_CONFIG := \
					 mkdir build_pc;cd build_pc ; ../configure ;make -j$(MAKE_JLEVEL); \
					 cd .. ; \
					 mkdir build_mips; cd build_mips;mkdir usr ; \
					 ../configure --prefix=$(PWD)/$(LOCAL_PATH)/build_mips/usr\
					 --host=$(DEVICE_COMPILER_PREFIX) \
					 --build=mips \
					 --enable-ipv6 \
					 --enable-shared \
					 ac_cv_file__dev_ptmx="yes" \
					 ac_cv_file__dev_ptc="no"; \
					 make HOSTPYTHON=$(PWD)/$(LOCAL_PATH)/build_pc/python \
					 HOSTPGEN=$(PWD)/$(LOCAL_PATH)/build_pc/Parser/pgen \
					 BLDSHARED="mips-linux-gnu-gcc -shared" \
					 CROSS_COMPILE=$(DEVICE_COMPILER_PREFIX)- \
					 CROSS_COMPILE_TARGET=yes -j$(MAKE_JLEVEL) ;\
					 make install ;\
					 cd ..; ./trim_lib.sh

LOCAL_MODULE_COMPILE :=
LOCAL_MODULE_COMPILE_CLEAN := rm build_pc build_mips -rf
include $(BUILD_THIRDPART)

#==================================================
# copy config file to system root
##
include $(CLEAR_VARS)
LOCAL_MODULE :=python
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := DIR
LOCAL_MODULE_PATH := $(TARGET_FS_BUILD)/usr/lib/
LOCAL_MODULE_DIR := ./build_mips/usr/lib/python2.7
LOCAL_DEPANNER_MODULES := python2
include $(BUILD_MULTI_COPY)


