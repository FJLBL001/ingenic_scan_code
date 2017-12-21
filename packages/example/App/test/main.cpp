#include "scanvideo.h"
#include "DecodeEntry.h"
#include "scanpower.h"
#include "scanparam.h"
#include "scanserial.h"
#include "scangpio.h"








//#define THRESHOLD 1
#if defined THRESHOLD

int quickAdaptiveThreshold(unsigned char* grayscale, unsigned char* thres, int width, int height )  
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
    if(prev_gn == NULL)
    {
        return -1;
    }
    thres_tmp = (unsigned char *)malloc(width*sizeof(unsigned char )*height);  
    if(thres_tmp == NULL)
    {
        free(prev_gn);
        return -1;
    }   
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

    return 0;
}



int  adaptiveThreshold(unsigned char* input, unsigned char* output, int width, int height)  
{  
    int S = width >> 3;  
    int T = 15;  
      
    unsigned long* integralImg = 0; 
    unsigned char* bin = NULL; 
    int i, j;  
    long sum=0;  
    int count=0;  
    int index;  
    int x1, y1, x2, y2;  
    int s2 = S/2;  
      
    bin = (unsigned char*)malloc(width*height*sizeof(unsigned char));
    if(bin == NULL)
    {
        return -1;
    }

    // create the integral image  
    integralImg = (unsigned long*)malloc(width*height*sizeof(unsigned long)); 
    if(integralImg == NULL)
    {
        free(bin);
        return -1;
    } 
    for (i=0; i<width; i++)  
    {  
        // reset this column sum  
        sum = 0;  
        for (j=0; j<height; j++)  
        {  
            index = j*width+i;  
            sum += input[index];  
            if (i==0)  
                integralImg[index] = sum;  
            else  
                integralImg[index] = integralImg[index-1] + sum;  
        }  
    }  
    // perform thresholding  
    for (i=0; i<width; i++)  
    {  
        for (j=0; j<height; j++)  
        {  
            index = j*width+i;  
            // set the SxS region  
            x1=i-s2; x2=i+s2;  
            y1=j-s2; y2=j+s2;  
            // check the border  
            if (x1 < 0) x1 = 0;  
            if (x2 >= width) x2 = width-1;  
            if (y1 < 0) y1 = 0;  
            if (y2 >= height) y2 = height-1;  
            count = (x2-x1)*(y2-y1);  
            // I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)  
            sum = integralImg[y2*width+x2] -  
                integralImg[y1*width+x2] -  
                integralImg[y2*width+x1] +  
                integralImg[y1*width+x1];  
            if ((long)(input[index]*count) < (long)(sum*(100-T)/100))  
                bin[index] = 0;  
            else  
                bin[index] = 255;  
        }  
    } 

    memcpy(output,bin,width*height);

    free (integralImg);  
    free(bin);
}  



#endif














static struct camera_info g_camera_inf;
static struct scan_param g_scan_param;



static __u32 cim_yuv422_formats[8] = {
    YCbCr422_CrY1CbY0,  //CIMCFG.PACK[6:4]=0
    YCbCr422_Y0CrY1Cb,  //CIMCFG.PACK[6:4]=1
    YCbCr422_CbY0CrY1,  //CIMCFG.PACK[6:4]=2
    YCbCr422_Y1CbY0Cr,  //CIMCFG.PACK[6:4]=3
    YCbCr422_Y0CbY1Cr,  //CIMCFG.PACK[6:4]=4
    YCbCr422_CrY0CbY1,  //CIMCFG.PACK[6:4]=5
    YCbCr422_Y1CrY0Cb,  //CIMCFG.PACK[6:4]=6
    YCbCr422_CbY1CrY0,  //CIMCFG.PACK[6:4]=7
};

