#include <stdio.h>
#include <stdlib.h>
#include "ImageIO.h"

#define BUFSIZE 256

int LoadPPMImage(const char *filename, ImageData *img)
{
	FILE *fp;
	char buf[BUFSIZE];
	int w, h, dummy, size, c;

	fp = fopen(filename, "rb");
	if ( fp == NULL )
	{
		fprintf(stderr, "LoadPPMImage: error: cannot open file: %s\n", filename);
		return 0;
	}

	fscanf(fp, "%s\n", buf);

	if ( buf[0] != 'P' || (buf[1] != '2' && buf[1] != '3' && buf[1] != '5' && buf[1] != '6') )
	{
		fprintf(stderr, "LoadPPMImage: error: format not supported: %s\n", buf);
		fclose(fp);
		return 0;
	}

	/* GIMP などで作った画像ファイルにはコメント行が入っているので、読み飛ばす */
	while ( (c =fgetc(fp)) == '#')
	{
		fgets(buf, BUFSIZE, fp);
	}

	/* 1 文字余計に読んでいるので、読み戻す */
	ungetc(c, fp);

	/* ヘッダをチェック */
	fscanf(fp, "%d %d %d\n", &w, &h, &dummy);

	if ( w <= 0 || h <= 0 )
	{
		fprintf(stderr, "LoadPPMImage: error: invalid size: %dx%d\n", w, h);
		fclose(fp);
		return 0;
	}

	img->width = w;
	img->height = h;
	img->channels = (buf[1] == '2' || buf[1] == '5') ? 1 : 3;

	size = w * h * img->channels;

	img->data = (unsigned char *)malloc(size);
	
	for (int i=0; i<size; i++)
	{
		int d;	
		fscanf(fp, "%d", &d);

		img->data[i] = (unsigned char)d;		
	}
	

	fclose(fp);

	fprintf(stderr, "LoadPPMImage: \"%s\" loaded\n", filename);

	return 1;
}

int SavePPMImage(const char *filename, ImageData *img)
{
	FILE *fp;
	int size, nWritten;

	fp = fopen(filename, "wb");
	if ( fp == NULL )
	{
		fprintf(stderr, "SavePPMImage: error: cannot open file: %s\n", filename);
		return 0;
	}

	if (img->channels == 1)	/* グレースケール */
	{
		fprintf(fp, "P5\n");
	}
	else if (img->channels == 3)	/* RGB */
	{
		fprintf(fp, "P6\n");
	}

	fprintf(fp, "%d %d\n255\n", img->width, img->height);

	size = img->width * img->height * img->channels;
	nWritten = fwrite(img->data, sizeof(unsigned char), size, fp);
	
	if ( size != nWritten )
	{
		fprintf(stderr, "SavePPMImage: cannot write full data: %d written\n", nWritten);
	}
	
	fclose(fp);

	fprintf(stderr, "SavePPMImage: \"%s\" saved\n", filename);

	return 1;
}
