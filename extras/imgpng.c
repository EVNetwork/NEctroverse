/* -----------------------------------------------------------------------------
 *
 * Copyright (c) 2007-2012 Alexis Naveros. All Rights Reserved.
 *
 * Portions developed under contract to the SURVICE Engineering Company.
 * No redistribution without express written permission.
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include <png.h>


#include "imgpng.h"



typedef struct
{
  void *data;
  size_t datasize;
} pngReadState;


static void pngReadChunk( png_structp png_ptr, png_bytep data, png_size_t length )
{
  pngReadState *datapos;
  datapos = png_get_io_ptr( png_ptr );
  if( length <= datapos->datasize )
  {
    memcpy( data, datapos->data, length );
    datapos->data += length;
    datapos->datasize -= length;
  }
  return;
}


void imgConvertGrayscale( imgImage *image, int desiredformat )
{
  int i, pixelcount;
  char *newdata, *src, *dst;
  if( image->format != IMG_IMAGE_FORMAT_GRAYSCALE )
    return;

  if( ( desiredformat == IMG_IMAGE_FORMAT_RGB24 ) || ( desiredformat == IMG_IMAGE_FORMAT_BGR24 ) )
  {
    newdata = malloc( image->width * image->height * 3 );
    src = image->data;
    dst = newdata;
    pixelcount = image->width * image->height;
    for( i = 0 ; i < pixelcount ; i++ )
    {
      dst[0] = src[i];
      dst[1] = src[i];
      dst[2] = src[i];
      dst += 3;
    }
    free( image->data );
    image->data = newdata;
    image->format = desiredformat;
    image->bytesperpixel = 3;
    image->bytesperline = image->width * image->bytesperpixel;
  }
  else if( ( desiredformat == IMG_IMAGE_FORMAT_RGBA32 ) || ( desiredformat == IMG_IMAGE_FORMAT_BGRA32 ) )
  {
    newdata = malloc( image->width * image->height * 4 );
    src = image->data;
    dst = newdata;
    pixelcount = image->width * image->height;
    for( i = 0 ; i < pixelcount ; i++ )
    {
      dst[0] = src[i];
      dst[1] = src[i];
      dst[2] = src[i];
      dst[3] = src[i];
      dst += 4;
    }
    free( image->data );
    image->data = newdata;
    image->format = desiredformat;
    image->bytesperpixel = 4;
    image->bytesperline = image->width * image->bytesperpixel;
  }

  return;
}


int imgReadPngData( imgImage *image, void *data, size_t datasize, int desiredformat )
{
  int i, colortype;
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep *rows;
  pngReadState datapos;

  rows = 0;

  png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
  if( !png_ptr )
    return 0;
  info_ptr = png_create_info_struct( png_ptr );
  if( !info_ptr )
  {
    png_destroy_read_struct( &png_ptr, 0, 0 );
    return 0;
  }

  if( !png_check_sig( data, datasize ) )
    return 0;

  if( setjmp( png_jmpbuf( png_ptr ) ) )
  {
    error:
    png_destroy_read_struct( &png_ptr, &info_ptr, 0 );
    if( rows )
      free( rows );
    return 0;
  }

  datapos.data = data;
  datapos.datasize = datasize;
  png_set_read_fn( png_ptr, &datapos, pngReadChunk );

  png_read_info( png_ptr, info_ptr );
  colortype = png_get_color_type( png_ptr, info_ptr );
  if( colortype == PNG_COLOR_TYPE_PALETTE )
    png_set_expand( png_ptr );
  if( ( colortype == PNG_COLOR_TYPE_GRAY ) && ( png_get_bit_depth( png_ptr, info_ptr ) < 8 ) )
    png_set_expand( png_ptr );
  if( png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS ) )
    png_set_expand( png_ptr );
  if( png_get_bit_depth( png_ptr, info_ptr ) == 16 )
    png_set_strip_16( png_ptr );

#if 1
  if( ( desiredformat == IMG_IMAGE_FORMAT_RGBA32 ) || ( desiredformat == IMG_IMAGE_FORMAT_BGRA32 ) )
  {
    if( colortype == PNG_COLOR_TYPE_RGB )
    {
      png_set_filler( png_ptr, 0xff, PNG_FILLER_AFTER );
      colortype = PNG_COLOR_TYPE_RGB_ALPHA;
    }
  }
  if( ( desiredformat == IMG_IMAGE_FORMAT_RGB24 ) || ( desiredformat == IMG_IMAGE_FORMAT_BGR24 ) )
  {
    if( colortype & PNG_COLOR_MASK_ALPHA )
    {
      png_set_strip_alpha( png_ptr );
      colortype = PNG_COLOR_TYPE_RGB;
    }
  }
  if( ( desiredformat == IMG_IMAGE_FORMAT_BGR24 ) || ( desiredformat == IMG_IMAGE_FORMAT_BGRA32 ) )
    png_set_bgr( png_ptr );
#endif

  png_read_update_info( png_ptr, info_ptr );
  image->width = png_get_image_width( png_ptr, info_ptr );
  image->height = png_get_image_height( png_ptr, info_ptr );

  /* CHECKME: Can we safely update colortype to account for the changes performed? */
