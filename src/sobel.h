#pragma once
#ifndef SOBEL
#define SOBEL

#include "image.h"

typedef struct
{
  float x, y;
} vec2;

typedef struct
{
  vec2* data;
  int width;
  int height;
} sobel;

sobel sobel_x(image* img);
sobel sobel_y(image* img);

sobel downsize_sobel(sobel* sob, int x, int y);

#endif
