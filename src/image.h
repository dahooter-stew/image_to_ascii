#pragma once
#ifndef IMAGE
#define IMAGE

#include "stb_image.h"

typedef struct
{
  unsigned char* data;
  int width;
  int height;
  int channels;

} image;

image create_image(int x, int y, int channels);
image load_image(char* name);
void delete_image(image* img);

unsigned char* image_at(image* img, int x, int y);
float luminance_at(image* img, int x, int y);

image img_resize(image* img_from, int x, int y);

#endif
