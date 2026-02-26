#include "screen_surface.h"

#include <stdlib.h>

screen_surface* create_surface(surface_size size)
{
  screen_surface* surface = malloc(sizeof(screen_surface));
  surface->data = malloc(size.width * size.height * sizeof(fragment));
  surface->width = size.width;
  surface->height = size.height;

  return surface;
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
