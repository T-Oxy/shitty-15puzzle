#ifndef __IMAGE_DATA_H__
#define __IMAGE_DATA_H__

typedef struct {
	int width, height, channels;
	unsigned char *data;
} ImageData;

#define PIXEL_CHANNEL(img,xi,yi,c) ((img)->data[(img)->channels*((xi)+(img)->width*(yi))+(c)])

int IsImageDataAllocated(ImageData *img);

void InitImageData(ImageData *img);

int AllocateImageData(ImageData *img, int _w, int _h, int _channels);

void ReleaseImageData(ImageData *img);

void CopyImageData(ImageData *imgIn, ImageData *imgOut);

void DrawImageData(ImageData *img, int xi, int yi);

void FlipImageData(ImageData *img);

#endif

