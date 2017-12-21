/* Copyright (C)
 * 2017 - cxtan, chenxi.tan@ingenic.com
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include <stdio.h>
#include "headers.h"
#include "camera.h"
#include "jpeg.h"

static struct camera_info camera_inf;
static struct camera_ctl  camera_ctl;
static struct fb_info fb_info;
static unsigned long long starttime,endtime;

static struct option long_options[] = {
	{ "preview            ", 0, 0, 'P'}, /* 0 */
	{ "capture            ", 0, 0, 'C'},
	{ "testing            ", 0, 0, 'T'},
	{ "preview_bpp        ", 1, 0, 'b'},
	{ "capture_width      ", 1, 0, 'x'},
	{ "capture_height     ", 1, 0, 'y'},
	{ "file               ", 1, 0, 'f'},
	{ "format             ", 1, 0, 't'},
	{ "cim_id             ", 1, 0, 'I'},
	{ "virtual_memory     ", 0, 0, 'v'},
	{ "capture count limit", 0, 0, 'l'},
	{ "test fps           ", 0, 0, 'd'},
};

#define print_opt_help(opt_index, help_str)				\
	do {								\
		printf("\t--%s\t-%c\t%s", long_options[opt_index].name, (char)long_options[opt_index].val, help_str); \
	} while (0)
static void usage() {
	printf("\nUsage:\n");
	print_opt_help(0, "preview on lcd panel\n");
	print_opt_help(1, "capture in JPEG or BMP format\n");
	print_opt_help(2, "using virtual memory based on internal tlb\n");
	print_opt_help(3, "preview bpp\n");
	print_opt_help(4, "picture width\n");
	print_opt_help(5, "picture height\n");
	print_opt_help(6, "filename\n");
	print_opt_help(7, "source format\n");
	print_opt_help(8, "cim id (0 or 1)\n");
	print_opt_help(9, "use userptr mode\n");
	print_opt_help(10, "capture count limit\n");
	print_opt_help(11, "test fps for cim\n");

}

static int get_paramt(int argc, char *argv[])
{
	int ret = 0;
	char optstring[] = "PCTb:x:y:f:t:I:vl:d";
	if (argc < 2)
	{
		printf("please input paramt!!!");
		usage();
		return -1;
	}
	while ((ret = getopt_long(argc, argv, optstring, long_options, NULL)) != -1)
	{
		switch (ret) {
			case 'P':
				camera_ctl.cam_opt = OPS_PREVIEW;
				printf("do preview ...\n");
				break;
			case 'C':
				camera_ctl.cam_opt = OPS_CAPTURE;
				printf("do capture......\n");
				break;
			case 'T':
				camera_inf.param.tlb = 1;
				printf("use tlb ......\n");
				break;
			case 't':
				camera_inf.fmt.format = str_to_fmt(optarg);
				camera_inf.param.format_spc = 1;
				printf("format = %s(%#0x)\n", optarg, camera_inf.fmt.format);
				break;
			case 'b':
				camera_inf.param.bpp = atoi(optarg);
				printf("preview bpp = %d\n", camera_inf.param.bpp);
				break;
			case 'x':
				camera_inf.param.width = atoi(optarg);
				printf("capture width = %d\n", camera_inf.param.width);
				break;
			case 'y':
				camera_inf.param.height = atoi(optarg);
				printf("capture height = %d\n", camera_inf.param.height);
				break;
			case 'f':
				if (strlen(optarg) > 63) {
					printf("file name too long!\n");
					usage();
					return -1;
				}
				strncpy(camera_inf.file_name, optarg, strlen(optarg));
				break;
			case 'I':
				camera_inf.param.cim_id = atoi(optarg);
				if ((camera_inf.param.cim_id != 0) && (camera_inf.param.cim_id != 1)) {
					printf("====>invalid cim id (%d)\n", camera_inf.param.cim_id);
					return -1;
				}
				printf("using cim%d ......\n", camera_inf.param.cim_id);
				break;
			case 'v':
				camera_ctl.io_method = IO_METHOD_USERPTR;
				camera_inf.param.save_jpeg_userptr = 1;     //for choosing  compress pictures model  in savepage.c
				printf("using userptr mode......\n");
				break;
			case 'l':
				camera_inf.capture_limit = atoi(optarg);
				printf("raw_limit = %d\n", camera_inf.capture_limit);
				break;
			case 'd':
				camera_inf.param.debug = 1;
				printf("camera_inf.param.debug = %d\n", camera_inf.param.debug);
				break;
			default:
				usage();
				return -1;
				;
		}
	}
	return 0;
}

static long long usectime(void)
{
	struct timeval t;
	gettimeofday(&t,NULL);

	return (((long long) t.tv_sec) * 1000000LL) +
		((long long) t.tv_usec);
}

static void camera_fps(struct camera_info *camera_inf)
{
	if (camera_inf->squeue % 60 == 0){
		starttime = usectime();
	} else if (camera_inf->squeue % 60 == 59){
		endtime = usectime();
		printf("fps = %lld, starttime is %lld, endtime is %lld, squeue is %d\n", \
				1000/((endtime - starttime) / 60 / 1000), starttime, endtime, camera_inf->squeue);
	}
}

static void cim_uninit()
{

	if(camera_ctl.io_method == IO_METHOD_USERPTR){
		if (!camera_inf.param.tlb)
			jz_jpeg_encode_deinit(camera_inf.jz_jpeg);
	}
	uninit_device(&camera_ctl, &camera_inf);	//cleanup video buf

	if (camera_inf.fd_cim > 0)
		close(camera_inf.fd_cim);

	if (camera_ctl.cam_opt == OPS_PREVIEW){
		finish_framebuffer(&fb_info);

		if (camera_inf.ybuf) {
			free(camera_inf.ybuf);
			free(camera_inf.ubuf);
			free(camera_inf.vbuf);
		}

	}
}

