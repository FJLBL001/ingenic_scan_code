#include <stdio.h>
#include "jpeg.h"

#include "savejpeg.h"
#include "jpeglib.h"
/* convert yuv422 to jpeg with soft*/
static int ycbcr422_planar_to_jpeg(__u8 *y, __u8 *cb, __u8 *cr,
			    int image_width, int image_height, FILE *fp, int quality)
{

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	// JSAMPROW row_pointer[1];  /* pointer to JSAMPLE row[s] */
	// int row_stride;    /* physical row width in image buffer */
	JSAMPIMAGE  buffer;
	int band,i,buf_width[3],buf_height[3], mem_size, max_line, counter;
	__u8 *yuv[3];
	__u8 *pSrc, *pDst;

	yuv[0] = y;
	yuv[1] = cb;
	yuv[2] = cr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, fp);

	cinfo.image_width = image_width;  /* image width and height, in pixels */
	cinfo.image_height = image_height;
	cinfo.input_components = 3;    /* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB;  /* colorspace of input image */

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE );

	cinfo.raw_data_in = TRUE;
	cinfo.jpeg_color_space = JCS_YCbCr;
	cinfo.comp_info[0].h_samp_factor = 2;
	cinfo.comp_info[0].v_samp_factor = 1;

	jpeg_start_compress(&cinfo, TRUE);

	buffer = (JSAMPIMAGE) (*cinfo.mem->alloc_small) ((j_common_ptr) &cinfo, JPOOL_IMAGE, 3 * sizeof(JSAMPARRAY));
	for(band=0; band <3; band++)
	{
		buf_width[band] = cinfo.comp_info[band].width_in_blocks * DCTSIZE;
		buf_height[band] = cinfo.comp_info[band].v_samp_factor * DCTSIZE;
		buffer[band] = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, buf_width[band], buf_height[band]);
	}

	max_line = cinfo.max_v_samp_factor*DCTSIZE;

	for(counter=0; cinfo.next_scanline < cinfo.image_height; counter++)
	{
		//buffer image copy.
		for(band=0; band <3; band++)
		{
			mem_size = buf_width[band];
			pDst = (__u8 *) buffer[band][0];
			pSrc = (__u8 *) yuv[band] + counter*buf_height[band] * buf_width[band];

			for(i=0; i <buf_height[band]; i++)
			{
				memcpy(pDst, pSrc, mem_size);
				pSrc += buf_width[band];
				pDst += buf_width[band];
			}
		}
		jpeg_write_raw_data(&cinfo, buffer, max_line);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	return 0;

}
int yuv422_to_jpeg_soft (void * handle, const __u8 *frm, FILE *fp, struct camera_info *camera_inf, int quality)
{
	__u8 *ybuf = NULL;
	__u8 *ubuf = NULL;
	__u8 *vbuf = NULL;
	int width = camera_inf->param.width;
	int height = camera_inf->param.height;
	int fmt = camera_inf->fmt.format;
	int retval = -1;

	/* convert yuv422 pack to planar */

	debug("==>%s(): w=%d, h=%d, fmt=0x%08x\n", __func__, width, height, fmt);

	ybuf = malloc(width * height);
	ubuf = malloc(width * height);
	vbuf = malloc(width * height);

	if ( (ybuf == NULL) || (ubuf == NULL) || (vbuf == NULL) ) {
		printf("%s: alloc memory failed!\n", __FUNCTION__);
		return -1;
	}

	yCbCr422_pack_to_planar(ybuf, ubuf, vbuf, frm, camera_inf);
	retval = ycbcr422_planar_to_jpeg(ybuf, ubuf, vbuf, width, height, fp, quality);

	free(ybuf);
	free(ubuf);
	free(vbuf);

	return retval;
}
static float start_time,end_time;
static float usetime_ms()
{
	struct timeval t;
	gettimeofday(&t,NULL);
	float ms = (t.tv_sec * 1000)+ (t.tv_usec / 1000);
	return ms;

}
int yuv422_packed_to_jpeg(void *handle, const __u8 *frm, FILE *fp, struct camera_info *camera_inf, int quality)
{
	int width = camera_inf->param.width;
	int height = camera_inf->param.height;
	int retval = -1;
	int save_jpeg_userptr = camera_inf->param.save_jpeg_userptr;
	if(save_jpeg_userptr)
	{
		debug("==>%s %dL  save_jpeg_userptr =%d \n", __func__, __LINE__, save_jpeg_userptr);
//		printf("before userptr compress!!!\n");
//		start_time = usetime_ms();
		retval = yuv422_to_jpeg (handle, frm, fp, width, height, quality);    //defined in libjpeg.so for userptr model
//		end_time = usetime_ms();
//		printf("jpeg use time: %.2f ms\n",(end_time - start_time));
//		printf("after userptr compress!!!\n");
	}
	else
	{
		debug("==>%s %dL  save_jpeg_mmap =%d \n", __func__, __LINE__, save_jpeg_userptr);
#ifdef USE_V4L2
		retval = yuv422_to_jpeg (handle, frm, fp, width, height, quality);    //defined in libjpeg.so for userptr model
#else
//		printf("before mmap compress!!!\n");
//		start_time = usetime_ms();
		retval = yuv422_to_jpeg_soft(handle, frm, fp, camera_inf, 100);          //defined in savejpeg.c for mmap model
//		end_time = usetime_ms();
//		printf("soft use time: %.2f ms\n",(end_time - start_time));
//		printf("after mmap compress!!!\n");
#endif
	}
	return retval;
}

