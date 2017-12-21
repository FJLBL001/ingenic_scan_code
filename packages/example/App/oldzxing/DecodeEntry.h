#ifndef _DECODEENTRY_H
#define _DECODEENTRY_H

#ifdef __cplusplus
extern "C" {
#endif




typedef enum decode_method
{
    D_ZBAR = 0,
    D_ZXING = 1
}DECODEMETHOD;



extern int decodeCode(DECODEMETHOD mode,int dataWidth, int dataHeight,char *rotateData,char *decodeResult,int *decodeResultLen);


#ifdef __cplusplus
}
#endif


#endif