#if 0
int main(int argc, char const *argv[])
{

    int serial1_fd;

    char *result = NULL;
    int out_data_len = 0;


    CLEAR(g_scan_param);


    if(init_scan_param() != 0)
    {
        printf("init scan param fial\r\n");

        return -1;
    }

    read_scan_param(&g_scan_param,sizeof(g_scan_param));

/***<set led  */
    if(create_gpio_node(0)<0)
    {
        return -1;
    }
    set_gpio_output_high_low(0,"high");

    if(create_gpio_node(1)<0)
    {
        destroy_gpio_node(0);
        return -1;
    }
    set_gpio_output_high_low(1,"high");

/**<open and set serial param**/
    serial1_fd = UART0_Open("/dev/ttyS1");
    if(serial1_fd < 0)
    {
        destroy_gpio_node(0);
        destroy_gpio_node(1);
        return -1;
    }

    UART0_Set(serial1_fd, g_scan_param._serial_param.speed, g_scan_param._serial_param.flow_ctrl, g_scan_param._serial_param.databits, g_scan_param._serial_param.stopbits, g_scan_param._serial_param.parity);

    CLEAR(g_camera_inf);


    g_camera_inf.io_method = IO_METHOD_MMAP;

    g_camera_inf.fmt.format = cim_yuv422_formats[4];
    g_camera_inf.fmt.fourcc = V4L2_PIX_FMT_YUYV;

    if( atoi(argv[1]) == 0)
    {
        strcpy((char *)g_camera_inf.video_dev_name,"/dev/video0");
    }
    else
    {
        strcpy((char *)g_camera_inf.video_dev_name,"/dev/video1");
    }


    printf("video_dev_name = %s\n", (char *)g_camera_inf.video_dev_name);


    g_camera_inf.param.width = 320;
    g_camera_inf.param.height = 240 ;

    __s8 decode_image[g_camera_inf.param.width*g_camera_inf.param.height] = {0};

/**
STATE_SLEEP:

  set_power_state("mem",strlen("mem"));
**/

    g_camera_inf.fd_cim = open_device((const char *)g_camera_inf.video_dev_name);
    if (g_camera_inf.fd_cim < 0) {
        printf("==>fail to open %s\n", g_camera_inf.video_dev_name);

        UART0_Close(serial1_fd);

        destroy_gpio_node(0);
        destroy_gpio_node(1);

        return -1;
    }
    printf("open_device\n");

    /* initialize device*/
    init_device(&g_camera_inf);
       printf("init_device\n");

    /* camera start capture*/
    start_capturing(&g_camera_inf);
    printf("start_capturing\n");


    while(1)
    {

        process_framebuf(&g_camera_inf);
        
        {

            int i = 0;
            int tmplen;
            tmplen = g_camera_inf.datalen>>1;

            printf("tmplen = %d\n", tmplen);

        
            while(i < tmplen)
            {            
                decode_image[i] = *(unsigned char *)((unsigned char *)g_camera_inf.data+(i<<1));
            
                i++;
            }

            #if defined THRESHOLD
            adaptiveThreshold((unsigned char *)decode_image,(unsigned char *)decode_image,g_camera_inf.param.width,g_camera_inf.param.height);
            #endif

            #if 1
            {
                char tmp[240*320+5]={0};
                int usb_ser_fd = open("/dev/ttyGS0",O_RDWR);
                if(usb_ser_fd < 0)
                {
                     printf("usb_ser_fd  open fail \r\n");
                }
                tmp[0] = 0x20;
                memcpy(tmp+1,decode_image,g_camera_inf.param.width*g_camera_inf.param.height);
                tmp[g_camera_inf.param.width*g_camera_inf.param.height+1] = 0x04;
                if(write(usb_ser_fd,tmp,g_camera_inf.param.width*g_camera_inf.param.height+1+1) <0)
                {
                     printf("usb_ser_fd  write fail \r\n");
                }
                close(usb_ser_fd);
            }
            
            #endif
            


          

            /***
            out_data_len = sizeof(result) - 1;

            gettimeofday(&z_start,NULL);

            decodeCode(D_ZBAR,WIDTH,HEIGHT,(char *)imgbuff,result,&out_data_len);

            dbprintf("zbar result = %s\r\n",result);
            //    break;   
            gettimeofday(&z_end,NULL);

            dbprintf("zbar z_time = %d\r\n",z_end.tv_usec-z_start.tv_usec);
            ****/

            struct timeval t_start,t_end,z_start,z_end;

            
            gettimeofday(&z_start,NULL); 

            out_data_len = 0;
            decodeCode(D_ZXING,g_camera_inf.param.width ,g_camera_inf.param.height,(char *)decode_image,&result,&out_data_len);

            if(out_data_len>0)
            {
                printf("zxing result = %s\r\n",result);

                scan_UART0_Send(serial1_fd,result,out_data_len);

                free(result);
                result = NULL;
                break;
            }
            gettimeofday(&z_end,NULL);

            printf("zxing z_time = %d\r\n",z_end.tv_usec-z_start.tv_usec);
        }

 
    }

    printf("process_framebuf\n");

    stop_capturing(&g_camera_inf);

    printf("stop_capturing\n");

    uninit_device(&g_camera_inf);

    printf("uninit_device\n");
    

    close(g_camera_inf.fd_cim);


    UART0_Close(serial1_fd);

    destroy_gpio_node(0);
    destroy_gpio_node(1);

/**
    goto STATE_SLEEP;
**/    
    return 0;
}
#endif


