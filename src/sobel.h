#pragma once
#ifndef SOBEL
#define SOBEL

#include "image.h"
#include "sobel_obj.h"

sobel     img_sobel(image* img);
angles    get_angles_ascii(sobel* sob);

angles    downsize_angles(angles* ang, int x, int y);

vec2*     sobel_at(sobel* sob, int x, int y);
int*      angles_at(angles* ang, int x, int y);

void      delete_sobel(sobel* sob);
void      delete_angles(angles* ang);

#endif
