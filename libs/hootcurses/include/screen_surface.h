#pragma once
#ifndef SURFACE
#define SURFACE

#include <stdbool.h>

typedef union 
{
  char c[4];
  int data;
  struct {
    unsigned char r;
    unsigned char b;
    unsigned char g;
    unsigned char chr;
  };
} fragment;

typedef struct
{
  int width;
  int height;
} surface_size;

typedef struct 
{
  fragment* data;
  int width;
  int height;
} screen_surface;

screen_surface*   create_surface(surface_size size);
surface_size      get_surface_size(screen_surface* surface);
fragment*         surface_at(screen_surface* surface, int x, int y);
fragment*         surface_index(screen_surface* surface);

bool              fragment_equal(fragment a, fragment b);
bool              color_equal(fragment a, fragment b);

void              fill_surface(screen_surface* surface, fragment frag);
void              clear_surface(screen_surface* surface);
void              put_at(screen_surface* surface, int x, int y, fragment frag);

#endif