#if 0
/*
 * V4L2 Spec Appendix B.
 *
 * Video Capture Example
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h> //getopt_long()
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


#include <sys/soundcard.h>
#include <math.h>
#include <getopt.h>
#include <linux/i2c.h>


typedef unsigned int    __u32;
typedef signed int  __s32;
typedef unsigned short  __u16;
typedef signed short    __s16;
typedef unsigned char   __u8;
typedef signed char __s8;

struct camera_param{
    __u32 tlb   : 1,
          debug : 1,
          format_spc: 1,
          save_jpeg_userptr :1,
          planar    : 1,
          reserved  : 26;


    __s32 cim_id;
    __u32 width;
    __u32 height;
    __u32 bpp;

};

struct camera_format {
    __u32 fourcc;   // compatible with V4L2
    __u32 format;   // local definition, more detail
};

struct camera_info{
    __s32 fd_cim;
    __s32 capture_limit;
    __s32 squeue;

    __u8  file_name[64];
    __u8  video_dev_name[16];

    struct camera_param param;
    struct camera_format fmt;

    __u32 yoff;
    __u32 xoff;

    __u8 *ybuf;
    __u8 *ubuf;
    __u8 *vbuf;
    __u32 n_bytes;

    void *jz_jpeg;
    void *buf_vaddr;
};

enum io_method {
    IO_METHOD_READ = 0,
    IO_METHOD_MMAP = 1,
    IO_METHOD_USERPTR = 2,
};

enum camera_options {
    OPS_PREVIEW = 0,
    OPS_CAPTURE = 1,
};

enum capture_options {
    OPS_SAVE_BMP = 0,
    OPS_SAVE_JPG = 1,
    OPS_SAVE_RAW = 2,
};

struct cim_ops {
    void (*capture_picture)(struct camera_info *camera_info, struct camera_ctl *camera_ctl);
    int (*priview_picture)(struct camera_info *camera_info);
    void (*camera_fps)(struct camera_info *camera_inf);
};

struct camera_ctl{
    /*v4l2 support  mode*/
    enum io_method io_method;
    /*camera optiongs*/
    enum camera_options cam_opt;
    /*capture optiong*/
    enum capture_options cap_opt;

    struct cim_ops ops;
};







#define CLEAR(x)    memset(&(x), 0, sizeof(x))
#define DBG()   do {} while(0)

struct buffer {
    void *  start;
    size_t  length;
};

static char *       dev_name    = NULL;
static int      fd      = -1;
struct buffer *     buffers     = NULL;
static unsigned int n_buffers   = 0;

static void errno_exit(const char *s)
{
    fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
    exit(EXIT_FAILURE);
}

static int xioctl(int fd, int request, void *arg)
{
    int r;

    do {
        r = ioctl(fd, request, arg);
    } while (-1 == r && EINTR == errno);

    return r;
}

