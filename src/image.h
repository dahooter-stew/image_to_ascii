#pragma once
#ifndef IMAGE
#define IMAGE

#include "screen_surface.h"

typedef struct
{
  unsigned char* data;
  int width;
  int height;
  int channels;

} image;

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

typedef struct
{
  int* data;
  int width;
  int height;
} angles;

image             create_image(int x, int y, int channels);
image             load_image(char* name);
sobel             img_sobel(image* img);
angles            get_angles_ascii(sobel* sob);

void              delete_image(image* img);
void              delete_sobel(sobel* sob);
void              delete_angles(angles* ang);

unsigned char*    image_at(image* img, int x, int y);
float             luminance_at(image* img, int x, int y);
vec2*             sobel_at(sobel* sob, int x, int y);
int*              angles_at(angles* ang, int x, int y);

image             img_resize(image* img_from, int x, int y);
image             img_fit_to_terminal(image* img_from, int x, int y);
angles            downsize_angles(angles* ang, int x, int y);

void              display_image(screen_surface* surface, image* img, angles* ang);

void              add_saturation_threshold(float val);
void              add_luminance_threshold(float val);
void              add_maginitude_threshold(float val);

float             get_saturation(void);
float             get_luminance(void);
float             get_magnitude(void);

#endif
