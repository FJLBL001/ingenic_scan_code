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

#include "DecodeEntry.h"
#include "serial/scanserial.h"
#include "power/scanpower.h"

using namespace std;

namespace {

    bool more = false;
}




 
#define CLEAR(x) memset (&(x), 0, sizeof (x))  

#define WIDTH   320    
#define HEIGHT  240    


//#define dbprintf(...) do{}while(0)
//
#define dbprintf printf
 

 
struct buffer {  
        void *                  start;  
        size_t                  length;  
};  
  
static char *           dev_name        = "/dev/video0";//摄像头设备名  
static int              fd              = -1;  
struct buffer *         buffers         = NULL;  
static unsigned int     n_buffers       = 0;  
  
static unsigned long file_length;  

static unsigned char *imgbuff = NULL;



void quickAdaptiveThreshold(unsigned char* grayscale, unsigned char* thres, int width, int height )  
{  
      
    /**           / 
    *            | FOREGROUND, if pn < ((gs(n) + gs(n-w)) / (2*s)) * 
    * color(n) = |                     ((100-t)/100) 
    *            | BACKGROUND_QR, otherwise 
    *            / 
    * where pn = gray value of current pixel, 
    *        s = width of moving average, and 
    *        t = threshold percentage of brightness range 
    *    gs(n) = gs(n-1) * (1-1/s) + pn 
    *    gs(n-w) = gs-value of pixel above current pixel 
    * 
    */  
    int t = 15;   
    int s = width >> 3; // s: number of pixels in the moving average (w = image width)  
    const int S = 9; // integer shift, needed to avoid floating point operations  
    const int power2S = 1 << S;  
    // for speedup: multiply all values by 2^s, and use integers instead of floats  
    int factor = power2S * (100-t) / (100*s); // multiplicand for threshold  
    int gn = 127 * s; // initial value of the moving average (127 = average gray value)  
    int q = power2S - power2S / s; // constant needed for average computation  
    int pn, hn;  
    unsigned char *scanline = NULL;  
    int *prev_gn = NULL;  
    unsigned char* thres_tmp = NULL;
    int i,x,y;

    prev_gn = (int *)malloc(width*sizeof(unsigned int ));
    thres_tmp = (unsigned char *)malloc(width*sizeof(unsigned char )*height);     
    for (i = 0; i < width; i++) {  
        prev_gn[i] = gn;  
    }  

    for (y = 0; y < height; y ++ )  
    {  
        int yh = y * width;  
        scanline = grayscale + y * width;  
        for (x = 0; x <width; x ++ )  
        {  
            pn = scanline[x] ;  
            gn = ((gn * q) >> S) + pn;   
            hn = (gn + prev_gn[x]) >> 1;  
            prev_gn[x] = gn;          
            thres_tmp[yh+x] = (pn < ((hn*factor) >> S))?0:0xff;  
        }  
        y ++ ;  
        if ( y == height)  
            break;  
        yh = y * width;  
        scanline = grayscale + y * width;  
        for ( x = width-1; x >= 0; x --)  
        {  
            pn = scanline[x] ;  
            gn = ((gn * q) >> S) + pn;   
            hn = (gn + prev_gn[x]) >> 1;  
            prev_gn[x] = gn;          
            thres_tmp[yh+x] = (pn < ((hn*factor) >> S))?0:0xff;  
        }  
    }  
    
    memcpy(thres,thres_tmp,width*sizeof(unsigned char )*height);
    free(prev_gn);
    free(thres_tmp);
}  

