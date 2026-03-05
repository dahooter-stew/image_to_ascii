#include "screen_surface.h"

#include <stdlib.h>
#include <math.h>

screen_surface* create_surface(surface_size size)
{
  screen_surface* surface = malloc(sizeof(screen_surface));
  surface->data = malloc(size.width * size.height * sizeof(fragment));
  surface->width = size.width;
  surface->height = size.height;

  return surface;
}

void delete_surface(screen_surface* surface)
{
  free(surface->data);
  free(surface);
}

surface_size get_surface_size(screen_surface* surface)
{
  return (surface_size) {
    .width = surface->width,
    .height = surface->height
  };
}

fragment* surface_at(screen_surface* surface, int x, int y)
{
  surface_size size = get_surface_size(surface);
  if (x < 0 || x > size.width - 1 || y < 0 || y > size.height)
    return NULL;

  return &surface->data[y * surface->width + x ];
}

fragment* surface_index(screen_surface* surface)
{
  return surface->data;
}

color to_hsl(fragment p)
{
  float r = (float)p.r / 255.0f;
  float g = (float)p.g / 255.0f;
  float b = (float)p.b / 255.0f;

  float max = fmaxf(fmaxf(r, g), b);
  float min = fminf(fmaxf(r, g), b);

  float luminance = (max + min) / 2.0f;
  float saturation;
  float hue;

  if (max == min)
  {
    hue = 0.0f;
    saturation = 0.0f;
  }
  if (luminance > 0.5f)
    saturation = (max - min) / (2.0f - max - min);
  else
    saturation = (max - min) / (max + min);

  if (r > g && r > b)
    hue = (g - b) / (max - min);
  else if (g > r && g > b)
    hue = 2.0f + (b - r) / (max - min);
  else if (b > r && b > g)
    hue = 4.0f + (r - g) / (max - min);

  hue *= 60.0f;
  if (hue < 0.0f)
    hue = 360.0f;

  return (color){
    .h = hue, .s = saturation, .l = luminance
  };
}

bool fragment_equal(fragment a, fragment b)
{
  return (a.r == b.r && a.g == b.g && a.b == b.b && a.chr == b.chr);
}

bool color_equal(fragment a, fragment b)
{
  return (a.r == b.r && a.g == b.g && a.b == b.b);
}

void fill_surface(screen_surface* surface, fragment frag)
{
  surface_size size = get_surface_size(surface);
  for (int i = 0; i < size.width * size.height; i++)
    surface_index(surface)[i] = frag;
}

void clear_surface(screen_surface* surface)
{
  fill_surface(surface, (fragment){ .r = 0, .g = 0, .b = 0, .chr = ' ' });
}
