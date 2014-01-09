#ifndef _IMGPNG_H
#define _IMGPNG_H
/* -----------------------------------------------------------------------------
 *
 * Copyright (c) 2007-2012 Alexis Naveros. All Rights Reserved.
 *
 * Portions developed under contract to the SURVICE Engineering Company.
 * No redistribution without express written permission.
 *
 * -----------------------------------------------------------------------------
 */

typedef struct
{
  int width;
  int height;
  int format;
  int bytesperpixel;
  int bytesperline;
  void *data;
} imgImage;

#define IMG_IMAGE_FORMAT_ANY (0x0)
#define IMG_IMAGE_FORMAT_RGB24 (0x1)
#define IMG_IMAGE_FORMAT_BGR24 (0x2)
#define IMG_IMAGE_FORMAT_RGBA32 (0x3)
#define IMG_IMAGE_FORMAT_BGRA32 (0x4)
#define IMG_IMAGE_FORMAT_GRAYSCALE (0x5)


/* Format can be zero to read file as it is, without transformation */
int imgReadPngData( imgImage *image, void *data, size_t datasize, int desiredformat );
int imgReadPngFile( imgImage *image, char *filename, int desiredformat );
int imgWritePngFile( char *filename, imgImage *image );

void imgConvertGrayscale( imgImage *image, int desiredformat );
void imgFree( imgImage *image );


#endif
