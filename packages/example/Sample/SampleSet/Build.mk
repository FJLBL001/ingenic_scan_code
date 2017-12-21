LOCAL_PATH := $(my-dir)
#====copy runtest.sh ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := RuntestScript
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)
LOCAL_COPY_FILES := runtest.sh:runtest.sh
include $(BUILD_MULTI_COPY)
#
#===DMIC test Demo=================================================
include $(CLEAR_VARS)
LOCAL_MODULE := DmicScript
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)
LOCAL_COPY_FILES := Dmic-Demo.sh:Dmic-Demo.sh
include $(BUILD_MULTI_COPY)
#
#===AMIC test Demo==================================================
include $(CLEAR_VARS)
LOCAL_MODULE := AmicScript
LOCAL_MODULE_TAGS :=optional
LOCAL_MODULE_PATH := $(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)
LOCAL_COPY_FILES := Amic-Demo.sh:Amic-Demo.sh
LOCAL_DEPANNER_MODULES:= tinycap tinyplay tinymix
include $(BUILD_MULTI_COPY)
#
#===nand nor copy test Demo==================================================
include $(CLEAR_VARS)
LOCAL_MODULE := StorageMediaTestScript
LOCAL_MODULE_TAGS :=optional
LOCAL_MODULE_PATH := $(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)
LOCAL_COPY_FILES := StorageMedia-Test.sh
include $(BUILD_MULTI_COPY)
#
#===Awak test Demo==================================================
include $(CLEAR_VARS)
LOCAL_MODULE := AwakeTestScript
LOCAL_MODULE_TAGS :=optional
LOCAL_MODULE_PATH := $(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)
LOCAL_COPY_FILES :=Awake-Test.sh
include $(BUILD_MULTI_COPY)