#if 1
  colortype = png_get_color_type( png_ptr, info_ptr );
#endif
  if( colortype == PNG_COLOR_TYPE_RGB )
  {
    image->format = IMG_IMAGE_FORMAT_RGB24;
    image->bytesperpixel = 3;
  }
  else if( colortype == PNG_COLOR_TYPE_RGB_ALPHA )
  {
    image->format = IMG_IMAGE_FORMAT_RGBA32;
    image->bytesperpixel = 4;
  }
  else if( colortype == PNG_COLOR_TYPE_GRAY )
  {
    image->format = IMG_IMAGE_FORMAT_GRAYSCALE;
    image->bytesperpixel = 1;
  }
  else
    goto error;
  image->bytesperline = image->width * image->bytesperpixel;
  image->data = malloc( image->height * image->bytesperline );

  rows = malloc( image->height * sizeof(png_bytep) );
  for( i = 0 ; i < image->height ; i++ )
    rows[i] = (png_bytep)( image->data + ( i * image->bytesperline ) );

  png_read_image( png_ptr, rows );

  png_read_end( png_ptr, info_ptr );
  png_destroy_read_struct( &png_ptr, &info_ptr, 0 );

  free( rows );

  if( ( image->format == IMG_IMAGE_FORMAT_GRAYSCALE ) && ( desiredformat != IMG_IMAGE_FORMAT_GRAYSCALE ) )
    imgConvertGrayscale( image, desiredformat );

  return 1;
}



int imgReadPngFile( imgImage *image, char *filename, int desiredformat )
{
  int ret;
  void *data;
  FILE *file;
  struct stat filestat;

  ret = 0;
  data = 0;
  file = fopen( filename, "rb" );
  if( !file )
    goto done;
  if( fstat( fileno( file ), &filestat ) )
    goto done;
  if( !( data = malloc( filestat.st_size ) ) )
    goto done;
  if( !( fread( data, filestat.st_size, 1, file ) ) )
    goto done;
  if( !( imgReadPngData( image, data, filestat.st_size, desiredformat ) ) )
    goto done;
  ret = 1;

  done:
  if( file )
    fclose( file );
  if( data )
    free( data );
  return ret;
}


void imgFree( imgImage *image )
{
  free( image->data );
  image->data = 0;
  return;
}



