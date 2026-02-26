#include "image.h"
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
  float r = (float)img->data[(y * img->width + x) * img->channels + 0] * 0.22f;
  float g = (float)img->data[(y * img->width + x) * img->channels + 1] * 0.71f;
  float b = (float)img->data[(y * img->width + x) * img->channels + 2] * 0.07f;
  return (r + g + b) / 255.0f;
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
