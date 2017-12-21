#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void uyuv_to_nv12 (char *src,int dstStride, char *dst, int width, int height)
{
	char *py;
	char *puv;

	int linesize = width * 2;
	int offsety=0;
	int offsetuv=0;

	py=dst;
	puv=py+(width*height);

	int h=0;
	int w=0;

	int wy=0;
	int wuv=0;

	for(h=0;h<height;h+=2)
	{
		wy=0;
		wuv=0;

		for(w=0;w<linesize;w+=4)
		{

			/*u0*/
			unsigned char u0 = *src++;
			unsigned char u1 = src[dstStride * 2 - 1];
			puv[wuv + offsetuv] = (u0 + u1) / 2;

			/*y00*/
			py[wy + offsety] = *src++;
			/*y10*/
			py[wy + offsety + width] =  src[dstStride  * 2 - 1];

			/*v0*/
			unsigned char v0 = *src++;
			unsigned char v1 = src[dstStride * 2 - 1];
			puv[(wuv + 1) + offsetuv] = (v0 + v1) / 2;

			/*y01*/
			py[(wy + 1) + offsety] = *src++;
			/*y11*/
			py[(wy + 1) + offsety + width] = src[dstStride * 2 - 1];

			wuv+=2;
			wy+=2;
		}

		src += dstStride * 2;
		offsety += width * 2;
		offsetuv +=  width;
	}
}


int main(int argc, const char *argv[])
{
	FILE *file;
	FILE *file1;
	int c = 0;
	char src[1280 * 720 * 2];
	char dst[1280 * 720 * 3/2];
	file = fopen(argv[1], "r");
	file1 = fopen(argv[2], "w+");
	if (!file){
		printf("open faild!!\n");
		exit(1);
	}
	fread(src, 1, sizeof(src), file);
	uyuv_to_nv12(src, 1280, dst, 1280, 720);
	fwrite(dst, 1, sizeof(dst), file1);
	return 0;
}
