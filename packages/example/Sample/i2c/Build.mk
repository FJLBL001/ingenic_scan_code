LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := i2c_sample
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/usr/bin/
LOCAL_SRC_FILES := PmuDevice.cpp
LOCAL_LDLIBS := -lc -lstdc++
LOCAL_SHARED_LIBRARIES := libi2c.so #source code : development/source
LOCAL_DEPANNER_MODULES := libi2c i2c_testscript

include $(BUILD_EXECUTABLE)

# #####################
# eeprom Demo
#######################
include $(CLEAR_VARS)
LOCAL_MODULE := eeprom_test
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/usr/bin/
LOCAL_SRC_FILES := EEpromDevice.cpp
LOCAL_LDLIBS := -lc -lstdc++
LOCAL_SHARED_LIBRARIES := libi2c.so #source code : development/source
LOCAL_DEPANNER_MODULES := libi2c

include $(BUILD_EXECUTABLE)
#######################
# eeprom speed test
#######################
include $(CLEAR_VARS)
LOCAL_MODULE := i2c_speed_test
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/usr/bin/
LOCAL_SRC_FILES := i2c_speed_test.cpp
LOCAL_LDLIBS := -lc -lstdc++ -lrt
LOCAL_SHARED_LIBRARIES := libi2c.so #source code : development/source
LOCAL_DEPANNER_MODULES := libi2c i2c_speed_testscript

include $(BUILD_EXECUTABLE)
#=============================
# copy test script
#
include $(CLEAR_VARS)
LOCAL_MODULE :=i2c_testscript
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/
LOCAL_COPY_FILES := I2C-Test.sh
include $(BUILD_PREBUILT)
#=============================
# copy test script
#
include $(CLEAR_VARS)
LOCAL_MODULE :=i2c_speed_testscript
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/
LOCAL_COPY_FILES := i2c_speed_test.sh
include $(BUILD_PREBUILT)