static int read_frame(struct camera_info *camera_inf, struct camera_ctl *camera_ctl)
{
    struct v4l2_buffer buf;
    unsigned int i;
    unsigned long long starttime,endtime;
    enum io_method io = camera_ctl->io_method;

    switch (io) {
    case IO_METHOD_READ:
        if (-1 == read(fd, buffers[0].start, buffers[0].length)) {
            switch (errno) {
            case EAGAIN:
                return 0;
            case EIO:
            default:
                errno_exit("read");
            }
        }
        break;

    case IO_METHOD_MMAP:
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
            switch (errno) {
            case EAGAIN:
                return 0;
            case EIO:
            default:
                errno_exit("VIDIOC_DQBUF");
            }
        }

        assert(buf.index < n_buffers);


        {

             int i = 0;
            int tmplen;
            tmplen = buffers[buf.index].length>>1;

            printf("tmplen = %d\n", tmplen);

            char *imgbuff = (char *)malloc(tmplen);
            if (imgbuff == NULL)
            {
                printf("malloc fail \r\n");
                return -1;
            }

            while(i < tmplen)
            {            
                imgbuff[i] = *(unsigned char *)((unsigned char *)buffers[buf.index].start+(i<<1));
            
                i++;
            }

            
            #if 0
            {
                char tmp[240*320+5]={0};
                int usb_ser_fd = open("/dev/ttyGS0",O_RDWR);
                if(usb_ser_fd < 0)
                {
                     printf("usb_ser_fd  open fail \r\n");
                }
                tmp[0] = 0x20;
                memcpy(tmp+1,imgbuff,camera_inf->param.width*camera_inf->param.height);
                tmp[camera_inf->param.width*camera_inf->param.height+1] = 0x04;
                if(write(usb_ser_fd,tmp,camera_inf->param.width*camera_inf->param.height+1+1) <0)
                {
                     printf("usb_ser_fd  write fail \r\n");
                }
                close(usb_ser_fd);
            }
            
            #endif

        
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

            struct timeval t_start,t_end,z_start,z_end;


            out_data_len = sizeof(result) - 1;
            memset(result,0,sizeof(result));
            
            gettimeofday(&z_start,NULL);    
            
            decodeCode(D_ZXING,camera_inf->param.width ,camera_inf->param.height,(char *)imgbuff,result,&out_data_len);

            printf("zxing result = %s\r\n",result);
            gettimeofday(&z_end,NULL);

            printf("zxing z_time = %d\r\n",z_end.tv_usec-z_start.tv_usec);

            free(imgbuff);

        }   

    


        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            errno_exit("VIDIOC_QBUF");
    
        break;

    case IO_METHOD_USERPTR:
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_USERPTR;

        if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
            switch (errno) {
            case EAGAIN:
                return 0;
            case EIO:
            default:
                errno_exit("VIDIOC_DQBUF");
            }
        }
            int page_size = getpagesize();
            int buf_size = (buf.length  + page_size - 1) & ~(page_size - 1);
        for (i=0; i<n_buffers; ++i) {
            if (buf.m.userptr == (unsigned long)buffers[i].start
             && buf_size == buffers[i].length){
                break;
            }
        }

        

        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            errno_exit("VIDIOC_QBUF");

        break;
    }
    camera_inf->squeue++;
    return 1;
}

static void mainloop(struct camera_info *camera_inf, struct camera_ctl *camera_ctl)
{
    unsigned int count;

    count = 1;

    while (count-- > 0) {
        for (;;) {
            fd_set fds;
            struct timeval tv;
            int r;

            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* Timeout. */
            tv.tv_sec = 20;
            tv.tv_usec = 0;

            r = select(fd+1, &fds, NULL, NULL, &tv);

            if (-1 == r) {
                if (EINTR == errno)
                    continue;

                errno_exit("select");
            }

            if (0 == r) {
                fprintf(stderr, "select timeout\n");
                exit(EXIT_FAILURE);
            }

            if (read_frame(camera_inf, camera_ctl))
                break;
        }
    }
}

void stop_capturing(struct camera_ctl *camera_ctl)
{
    enum v4l2_buf_type type;
    enum io_method io = camera_ctl->io_method;

    switch (io) {
    case IO_METHOD_READ:
        /* nothing to do */
        break;
    case IO_METHOD_MMAP:
    case IO_METHOD_USERPTR:
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
            errno_exit("VIDIOC_STREAMOFF");

        break;
    }
}

void start_capturing(struct camera_ctl *camera_ctl)
{
    unsigned int i;
    enum v4l2_buf_type type;
    enum io_method io = camera_ctl->io_method;

    switch(io) {
    case IO_METHOD_READ:
        /* nothing to do */
        break;
    case IO_METHOD_MMAP:
        for (i=0; i<n_buffers; ++i) {
            struct v4l2_buffer buf;

            CLEAR(buf);

            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;

            if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                errno_exit("VIDIOC_QBUF");
        }

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
            errno_exit("VIDIOC_STREAMON");

        break;

    case IO_METHOD_USERPTR:
        for (i=0; i<n_buffers; ++i) {
            struct v4l2_buffer buf;

            CLEAR(buf);

            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_USERPTR;
            buf.index = i;
            buf.m.userptr = (unsigned long)buffers[i].start;
            buf.length = buffers[i].length;

            if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                errno_exit("VIDIOC_QBUF");
        }

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
            errno_exit("VIDIOC_STREAMON");

        break;
    }
}


