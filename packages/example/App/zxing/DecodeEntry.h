#ifndef _DECODEENTRY_H
#define _DECODEENTRY_H



#include <cstdio>  
#include <cstdlib>  
#include <cstring>  
#include <assert.h>  
      
#include <getopt.h>              
      
#include <fcntl.h>               
#include <unistd.h>  
#include <errno.h>  
#include <malloc.h>  
#include <sys/stat.h>  
#include <sys/types.h>  
#include <sys/time.h>  
#include <sys/mman.h>  
#include <sys/ioctl.h>  
  
#include <asm/types.h>           
#include <linux/videodev2.h>  
 


#include <iostream>
#include <zxing/common/Counted.h>
#include <zxing/Binarizer.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/ReaderException.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/common/GreyscaleLuminanceSource.h>

#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/multi/qrcode/QRCodeMultiReader.h>
#include <zxing/multi/ByQuadrantReader.h>
#include <zxing/multi/GenericMultipleBarcodeReader.h>
#include <zxing/common/StringUtils.h>
//#include <syslog.h>


#include <zbar.h>
#include <config.h>




using namespace std;
using namespace zxing;
using namespace zxing::multi;
using namespace zxing::qrcode;

namespace {

    bool more = false;
}



#ifdef __cplusplus
extern "C" {
#endif




typedef enum decode_method
{
    D_ZBAR = 0,
    D_ZXING = 1
}DECODEMETHOD;



extern int decodeCode(DECODEMETHOD mode,int dataWidth, int dataHeight,char *rotateData,char **decodeResult,int 
*decodeResultLen,DecodeHints _DecodeHints);


#ifdef __cplusplus
}
#endif


#endif