////static unsigned char *file_name;  
//////////////////////////////////////////////////////  
//获取一帧数据  
//////////////////////////////////////////////////////  
static int read_frame (void)  
{  
    struct v4l2_buffer buf;  
    unsigned int i;  

    CLEAR (buf);  
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    buf.memory = V4L2_MEMORY_MMAP;  
    int ff = ioctl (fd, VIDIOC_DQBUF, &buf);  
    if(ff<0)  
        dbprintf("failture\n"); //出列采集的帧缓冲  
  
    assert (buf.index < n_buffers);  
        dbprintf ("buf.index dq is %d,\n",buf.index);  
   // fwrite(buffers[buf.index].start, buffers[buf.index].length, 1, file_fd); //将其写入文件中              
   // memcpy(imgbuff,buffers[buf.index].start,buffers[buf.index].length);

  
    i = 0;
    int tmplen;
    tmplen = buffers[buf.index].length>>1;

    while(i < tmplen)
    {            
        imgbuff[i] = *(unsigned char *)((unsigned char *)buffers[buf.index].start+(i<<1));
    
        i++;
    }

    /***
    {
        char tmp[240*320+5]={0};
        int usb_ser_fd = open("/dev/ttyGS0",O_RDWR);
        if(usb_ser_fd < 0)
        {
             dbprintf("usb_ser_fd  open fail \r\n");
        }
        tmp[0] = 0x20;
        memcpy(tmp+1,imgbuff,WIDTH*HEIGHT);
        tmp[WIDTH*HEIGHT+1] = 0x04;
        if(write(usb_ser_fd,tmp,WIDTH*HEIGHT+1+1) <0)
        {
             dbprintf("usb_ser_fd  write fail \r\n");
        }
        close(usb_ser_fd);
    }
    ****/
    

    
    ff=ioctl (fd, VIDIOC_QBUF, &buf); //再将其入列  
    if(ff<0)  
        dbprintf("failture VIDIOC_QBUF\n");   
    return 1;  
}  
      