static void init_read(unsigned int buffer_size)
{
    buffers = (struct buffer * )calloc(1, sizeof(*buffers));

    if (!buffers) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    buffers[0].length = buffer_size;
    buffers[0].start = malloc(buffer_size);

    if (!buffers[0].start) {
        fprintf(stderr, "Out of memory2\n");
        exit(EXIT_FAILURE);
    }
}

static void init_mmap(void)
{
    struct v4l2_requestbuffers req;

    CLEAR(req);

    req.count   = 2;
    req.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory  = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            fprintf(stderr, "%s does not support memory mapping\n", dev_name);
            exit(EXIT_FAILURE);
        } else {
            errno_exit("VIDIOC_REQBUFS");
        }
    }

    if (req.count < 1) {
        fprintf(stderr, "Insufficient buffer memory on %s\n", dev_name);
        exit(EXIT_FAILURE);
    }

    buffers = (struct buffer * )malloc(req.count * sizeof(*buffers));

    if (!buffers) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    for (n_buffers=0; n_buffers<req.count; ++n_buffers) {
        struct v4l2_buffer buf;

        CLEAR(buf);

        buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory  = V4L2_MEMORY_MMAP;
        buf.index   = n_buffers;

        if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
            errno_exit("VIDIOC_QUERYBUF");

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
            mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start)
            errno_exit("mmap");
    }
}

static void init_userp(struct camera_info *camera_inf, unsigned int buffer_size)
{
    struct v4l2_requestbuffers req;
    unsigned int page_size = 0;
    int use_tlb = camera_inf->param.tlb;

    page_size = getpagesize();
    buffer_size = (buffer_size + page_size - 1) & ~(page_size - 1);

    CLEAR(req);

    req.count   = 2;
    req.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory  = V4L2_MEMORY_USERPTR;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            fprintf(stderr, "%s does not support user pointer i/o\n", dev_name);
            exit(EXIT_FAILURE);
        } else
            errno_exit("VIDIOC_REQBUFS");
    }

    buffers =(struct buffer * )calloc(req.count, sizeof(*buffers));
    if (!buffers) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        buffers[n_buffers].length = buffer_size;
        if (use_tlb)
            buffers[n_buffers].start = (unsigned char *)valloc(buffer_size);
        else
            buffers[n_buffers].start = (unsigned char *)calloc(128, buffer_size);
        printf("==>%s L%d: the buffers[%d].start = %x\n", __func__, __LINE__ , n_buffers,buffers[n_buffers].start);
        if (!buffers[n_buffers].start) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
}

static void query_cap(struct v4l2_capability *cap)
{
    if (cap->capabilities & V4L2_CAP_VIDEO_CAPTURE)
        fprintf(stdout, "Video capture device\n");
    if (cap->capabilities & V4L2_CAP_READWRITE)
        fprintf(stdout, "Read/Write systemcalls\n");
    if (cap->capabilities & V4L2_CAP_STREAMING)
        fprintf(stdout, "Streaming I/O ioctls\n");
}

static void dump_fmt(struct v4l2_format *fmt)
{
    if (fmt->type == V4L2_BUF_TYPE_VIDEO_CAPTURE) {
        fprintf(stdout, "width=%d, height=%d\n", fmt->fmt.pix.width, fmt->fmt.pix.height);
        fprintf(stdout, "pixelformat=%s, field=%d\n", (char *)&fmt->fmt.pix.pixelformat, fmt->fmt.pix.field);
        fprintf(stdout, "bytesperline=%d, sizeimage=%d\n", fmt->fmt.pix.bytesperline, fmt->fmt.pix.sizeimage);
        fprintf(stdout, "colorspace=%d\n", fmt->fmt.pix.colorspace);
    }
}

