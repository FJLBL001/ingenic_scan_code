#Tools and libraries necessary by program

#kernel & uboot
PRODUCT_MODULES += $(KERNEL_TARGET_IMAGE) \
                   $(UBOOT_TARGET_FILE)


#TINYALSA_UTILS := tinyplay tinycap tinymix tinypcminfo
ALSA_UTILS := alsa-utils alsa-lib
RUNTESTDEMO_UTILS := RuntestScript \
                                         AmicScript  \
                                         bash \
                                         DmicScript \
                                         StorageMediaTestScript \
                                         AwakeTestScript
CAMERA_UTILS := cimutils

#the device applications & test demo
PRODUCT_MODULES +=$(RUNTESTDEMO_UTILS)\
		  $(CAMERA_UTILS)\
		  $(ALSA_UTILS)
#		  $(TINYALSA_UTILS)
#Demo
#PRODUCT_MODULES += gpio_demo \
            #       i2c_demo \
            #       spi_demo


