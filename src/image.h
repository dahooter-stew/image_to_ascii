#pragma once
#ifndef IMAGE
#define IMAGE

#include "screen_surface.h"
#include "sobel_obj.h"

typedef struct
{
  unsigned char* data;
  int width;
  int height;
  int channels;

} image;

image             create_image(int x, int y, int channels);
image             load_image(char* name);
void              delete_image(image* img);

unsigned char*    image_at(image* img, int x, int y);
float             luminance_at(image* img, int x, int y);

image             img_resize(image* img_from, int x, int y);
image             img_fit_to_terminal(image* img_from, int x, int y);

void              display_image(screen_surface* surface, image* img, angles* ang);

#endif