int open_device(const char *dev_name)
{
    struct stat st;

    if (-1 == stat(dev_name, &st)) {
        fprintf(stderr, "Cannot identify '%s':%d, %s\n", dev_name, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (!S_ISCHR(st.st_mode)) {
        fprintf(stderr, "%s is not char device\n", dev_name);
        exit(EXIT_FAILURE);
    }
    fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);
    if (-1 == fd) {
        fprintf(stderr, "Cannot open '%s':%d, %s\n", dev_name, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return fd;
}

unsigned int set_format(struct camera_info *camera_inf)
{
    struct v4l2_format fmt;
    unsigned int min;
    int idx = 0;

    CLEAR(fmt);

    fmt.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width   = camera_inf->param.width;
    fmt.fmt.pix.height  = camera_inf->param.height;
    fmt.fmt.pix.pixelformat = camera_inf->fmt.fourcc;
    fmt.fmt.pix.field   = V4L2_FIELD_ANY;/*V4L2_FIELD_NONE;*/
//  fmt.fmt.pix.priv    = camera_inf->fmt.fmt_priv;

    if (-1 == ioctl(fd, VIDIOC_S_INPUT, &idx)) {
        fprintf(stderr, " VIDIOC_S_INPUT error\n");
        return -1;
    }
    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt)){
        printf("==>%s L%d: set error  !!!!!!!!!!!!! \n",__func__, __LINE__);
        errno_exit("VIDIOC_S_FMT");
    }
    if (-1 ==  xioctl(fd, VIDIOC_G_FMT, &fmt)){
        fprintf(stderr, " VIDIOC_G_FMT error\n");
        return -1;
    }
    if((camera_inf->param.width != fmt.fmt.pix.width) || (camera_inf->param.height != fmt.fmt.pix.height)){
        camera_inf->param.width = fmt.fmt.pix.width;
        camera_inf->param.height = fmt.fmt.pix.height;
    }

    min = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;

    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

    printf("==>%s L%d: sizeimage=0x%x fmt.fmt.pix.width = 0x%x fmt.fmt.pix.height = 0x%x\n", __func__, __LINE__, fmt.fmt.pix.sizeimage,fmt.fmt.pix.width, fmt.fmt.pix.height);

    return fmt.fmt.pix.sizeimage;
}

void init_device(struct camera_info *camera_inf, struct camera_ctl *camera_ctl)
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    enum io_method io = camera_ctl->io_method;
    unsigned int frmsize;
    int use_tlb = camera_inf->param.tlb;
    char *io_method_str[3] = {"IO_METHOD_READ", "IO_METHOD_MMAP", "IO_METHOD_USERPTR"};

    printf("==>%s L%d: io method: %s\n", __func__, __LINE__, io_method_str[io]);

    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
            fprintf(stderr, "%s is not V4L2 device\n", dev_name);
            exit(EXIT_FAILURE);
        } else
            errno_exit("VIDIOC_QUERYCAP");
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf(stderr, "%s is not capture device\n", dev_name);
        exit(EXIT_FAILURE);
    }

    switch (io) {
    case IO_METHOD_READ:
        if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
            fprintf(stderr, "%s doesn't support read i/o\n", dev_name);
            exit(EXIT_FAILURE);
        }
        break;
    case IO_METHOD_MMAP:
    case IO_METHOD_USERPTR:
        if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
            fprintf(stderr, "%s doesn't support streaming i/o\n", dev_name);
            exit(EXIT_FAILURE);
        }
        break;
    }

    /* select video input, video standard and tune here. */

    CLEAR(cropcap);

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect;   /* reset to default */

        if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
            switch(errno) {
            case EINVAL:
                /* Cropping not supported */
                break;
            default:
                /* Errors ignored */
                break;
            }
        }
    } else {
        /* Errors ignored */
    }

    frmsize = set_format(camera_inf);

    switch (io) {
    case IO_METHOD_READ:
        init_read(frmsize);
        break;
    case IO_METHOD_MMAP:
        init_mmap();
        break;
    case IO_METHOD_USERPTR:
        init_userp(camera_inf, frmsize);
        break;
    }
}

void uninit_device(struct camera_ctl *camera_ctl, struct camera_info *camera_inf)
{
    unsigned int i;
    int use_tlb = camera_inf->param.tlb;
    enum io_method io = camera_ctl->io_method;

    switch(io) {
    case IO_METHOD_READ:
        free(buffers[0].start);
        break;
    case IO_METHOD_MMAP:
        for (i=0; i<n_buffers; ++i)
            if (-1 == munmap(buffers[i].start, buffers[i].length))
                errno_exit("munmap");
        break;
    case IO_METHOD_USERPTR:
        if (use_tlb)
            for (i=0; i<n_buffers; ++i)
                free(buffers[i].start);
        break;
    }

    free(buffers);
}