int get_camdata(void)  
{  
    struct v4l2_capability cap;   
    struct v4l2_format fmt;  
    unsigned int i;  
    enum v4l2_buf_type type;  

    fd = open (dev_name, O_RDWR /* required */ |O_NONBLOCK);//打开设备  
    if(fd<0)  
    {     
        perror("open faiure!");  
        exit(1);  
    }  


    /***进入休眠***/

  //  set_power_state("mem",strlen("mem"));

    
    int ff=ioctl (fd, VIDIOC_QUERYCAP, &cap);               //获取摄像头参数    
    if(ff<0)  
        dbprintf("failture VIDIOC_QUERYCAP\n");  
  
    struct v4l2_fmtdesc fmt1;  
    int ret;  
    memset(&fmt1, 0, sizeof(fmt1));  
    fmt1.index = 0;            //初始化为0  
    fmt1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt1)) == 0)  //获取设置支持的视频格式  
    {  
        fmt1.index++;  
        dbprintf("{ pixelformat = '%c%c%c%c', description = '%s' }\n",fmt1.pixelformat & 0xFF,   
                (fmt1.pixelformat >> 8) & 0xFF,(fmt1.pixelformat >> 16) & 0xFF,   
                (fmt1.pixelformat >> 24) & 0xFF,fmt1.description);  
    }  

    CLEAR (fmt);  
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
//  fmt.fmt.pix.width       = 640;   
//  fmt.fmt.pix.height      = 480;  
    fmt.fmt.pix.width       = WIDTH;   
    fmt.fmt.pix.height      = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; //V4L2_PIX_FMT_JPEG;//V4L2_PIX_FMT_YUYV;//V4L2_PIX_FMT_YVU420;//V4L2_PIX_FMT_YUYV;  
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;  
    ff = ioctl (fd, VIDIOC_S_FMT, &fmt); //设置图像格式  
    if(ff<0)  
        dbprintf("failture VIDIOC_S_FMT\n");  

    file_length = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height; //计算图片大小  
    dbprintf("file_length = %d\r\n",file_length);

    imgbuff = (unsigned char *)malloc (WIDTH*HEIGHT);
    if(imgbuff == NULL)
    {
        dbprintf(" imgbuff malloc fail\r\n");
	    exit(EXIT_FAILURE);
    }
    
    memset(imgbuff,0,WIDTH*HEIGHT);

    struct v4l2_requestbuffers req;  
    CLEAR (req);  
    req.count               = 1;  
    req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    req.memory              = V4L2_MEMORY_MMAP;      
    ff=ioctl (fd, VIDIOC_REQBUFS, &req); //申请缓冲，count是申请的数量  
    if(ff<0)  
        dbprintf("failture VIDIOC_REQBUFS\n");  
    if (req.count < 1)  
        dbprintf("Insufficient buffer memory\n");  
  
    buffers = (struct buffer * )calloc (req.count, sizeof (*buffers));//内存中建立对应空间  
    if(buffers == NULL)
    {
        dbprintf("buffers Out of memory\n");
		exit(EXIT_FAILURE);
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)   
    {  
        struct v4l2_buffer buf;   //驱动中的一帧  
        CLEAR (buf);  
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
        buf.memory      = V4L2_MEMORY_MMAP;  
        buf.index       = n_buffers;  
  
        if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) //映射用户空间  
            dbprintf ("VIDIOC_QUERYBUF error\n");  
  
        buffers[n_buffers].length = buf.length;           
        buffers[n_buffers].start =  
        mmap (NULL /* start anywhere */,    //通过mmap建立映射关系  
            buf.length,  
            PROT_READ | PROT_WRITE /* required */,  
            MAP_SHARED /* recommended */,  
            fd, buf.m.offset);  
  
        if (MAP_FAILED == buffers[n_buffers].start)  
        dbprintf ("mmap failed\n");  
    }  
  
    for (i = 0; i < n_buffers; ++i)   
    {  
        struct v4l2_buffer buf;  
        CLEAR (buf);  
  
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
        buf.memory      = V4L2_MEMORY_MMAP;  
        buf.index       = i;  
  
        if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))//申请到的缓冲进入列队  
            dbprintf ("VIDIOC_QBUF failed\n");  
    }  
                  
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  



    if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据  
        dbprintf ("VIDIOC_STREAMON failed\n");  
  
    for (;;) //这一段涉及到异步IO  
    {  
        fd_set fds;  
        struct timeval tv;  
        int r;  
  
        struct timeval t_start,t_end,z_start,z_end;

        gettimeofday(&t_start, NULL);
       


        
        FD_ZERO (&fds);//将指定的文件描述符集清空  
        FD_SET (fd, &fds);//在文件描述符集合中增加一个新的文件描述符  
  
       /* Timeout. */  
        tv.tv_sec = 4;  
        tv.tv_usec = 0;  
  
        r = select (fd + 1, &fds, NULL, NULL, &tv);//判断是否可读（即摄像头是否准备好），tv是定时  
       
       
        if (-1 == r)   
        {  
            if (EINTR == errno)  
                continue;  
            dbprintf ("select err\n");  
        }  
        if (0 == r)   
        {  
            dbprintf ("select timeout\n");
            exit(EXIT_FAILURE);
        }  
        if (read_frame ())//如果可读，执行read_frame ()函数，并跳出循环  
        {
            char result[256] = {0};
            int out_data_len = 0;

            /***
            out_data_len = sizeof(result) - 1;

            gettimeofday(&z_start,NULL);

            decodeCode(D_ZBAR,WIDTH,HEIGHT,(char *)imgbuff,result,&out_data_len);

            dbprintf("zbar result = %s\r\n",result);
            //    break;   
            gettimeofday(&z_end,NULL);

            dbprintf("zbar z_time = %d\r\n",z_end.tv_usec-z_start.tv_usec);
            ****/

            out_data_len = sizeof(result) - 1;
            memset(result,0,sizeof(result));
            
            gettimeofday(&z_start,NULL);
            
        
            
            decodeCode(D_ZXING,WIDTH,HEIGHT,(char *)imgbuff,result,&out_data_len);

            dbprintf("zxing result = %s\r\n",result);
            //    break;   
            gettimeofday(&z_end,NULL);

            dbprintf("zxing z_time = %d\r\n",z_end.tv_usec-z_start.tv_usec);

            
            gettimeofday(&t_end,NULL);

            dbprintf("time = %d\r\n",t_end.tv_usec-t_start.tv_usec);
            
            int uart1fd = UART0_Open("/dev/ttyS1");
            if(uart1fd < 0)
            {
                dbprintf("open uart1 fail\r\n");
                return -1;
            }
        
            if(UART0_Set(uart1fd,115200,0,8,1,'N') <0)
            {
                dbprintf("set uart1 fail\r\n");
                return -1;
            }


            if(UART0_Send(uart1fd,result,out_data_len)<0)
            {
                dbprintf("send uart1 fail\r\n");
                return -1;
            }
            
            UART0_Close(uart1fd);

        }
    }  

//unmap:  
    for (i = 0; i < n_buffers; ++i)  
        if (-1 == munmap (buffers[i].start, buffers[i].length))  
            dbprintf ("munmap error");  
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;     
    if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))     
        dbprintf("VIDIOC_STREAMOFF");   


    close (fd);  
    free(imgbuff);
    free(buffers);


    return 0;  
}  



int main(int argc,char *argv[])
{
    get_camdata();
    return 0;
}