static void do_clean()
{
	cim_uninit();
	exit(0);
}

static void sig_int(int signo)
{
	do_clean();
}

static int priview_picture(struct camera_info *camera_inf)
{
	int ret = 0;
	ret = display_to_fb(camera_inf, &fb_info);	//preview_picture
	return ret;
}

static void capture_picture(struct camera_info *camera_inf, struct camera_ctl *camera_ctl)
{
	enum capture_options ops = camera_ctl->cap_opt;

	if (camera_inf->squeue == camera_inf->capture_limit)
		process_frame(camera_inf, ops);
}


static int cim_init(int argc, char *argv[])
{
	int bpp = 16;
	int ret = 0;
	camera_inf.param.bpp = bpp;
	camera_inf.fd_cim = -1;
	camera_inf.capture_limit = 5;
	camera_ctl.io_method = IO_METHOD_MMAP;

	const char *fb_dev_name = "/dev/fb0";
	__u32 cim_yuv422_formats[8] = {
		YCbCr422_CrY1CbY0,	//CIMCFG.PACK[6:4]=0
		YCbCr422_Y0CrY1Cb,	//CIMCFG.PACK[6:4]=1
		YCbCr422_CbY0CrY1,	//CIMCFG.PACK[6:4]=2
		YCbCr422_Y1CbY0Cr,	//CIMCFG.PACK[6:4]=3
		YCbCr422_Y0CbY1Cr,	//CIMCFG.PACK[6:4]=4
		YCbCr422_CrY0CbY1,	//CIMCFG.PACK[6:4]=5
		YCbCr422_Y1CrY0Cb,	//CIMCFG.PACK[6:4]=6
		YCbCr422_CbY1CrY0,	//CIMCFG.PACK[6:4]=7
	};
	memset(camera_inf.file_name, 0, sizeof(camera_inf.file_name));
	memset(camera_inf.video_dev_name, 0, sizeof(camera_inf.video_dev_name));

	/* 1. GET paramet*/
	ret = get_paramt(argc, argv);
	if (ret){
		printf("====>set paramet error!!!\n");
		return -1;
	}
	/* 2. VPU initialize */
	if (!camera_inf.param.tlb && camera_ctl.cam_opt == OPS_CAPTURE){
		printf("====>VPU initialize\n");
		camera_inf.jz_jpeg = jz_jpeg_encode_init(camera_inf.param.width, camera_inf.param.height);
		if (!camera_inf.jz_jpeg){
			printf("====>jpeg init faild!!!\n");
			return -1;
		}
	}
	/*3. Format initialize*/
	switch (camera_inf.fmt.format) {
		case DISP_FMT_YCbCr422:
			camera_inf.fmt.fourcc = V4L2_PIX_FMT_YUYV;
			break;
		case DISP_FMT_YCbCr420:
			camera_inf.fmt.fourcc = V4L2_PIX_FMT_YUV420;
			break;
		default:
			camera_inf.fmt.format = cim_yuv422_formats[4];
			camera_inf.fmt.fourcc = V4L2_PIX_FMT_YUYV;
			break;
	}

	/*4. LCD initialize*/
	if (camera_ctl.cam_opt == OPS_PREVIEW){
		strcpy(fb_info.dev_name, fb_dev_name);
		init_framebuffer(&fb_info);
	}

	camera_ctl.ops.priview_picture = priview_picture;
	camera_ctl.ops.capture_picture = capture_picture;
	camera_ctl.ops.camera_fps = camera_fps;

}

int main(int argc, char *argv[])
{
	char buf[16];
	char *suffix = buf;
	int ret = 0;
	int squeue_count = 0;
	signal(SIGINT, sig_int);

	/*initialize cim device*/
	cim_init(argc, argv);

	snprintf(camera_inf.video_dev_name, 16, "/dev/video%d", camera_inf.param.cim_id);
	camera_inf.fd_cim = open_device(camera_inf.video_dev_name);
	if (camera_inf.fd_cim < 0) {
		printf("==>fail to open %s\n", camera_inf.video_dev_name);
		return -1;
	}
	if (camera_ctl.cam_opt == OPS_CAPTURE){
		suffix = strchr(camera_inf.file_name, '.');
		if (! strcmp(suffix, ".bmp"))
			camera_ctl.cap_opt = OPS_SAVE_BMP;
		else if (! strcmp(suffix, ".jpg"))
			camera_ctl.cap_opt = OPS_SAVE_JPG;
		else if (! strcmp(suffix, ".raw"))
			camera_ctl.cap_opt = OPS_SAVE_RAW;
		else
		{
			printf("=======>>>Error! The case only support .bmp .jpg .raw now!!!\n");
			return -1;
		}
	}
	/* initialize device*/
	init_device(&camera_inf, &camera_ctl);

	/* camera start capture*/
	start_capturing(&camera_ctl);

	/* process camera capture buf*/
	if (camera_ctl.cam_opt == OPS_PREVIEW){
		printf("========>>>Now begin preview!!\n");
		for (;;)
			process_framebuf(&camera_inf, &camera_ctl);
	} else if (camera_ctl.cam_opt == OPS_CAPTURE){
		for (squeue_count = 0; squeue_count <= camera_inf.capture_limit; squeue_count++){
			process_framebuf(&camera_inf, &camera_ctl);
		}
	}

	/* camera stop capture*/
	stop_capturing(&camera_ctl);

	/* uninitalize cim device*/
	cim_uninit();

	printf("----------------------CIM TEST END -----------------------\n");
	return ret;
}
