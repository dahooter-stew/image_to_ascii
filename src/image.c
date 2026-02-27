#include "image.h"
#include "screen_surface.h"
#include "stb_image.h"
#include "stb_image_resize2.h"

image create_image(int x, int y, int channels)
{
  image img;
  img.data = malloc(x * y * channels * sizeof(unsigned char));
  img.width = x;
  img.height = y; 
  img.channels = channels;

  return img;
}

image load_image(char* name)
{
  image img; 
  img.data = stbi_load(name, &img.width, &img.height, &img.channels, 0);

  if (!img.data)
  {
    img.data = NULL;
    img.width = 0;
    img.height = 0;
    img.channels = 0;
  }

  return img;
}

void delete_image(image* img)
{
  free(img->data);
  img->data = NULL;
  img->width = 0;
  img->height = 0;
  img->channels = 0;
}

unsigned char* image_at(image* img, int x, int y)
{
  return &img->data[(y * img->width + x) * img->channels];
}

float luminance_at(image* img, int x, int y)
{
  float r = (float)img->data[(y * img->width + x) * img->channels + 0];
  float g = (float)img->data[(y * img->width + x) * img->channels + 1];
  float b = (float)img->data[(y * img->width + x) * img->channels + 2];
  return (r + g + b) / (255.0f * 3.0);
}

image img_resize(image* img_from, int x, int y)
{
  image img_to = create_image(x, y, img_from->channels);

  unsigned char* status = stbir_resize_uint8_linear(
    img_from->data, 
    img_from->width, 
    img_from->height, 
    0, 
    img_to.data, 
    img_to.width, 
    img_to.height, 
    0, 
    (stbir_pixel_layout)img_from->channels
  );

  if (status)
    return img_to;
  else
  {
    delete_image(&img_to);
    return (image){.data = NULL, .width = 0, .height = 0, .channels = 0};
  }
}

image img_fit_to_terminal(image* img_from, int x, int y)
{
  float ar = (float)img_from->width / (float)img_from->height;
  int new_w, new_h;
  if (x < y)
  {
    new_w = x * 2;
    new_h = (int)((float)x / ar);
  }
  else
  {
    new_h = y;
    new_w = (int)((float)y * ar) * 2;
  }

  return img_resize(img_from, new_w, new_h);
}

void display_image(screen_surface* surface, image* img)
{
  static const char* lum = " .:-=+*#%@";
  surface_size size = get_surface_size(surface);

  clear_surface(surface);

  for (int y = 0; y < size.height; y++)
  {
    for (int x = 0; x < size.width; x++)
    {
      if (x > img->width - 1 || x < 0 || y > img->height - 1 || y < 0)
        continue;

      unsigned char r = image_at(img, x, y)[0];
      unsigned char g = image_at(img, x, y)[1];
      unsigned char b = image_at(img, x, y)[2];

      color hsl = to_hsl((fragment){.r = r, .g = g, .b = b});

      unsigned char chr = lum[(int)(hsl.l * 10)];

      *surface_at(surface, x, y) = (fragment){
        .r = r, .g = g, .b = b, .chr = chr
      };
    }
  }
}
