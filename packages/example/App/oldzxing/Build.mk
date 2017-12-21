LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

#name of the module:
LOCAL_MODULE = old_my_zxing
LOCAL_MODULE_TAGS := optional           #Development mode of module ,userdebug,eng,optional
#which directory the target file copy 
LOCAL_MODULE_PATH := $(TARGET_FS_BUILD)
LOCAL_DEPANNER_MODULES :=                       #depend on module name

ZXING_SRC_FILES := zxing/zxing/BarcodeFormat.cpp zxing/zxing/Binarizer.cpp zxing/zxing/BinaryBitmap.cpp zxing/zxing/ChecksumException.cpp zxing/zxing/DecodeHints.cpp zxing/zxing/Exception.cpp zxing/zxing/FormatException.cpp zxing/zxing/InvertedLuminanceSource.cpp zxing/zxing/LuminanceSource.cpp zxing/zxing/MultiFormatReader.cpp zxing/zxing/Reader.cpp zxing/zxing/Result.cpp zxing/zxing/ResultIO.cpp zxing/zxing/ResultPoint.cpp zxing/zxing/ResultPointCallback.cpp zxing/zxing/aztec/AztecDetectorResult.cpp zxing/zxing/aztec/AztecReader.cpp zxing/zxing/aztec/detector/Detector.cpp zxing/zxing/aztec/decoder/Decoder.cpp zxing/zxing/common/BitArray.cpp zxing/zxing/common/BitArrayIO.cpp zxing/zxing/common/BitMatrix.cpp zxing/zxing/common/BitSource.cpp zxing/zxing/common/CharacterSetECI.cpp zxing/zxing/common/DecoderResult.cpp zxing/zxing/common/DetectorResult.cpp zxing/zxing/common/GlobalHistogramBinarizer.cpp zxing/zxing/common/GreyscaleLuminanceSource.cpp zxing/zxing/common/GreyscaleRotatedLuminanceSource.cpp zxing/zxing/common/GridSampler.cpp zxing/zxing/common/HybridBinarizer.cpp zxing/zxing/common/IllegalArgumentException.cpp zxing/zxing/common/PerspectiveTransform.cpp zxing/zxing/common/Str.cpp zxing/zxing/common/StringUtils.cpp zxing/zxing/common/detector/MonochromeRectangleDetector.cpp zxing/zxing/common/detector/WhiteRectangleDetector.cpp zxing/zxing/common/reedsolomon/GenericGF.cpp zxing/zxing/common/reedsolomon/GenericGFPoly.cpp zxing/zxing/common/reedsolomon/ReedSolomonDecoder.cpp zxing/zxing/common/reedsolomon/ReedSolomonException.cpp zxing/zxing/datamatrix/decoder/BitMatrixParser.cpp zxing/zxing/datamatrix/decoder/DataBlock.cpp zxing/zxing/datamatrix/decoder/DecodedBitStreamParser.cpp zxing/zxing/datamatrix/decoder/Decoder.cpp zxing/zxing/datamatrix/detector/CornerPoint.cpp zxing/zxing/datamatrix/detector/Detector.cpp zxing/zxing/datamatrix/detector/DetectorException.cpp zxing/zxing/datamatrix/DataMatrixReader.cpp zxing/zxing/datamatrix/Version.cpp zxing/zxing/multi/ByQuadrantReader.cpp zxing/zxing/multi/GenericMultipleBarcodeReader.cpp zxing/zxing/multi/MultipleBarcodeReader.cpp zxing/zxing/multi/qrcode/QRCodeMultiReader.cpp zxing/zxing/multi/qrcode/detector/MultiDetector.cpp zxing/zxing/multi/qrcode/detector/MultiFinderPatternFinder.cpp zxing/zxing/pdf417/PDF417Reader.cpp zxing/zxing/pdf417/decoder/BitMatrixParser.cpp zxing/zxing/pdf417/decoder/DecodedBitStreamParser.cpp zxing/zxing/pdf417/decoder/Decoder.cpp zxing/zxing/pdf417/decoder/ec/ErrorCorrection.cpp zxing/zxing/pdf417/decoder/ec/ModulusGF.cpp zxing/zxing/pdf417/decoder/ec/ModulusPoly.cpp zxing/zxing/pdf417/detector/Detector.cpp zxing/zxing/pdf417/detector/LinesSampler.cpp zxing/zxing/qrcode/ErrorCorrectionLevel.cpp zxing/zxing/qrcode/FormatInformation.cpp zxing/zxing/qrcode/QRCodeReader.cpp zxing/zxing/qrcode/Version.cpp zxing/zxing/qrcode/detector/AlignmentPattern.cpp zxing/zxing/qrcode/detector/AlignmentPatternFinder.cpp zxing/zxing/qrcode/detector/Detector.cpp zxing/zxing/qrcode/detector/FinderPattern.cpp zxing/zxing/qrcode/detector/FinderPatternFinder.cpp zxing/zxing/qrcode/detector/FinderPatternInfo.cpp zxing/zxing/qrcode/decoder/BitMatrixParser.cpp zxing/zxing/qrcode/decoder/DataBlock.cpp zxing/zxing/qrcode/decoder/DataMask.cpp zxing/zxing/qrcode/decoder/DecodedBitStreamParser.cpp zxing/zxing/qrcode/decoder/Decoder.cpp zxing/zxing/qrcode/decoder/Mode.cpp zxing/zxing/oned/CodaBarReader.cpp zxing/zxing/oned/Code39Reader.cpp zxing/zxing/oned/Code93Reader.cpp zxing/zxing/oned/Code128Reader.cpp zxing/zxing/oned/EAN8Reader.cpp zxing/zxing/oned/EAN13Reader.cpp zxing/zxing/oned/ITFReader.cpp zxing/zxing/oned/MultiFormatOneDReader.cpp zxing/zxing/oned/MultiFormatUPCEANReader.cpp zxing/zxing/oned/OneDReader.cpp zxing/zxing/oned/OneDResultPoint.cpp zxing/zxing/oned/UPCAReader.cpp zxing/zxing/oned/UPCEANReader.cpp zxing/zxing/oned/UPCEReader.cpp zxing/bigint/BigInteger.cc zxing/bigint/BigIntegerAlgorithms.cc zxing/bigint/BigIntegerUtils.cc zxing/bigint/BigUnsigned.cc zxing/bigint/BigUnsignedInABase.cc 


