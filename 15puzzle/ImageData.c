#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include "ImageData.h"

/*画像データのメモリ確保判定*/
int IsImageDataAllocated(ImageData *img)	
{
	return (img->data != NULL);
}

/*画像データの初期化*/
void InitImageData(ImageData *img)	
{
	img->data = NULL;
}

/*画像データのメモリ確保とメンバ変数の代入*/
int AllocateImageData(ImageData *img, int w, int h, int channels)
{
	if (img->data != NULL)
		ReleaseImageData(img);

	img->width = w;
	img->height = h;
	img->channels = channels;
	img->data = malloc(sizeof(unsigned char)*w*h*channels);

	if ( img->data == NULL )
	{
		fprintf(stderr, "AllocateImage: cannot allocate memory\n");
		return 0;
	}

	return 1;
}

/*画像データのメモリ解放*/
void ReleaseImageData(ImageData *img)
{
	if (img->data != NULL)
		free(img->data);

	img->data = NULL;
}

/*画像データをコピー*/
void CopyImageData(ImageData *imgIn, ImageData *imgOut)
{
	int size;

	AllocateImageData(imgOut, imgIn->width, imgIn->height, imgIn->channels);

	size  = sizeof(unsigned char) * imgIn->width * imgIn->height * imgIn->channels;
	memcpy(imgOut->data, imgIn->data, size);
}

/*画像データを描画*/
void DrawImageData(ImageData *img, int xi, int yi)
{
	GLenum format = (img->channels == 3) ? GL_RGB : GL_LUMINANCE;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glRasterPos2i(xi, yi);
	glDrawPixels(img->width, img->height, format, GL_UNSIGNED_BYTE, img->data);
	glRasterPos2i(0, 0);
}

/*画像データを上下反転(原点の違いを解消)*/
void FlipImageData(ImageData *img)
{
	int xi, yi, ci;

	//fprintf(stderr, "FlipImage: image size %dx%d\n", img->width, img->height);

	for (yi=0; yi<img->height/2; yi++)
	{
		const int yFlipped = img->height - 1 - yi;

		for (xi=0; xi<img->width; xi++)
		{
			for (ci=0; ci<img->channels; ci++)
			{
				unsigned char tmp = PIXEL_CHANNEL(img,xi,yi,ci);
				PIXEL_CHANNEL(img,xi,yi,ci) = PIXEL_CHANNEL(img,xi,yFlipped,ci);
				PIXEL_CHANNEL(img,xi,yFlipped,ci) = tmp;
			}
		}
	}
}
