#ifndef __IMAGE_IO_H__
#define __IMAGE_IO_H__

#include "ImageData.h"

int LoadPPMImage(const char *filename, ImageData *img);

int SavePPMImage(const char *filename, ImageData *img);


#endif