int process_framebuf(struct camera_info *camera_inf, struct camera_ctl *camera_ctl)
{

    mainloop(camera_inf, camera_ctl);

    return 0;
}





static struct camera_info g_camera_inf;
static struct camera_ctl  g_camera_ctl;


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


int main(int argc, char const *argv[])
{
    CLEAR(g_camera_ctl);
    CLEAR(g_camera_inf);




    __u32 cim_yuv422_formats[8] = {
        YCbCr422_CrY1CbY0,  //CIMCFG.PACK[6:4]=0
        YCbCr422_Y0CrY1Cb,  //CIMCFG.PACK[6:4]=1
        YCbCr422_CbY0CrY1,  //CIMCFG.PACK[6:4]=2
        YCbCr422_Y1CbY0Cr,  //CIMCFG.PACK[6:4]=3
        YCbCr422_Y0CbY1Cr,  //CIMCFG.PACK[6:4]=4
        YCbCr422_CrY0CbY1,  //CIMCFG.PACK[6:4]=5
        YCbCr422_Y1CrY0Cb,  //CIMCFG.PACK[6:4]=6
        YCbCr422_CbY1CrY0,  //CIMCFG.PACK[6:4]=7
    };

    g_camera_ctl.io_method = IO_METHOD_MMAP;

    g_camera_inf.fmt.format = cim_yuv422_formats[4];
    g_camera_inf.fmt.fourcc = V4L2_PIX_FMT_YUYV;

    if( atoi(argv[1]) == 0)
    {
        strcpy((char *)g_camera_inf.video_dev_name,"/dev/video0");
    }
    else
    {
        strcpy((char *)g_camera_inf.video_dev_name,"/dev/video1");
    }


    printf("video_dev_name = %s\n", (char *)g_camera_inf.video_dev_name);


    g_camera_inf.param.width = 320;
    g_camera_inf.param.height = 240 ;

    g_camera_inf.fd_cim = open_device((const char *)g_camera_inf.video_dev_name);
    if (g_camera_inf.fd_cim < 0) {
        printf("==>fail to open %s\n", g_camera_inf.video_dev_name);
        return -1;
    }
    printf("open_device\n");


    /* initialize device*/
    init_device(&g_camera_inf, &g_camera_ctl);
       printf("init_device\n");

    /* camera start capture*/
    start_capturing(&g_camera_ctl);
    printf("start_capturing\n");


    while(1)
        process_framebuf(&g_camera_inf, &g_camera_ctl);

    printf("process_framebuf\n");

    stop_capturing(&g_camera_ctl);

    printf("stop_capturing\n");


    uninit_device(&g_camera_ctl,&g_camera_inf);

    printf("uninit_device\n");


    return 0;
}

#endif



#if 1

#include <poll.h>


#define err_print printf


int main(char argc,char *argv[])
{
    int gpio_fd;
    int ret;

    int value;

    struct pollfd fds[1];

    char tmp[100+1] = {0};


    while(1);
    {
        sleep(1);
    }

    create_gpio_node(atoi(argv[1]));

    sprintf(tmp,"echo falling > /sys/class/gpio/gpio%d/edge",atoi(argv[1]));

    system(tmp);

    //也可以使用上述代码来配置gpio的中断触发方式

    memset(tmp,0,sizeof(tmp));

    sprintf(tmp,"/sys/class/gpio/gpio%d/value",atoi(argv[1]));

    gpio_fd = open(tmp,O_RDONLY);

    if( gpio_fd == -1 )

        err_print("gpio open");

    fds[0].fd = gpio_fd;

    fds[0].events = POLLPRI;

    ret = lseek(gpio_fd,0,SEEK_SET);

    if( ret == -1 )

        err_print("lseek");

    ret = read(gpio_fd,&value,1);

    if( ret == -1 )

        err_print("read");

    while(1)
    {

        ret = poll(fds,1,-1);

        if( ret == -1 )

            err_print("poll");

        if( fds[0].revents & POLLPRI)
        {

            ret = lseek(gpio_fd,0,SEEK_SET);

            if( ret == -1 )

                err_print("lseek");

            ret = read(gpio_fd,&value,1);

            if( ret == -1 )

                err_print("read");

            /*此时表示已经监听到中断触发了，该干事了*/

            printf("okokok.......................................\n");

        }
    }
}


#endif