int imgWritePngFile( char *filename, imgImage *image )
{
  int i, x, colortype, formatmatch;
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep *rows;
  FILE *fp;
  unsigned char *src;
  png_byte *dst;

  fp = fopen( filename, "wb" );
  if( !( fp ) )
    return 0;

  png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
  if( !png_ptr )
    return 0;
  info_ptr = png_create_info_struct( png_ptr );
  if( !info_ptr )
  {
    png_destroy_write_struct( &png_ptr, &info_ptr );
    fclose( fp );
    return 0;
  }

  if( setjmp( png_jmpbuf( png_ptr ) ) )
  {
    png_destroy_write_struct( &png_ptr, &info_ptr );
    fclose( fp );
    return 0;
  }

  png_set_compression_level( png_ptr, 9 );
  png_init_io( png_ptr, fp );

  if( image->format == IMG_IMAGE_FORMAT_ANY )
  {
    if( image->bytesperpixel == 1 )
      image->format = IMG_IMAGE_FORMAT_GRAYSCALE;
    else if( image->bytesperpixel == 3 )
      image->format = IMG_IMAGE_FORMAT_RGB24;
    else if( image->bytesperpixel == 4 )
      image->format = IMG_IMAGE_FORMAT_RGBA32;
  }

  if( ( image->format == IMG_IMAGE_FORMAT_RGB24 ) || ( image->format == IMG_IMAGE_FORMAT_BGR24 ) )
    colortype = PNG_COLOR_TYPE_RGB;
  else if( ( image->format == IMG_IMAGE_FORMAT_RGBA32 ) || ( image->format == IMG_IMAGE_FORMAT_BGRA32 ) )
    colortype = PNG_COLOR_TYPE_RGB_ALPHA;
  else if( image->format == IMG_IMAGE_FORMAT_GRAYSCALE )
    colortype = PNG_COLOR_TYPE_GRAY;
  else
  {
    png_destroy_write_struct( &png_ptr, &info_ptr );
    fclose( fp );
    return 0;
  }

  png_set_IHDR( png_ptr, info_ptr, image->width, image->height, 8, colortype, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
  if( ( image->format == IMG_IMAGE_FORMAT_BGR24 ) || ( image->format == IMG_IMAGE_FORMAT_BGRA32 ) )
    png_set_bgr( png_ptr );
  png_write_info( png_ptr, info_ptr );

  formatmatch = 0;
  if( ( image->bytesperpixel == 4 ) && ( ( image->format == IMG_IMAGE_FORMAT_RGBA32 ) || ( image->format == IMG_IMAGE_FORMAT_BGRA32 ) ) )
    formatmatch = 1;
  else if( ( image->bytesperpixel == 3 ) && ( ( image->format == IMG_IMAGE_FORMAT_RGB24 ) || ( image->format == IMG_IMAGE_FORMAT_BGR24 ) ) )
    formatmatch = 1;
  else if( ( image->bytesperpixel == 1 ) && ( image->format == IMG_IMAGE_FORMAT_GRAYSCALE ) )
    formatmatch = 1;

  if( formatmatch )
  {
    rows = malloc( image->height * sizeof(png_bytep) );
    for( i = 0 ; i < image->height ; i++ )
      rows[i] = (png_bytep)( image->data + ( i * image->bytesperline ) );
    png_write_image( png_ptr, rows );
    free( rows );
  }
  else if( ( image->bytesperpixel == 4 ) && ( ( image->format == IMG_IMAGE_FORMAT_RGB24 ) || ( image->format == IMG_IMAGE_FORMAT_BGR24 ) ) )
  {
    png_byte *row;
    row = malloc( image->bytesperline );
    for( i = 0 ; i < image->height ; i++ )
    {
      src = (png_bytep)( image->data + ( i * image->bytesperline ) );
      dst = row;
      for( x = 0 ; x < image->bytesperline ; x += 4, dst += 3 )
      {
        dst[0] = src[x+0];
        dst[1] = src[x+1];
        dst[2] = src[x+2];
      }
      png_write_rows( png_ptr, &row, 1 );
    }
    free( row );
  }
  else
  {
    png_destroy_write_struct( &png_ptr, &info_ptr );
    fclose( fp );
    return 0;
  }
  png_write_end( png_ptr, NULL );

  fclose( fp );

  return 1;
}