ZBAR_SRC_FILES := zbar/decoder/code39.c zbar/decoder/code128.c zbar/decoder/ean.c zbar/decoder/i25.c zbar/decoder/pdf417.c zbar/decoder/qr_finder.c zbar/qrcode/bch15_5.c zbar/qrcode/binarize.c zbar/qrcode/isaac.c zbar/qrcode/qrdec.c zbar/qrcode/qrdectxt.c zbar/qrcode/rs.c zbar/qrcode/util.c zbar/config.c zbar/decoder.c zbar/error.c zbar/image.c zbar/img_scanner.c zbar/refcnt.c zbar/scanner.c zbar/symbol.c zbar/decoder/codabar.c zbar/decoder/databar.c zbar/decoder/code93.c


GPIO_SRC_FILES := gpio/scangpio.c

POWER_SRC_FILES := power/scanpower.c

PWN_SRC_FILES := pwm/scanpwm.c

SERIAL_SRC_FILES := serial/scanserial.c


ZXING_INC_FILES := zxing

ZBAR_INC_FILES := zbar zbar/decoder zbar/qrcode

GPIO_INC_FILES := gpio

POWER_INC_FILES := power

PWN_INC_FILES := pwm

SERIAL_INC_FILES := serial


LOCAL_SRC_FILES := $(ZXING_SRC_FILES) $(ZBAR_SRC_FILES) $(GPIO_SRC_FILES) $(POWER_SRC_FILES) $(PWN_SRC_FILES) $(SERIAL_SRC_FILES) DecodeEntry.cpp main.cpp        #compiled src files

LOCAL_CFLAGS := -Wall --static -DVERSION=\"0.1.4\"           # -ffunction-sections -fdata-sections                                             #C compile parameters
LOCAL_LDFLAGS :=    #Link parameters


LOCAL_LDLIBS := -lstdc++ -lpthread -lz  -lrt -lc -lm

LOCAL_C_INCLUDES :=                          #include files
LOCAL_CPP_INCLUDES:=  $(ZXING_INC_FILES) $(ZBAR_INC_FILES) $(GPIO_INC_FILES) $(POWER_INC_FILES) $(PWN_INC_FILES) $(SERIAL_INC_FILES)                         #cpp include files

LOCAL_STATIC_LIBRARIES :=
LOCAL_DEPANNER_MODULES:=
LOCAL_MODULE_GEN_BINRARY_FILES :=                 #The binrary file module

LOCAL_EXPORT_C_INCLUDE_FILES :=                 #export included files 
LOCAL_EXPORT_C_INCLUDE_DIRS :=                   #export included dir

include $(BUILD_EXECUTABLE)
