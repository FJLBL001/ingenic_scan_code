#ifndef _SCANVIDEO_H
#define _SCANVIDEO_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>  //for videodev2.h
#include <linux/videodev2.h>
#include <math.h>





#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned int    __u32;
typedef signed int  __s32;
typedef unsigned short  __u16;
typedef signed short    __s16;
typedef unsigned char   __u8;
typedef signed char __s8;


enum io_method {
    IO_METHOD_READ = 0,
    IO_METHOD_MMAP = 1,
    IO_METHOD_USERPTR = 2,
};






struct camera_param{
    __u32 tlb   : 1;

    __u32 width;
    __u32 height;

};

struct camera_format {
    __u32 fourcc;   // compatible with V4L2
    __u32 format;   // local definition, more detail
};

struct camera_info{
    __s32 fd_cim;
    __s32 capture_limit;
    __s32 squeue;

    __u8  video_dev_name[16];

    struct camera_param param;
    struct camera_format fmt;

    enum io_method io_method;
    __u8 *data;
    __u32 datalen;

};


#define CLEAR(x)    memset(&(x), 0, sizeof(x))



#define DISP_FMT_YCbCr444    (1 << 31)
#define DISP_FMT_YCbCr422   (1 << 30)
#define DISP_FMT_YCbCr420   (1 << 29)
#define DISP_FMT_RGB888     (1 << 28)
#define DISP_FMT_RGB565     (1 << 27)
#define DISP_FMT_BAYER      (1 << 26)
#define DISP_FMT_ITU656P    (1 << 24)
#define DISP_FMT_ITU656I    (1 << 23)
#define DISP_FMT_OTHER      (1 << 22)
/*
#define DF_IS_YCbCr444(v) ( (v) & DISP_FMT_YCbCr444)
#define DF_IS_YCbCr422(v) ( (v) & DISP_FMT_YCbCr422)
#define DF_IS_YCbCr420(v) ( (v) & DISP_FMT_YCbCr420)
#define DF_IS_RGB888(v) ( (v) & DISP_FMT_RGB888)
#define DF_IS_RGB565(v) ( (v) & DISP_FMT_RGB565)
#define DF_IS_BAYER(v) ( (v) & DISP_FMT_BAYER)
#define DF_IS_ITU656P(v) ( (v) & DISP_FMT_ITU656P)
#define DF_IS_ITU656I(v) ( (v) & DISP_FMT_ITU656I)
*/
#define YCbCr422_MASK       (DISP_FMT_YCbCr422 | 0xFFFF)
#define YCbCr422_Y0CbY1Cr   (DISP_FMT_YCbCr422 | 0x0000)
#define YCbCr422_CbY1CrY0   (DISP_FMT_YCbCr422 | 0x0001)
#define YCbCr422_Y1CrY0Cb   (DISP_FMT_YCbCr422 | 0x0002)
#define YCbCr422_CrY0CbY1   (DISP_FMT_YCbCr422 | 0x0004)
#define YCbCr422_CrY1CbY0   (DISP_FMT_YCbCr422 | 0x0008)
#define YCbCr422_Y1CbY0Cr   (DISP_FMT_YCbCr422 | 0x0010)
#define YCbCr422_CbY0CrY1   (DISP_FMT_YCbCr422 | 0x0020)
#define YCbCr422_Y0CrY1Cb   (DISP_FMT_YCbCr422 | 0x0040)






extern int open_device(const char *dev_name);
extern void init_device(struct camera_info *camera_inf);
extern void start_capturing(struct camera_info *camera_inf);
extern int process_framebuf(struct camera_info *camera_inf);
extern int read_frame(struct camera_info *camera_inf);
extern void stop_capturing(struct camera_info *camera_inf);
extern void uninit_device(struct camera_info *camera_inf);


#ifdef __cplusplus
}
#endif



#endif


/*<
end of file
*****/
