// -*- mode:c++; tab-width:2; indent-tabs-mode:nil; c-basic-offset:2 -*-
/*
 *  Copyright 2010-2011 ZXing authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file excep t in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

#include "DecodeEntry.h"



using namespace std;
using namespace zxing;
using namespace zxing::multi;
using namespace zxing::qrcode;

namespace {

    bool more = false;
}




static int decodeZbar(int dataWidth, int dataHeight,
                char *rotateData,char *decodeResult,int *decodeResultLen)
{

    zbar_image_scanner_t *scanner = NULL;

    /* create a reader */
    scanner = zbar_image_scanner_create();
    
//	printf("ZBAR_CFG_ENABLE=%x\n",ZBAR_CFG_ENABLE);

    /* configure the reader */
    zbar_image_scanner_set_config(scanner, ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
    /* obtain image data */
    void *raw = NULL;
	
    raw = (void *)malloc(dataWidth*dataHeight*sizeof(char));
	
	  memset(raw,0,dataWidth*dataHeight*sizeof(char));

  	memcpy(raw,rotateData,dataWidth*dataHeight*sizeof(char));	

    /* wrap image data */
    zbar_image_t *image = zbar_image_create();
    zbar_image_set_format(image, *(int*)"Y800");
    zbar_image_set_size(image, dataWidth,dataHeight);
    zbar_image_set_data(image, raw,dataWidth * dataHeight, zbar_image_free_data);
	
    /* scan the image for barcodes */
    int n = zbar_scan_image(scanner, image);

    /* extract results */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
    for(; symbol; symbol = zbar_symbol_next(symbol)) {
        /* do something useful with results */
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);

        int tmpdatalen = 0;

        tmpdatalen = zbar_symbol_get_data_length(symbol);
        
        if(tmpdatalen>0)
        {
          *decodeResultLen = *decodeResultLen>tmpdatalen?tmpdatalen:*decodeResultLen;
          memcpy(decodeResult,data,*decodeResultLen);
        }
    }

    /* clean up */
    zbar_image_destroy(image);
    zbar_image_scanner_destroy(scanner);

    return(0);
}



static int decodeZxing(int dataWidth, int dataHeight, int left, int top, int width, int height,
                char *rotateData,char *decodeResult,int *decodeResultLen) {

        GreyscaleLuminanceSource *a = NULL; 
        HybridBinarizer *b = NULL;
        BinaryBitmap *c = NULL;

    try {
        ArrayRef<char> data(rotateData, dataWidth * dataHeight);

        a = new GreyscaleLuminanceSource(data, dataWidth, dataHeight, left,
                                             top, width, height);
        Ref<LuminanceSource> source(a);

        b = new HybridBinarizer(source);
             
        Ref<Binarizer> binarizer(b);
            
        c = new BinaryBitmap(binarizer);
        
        Ref<BinaryBitmap> image(c);
 
        DecodeHints hints(DecodeHints::DEFAULT_QR_HINT);
        
        MultiFormatReader reader;
        
        Ref<Result> result(reader.decode(image, hints));

        const char* resultData = result->getText()->getText().c_str();
        
        int tmpdatalen = 0;

        tmpdatalen = result->getText()->getText().length();
        
        if(tmpdatalen>0)
        {
          *decodeResultLen = *decodeResultLen>tmpdatalen?tmpdatalen:*decodeResultLen;
            memcpy(decodeResult,resultData,*decodeResultLen);
        }

        //  delete c; /**<decode success with auto delete***/ 

        return 0;
    }

    catch (zxing::Exception &e) {
//        delete c;
    }
    return 0;
}



int decodeCode(DECODEMETHOD mode,int dataWidth, int dataHeight,char *rotateData,char *decodeResult,int *decodeResultLen)
{
  if(mode == D_ZBAR)
  {
    decodeZbar(dataWidth,dataHeight,rotateData,decodeResult,decodeResultLen);
  }
  else if (mode == D_ZXING)
  {
    decodeZxing(dataWidth,dataHeight,0,0,dataWidth,dataHeight,rotateData,decodeResult,decodeResultLen);
  }
  else
  {
    return -1;
  }

  if(*decodeResultLen>0)
  {
    return 0;
  }
  else
  {
    return -1;
  }
  
  
}


