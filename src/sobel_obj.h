#pragma once
#ifndef SOBEL_OBJ
#define SOBEL_OBJ

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

#endif 
