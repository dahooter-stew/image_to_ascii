#include "image.h"

#include "screen_surface.h"

#include "stb_image.h"
#include "stb_image_resize2.h"

#include <math.h>

static char* SLOPES = " |\\_/|";
static float  SATURATION_THRESHOLD  = 0.85f;
static float  LUMINANCE_THRESHOLD   = 0.15f;
static float  MAGNITUDE_THRESHOLD   = 0.25f;

static float mat_at(float* mat, int x, int y)
{
  return mat[y * 3 + x];
}

static float mat_conv(float* gray, int width, int height, float* mat, int cx, int cy)
{
  float sum = 0;

  for (int i = -1; i < 2; i++)
  {
    for (int j = -1; j < 2; j++)
    {
      int x = cx + j;
      int y = cy + i;
      if (x < 0 || y < 0 || x > width - 1 || y > height - 1)
        continue;

      sum = sum + gray[y * width + x] * mat_at(mat, j + 1, i + 1);
    }
  }

  return sum;
}

static int angles_mode_block(angles* ang, int start_x, int start_y, int bw, int bh)
{
  int counts[6] = {0};

  int end_x = start_x + bw;
  int end_y = start_y + bh;

  if (end_x > ang->width)  end_x = ang->width;
  if (end_y > ang->height) end_y = ang->height;

  int total = 0;

  for (int y = start_y; y < end_y; y++)
  {
    for (int x = start_x; x < end_x; x++)
    {
      int index = *angles_at(ang, x, y);

      if (index >= 0 && index < 6)
      {
        counts[index]++;
        total++;
      }
    }
  }

  if (total == 0)
    return 0;

  int max_index = 0;
  for (int i = 1; i < 6; i++)
  {
    if (counts[i] > counts[max_index])
      max_index = i;
  }

  int threshold = (bw * bh) / 4;

  if (counts[max_index] > threshold)
    return max_index;

  return 0;
}

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

sobel img_sobel(image* img)
{
  static float mat_x[9] = {
    -1.0f, 0.0f, 1.0f,
    -2.0f, 0.0f, 2.0f,
    -1.0f, 0.0f, 1.0f
  };

  static float mat_y[9] = {
     1.0f,  2.0f, 1.0f,
     0.0f,  0.0f, 0.0f,
    -1.0f, -2.0f, -1.0f
  };

  float* grayscale = malloc(img->width * img->height * sizeof(float));
  for (int y = 0; y < img->height; y++)
    for (int x = 0; x < img->width; x++)
      grayscale[y * img->width + x] = luminance_at(img, x, y);

  sobel sob;
  sob.width = img->width;
  sob.height = img->height;
  sob.data = malloc(sob.width * sob.height * sizeof(vec2));

  for (int y = 0; y < sob.height; y++)
  {
    for (int x = 0; x < sob.width; x++)
    {
      float dx = mat_conv(grayscale, sob.width, sob.height, mat_x, x, y);
      float dy = mat_conv(grayscale, sob.width, sob.height, mat_y, x, y);
      sob.data[y * sob.width + x] = (vec2){ .x = dx, .y = dy};
    }
  }

  free(grayscale);

  return sob;
}

angles get_angles_ascii(sobel* sob)
{
  angles ang;
  ang.data = malloc(sob->width * sob->height * sizeof(int));
  ang.width = sob->width;
  ang.height = sob->height;

  for (int y = 0; y < ang.height; y++)
  {
    for (int x = 0; x < ang.width; x++)
    {
      vec2 vec = *sobel_at(sob, x, y);
      float angle = atan2f(vec.y, vec.x);
      int index = 5.0f * (angle / (M_PI * 2.0f) + 0.5f) + 1.0f;

      float mag = sqrtf(vec.x * vec.x + vec.y * vec.y);
      if (mag < MAGNITUDE_THRESHOLD)
        index = 0;

      *angles_at(&ang, x, y) = index;
    }
  }

  return ang;
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
  if (x < 0 || y < 0 || x > img->width - 1 || y > img->height - 1)
    return NULL;

  return &img->data[(y * img->width + x) * img->channels];
}

float luminance_at(image* img, int x, int y)
{
  float r = (float)img->data[(y * img->width + x) * img->channels + 0] * 0.21f;
  float g = (float)img->data[(y * img->width + x) * img->channels + 1] * 0.72f;
  float b = (float)img->data[(y * img->width + x) * img->channels + 2] * 0.07f;
  return (r + g + b) / (255.0f);
}

vec2* sobel_at(sobel* sob, int x, int y)
{
  return &sob->data[y * sob->width + x];
}

int* angles_at(angles* ang, int x, int y)
{
  return &ang->data[y * ang->width + x];
}

void delete_sobel(sobel* sob)
{
  free(sob->data);
  sob->data = NULL;
  sob->width = 0;
  sob->height = 0;
}

void delete_angles(angles* ang)
{
  free(ang->data);
  ang->data = NULL;
  ang->width = 0;
  ang->height = 0;
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
    new_w = x;
    new_h = (int)((float)x / ar) / 2;
  }
  else
  {
    new_h = y;
    new_w = (int)((float)y * ar) * 2;
  }

  return img_resize(img_from, new_w, new_h);
}

angles downsize_angles(angles* ang, int new_w, int new_h)
{
  angles downsized;
  downsized.width = new_w;
  downsized.height = new_h;
  downsized.data = malloc(new_w * new_h * sizeof(int));

  int bw = ang->width  / new_w;
  int bh = ang->height / new_h;

  if (bw <= 0) bw = 1;
  if (bh <= 0) bh = 1;

  for (int dy = 0; dy < new_h; dy++)
  {
    for (int dx = 0; dx < new_w; dx++)
    {
      int start_x = dx * bw;
      int start_y = dy * bh;

      int angle = angles_mode_block(
        ang,
        start_x,
        start_y,
        bw,
        bh
      );

      *angles_at(&downsized, dx, dy) = angle;
    }
  }

  return downsized;
}

void display_image(screen_surface* surface, image* img, angles* ang)
{
  static const char* lum = " .:-=+*#%@";
  surface_size size = get_surface_size(surface);

  clear_surface(surface);

  for (int y = 0; y < size.height; y++)
  {
    for (int x = 0; x < size.width; x++)
    {
      if (x < 0 || y > img->height - 1 || y < 0 || x > ang->width - 1 || y > ang->height - 1)
        continue;

      unsigned char r = image_at(img, x, y)[0];
      unsigned char g = image_at(img, x, y)[1];
      unsigned char b = image_at(img, x, y)[2];

      color hsl = to_hsl((fragment){.r = r, .g = g, .b = b});

      unsigned char chr = lum[(int)(hsl.l * 10)];

      int slope_index = *angles_at(ang, x, y);
      if (slope_index > 0)
        chr = SLOPES[slope_index];


      if (hsl.s > SATURATION_THRESHOLD)
      {
        r = 255;
        g = 255;
        b = 255;
        chr = '@';
      }

      if (hsl.l < LUMINANCE_THRESHOLD)
        chr = ' ';

      *surface_at(surface, x, y) = (fragment){
        .r = r, .g = g, .b = b, .chr = chr
      };
    }
  }
}

void add_saturation_threshold(float val)
{
  SATURATION_THRESHOLD += val;
}

void add_luminance_threshold(float val)
{
  LUMINANCE_THRESHOLD += val;
}

void add_maginitude_threshold(float val)
{
  MAGNITUDE_THRESHOLD += val;
}

float get_saturation(void)
{
  return SATURATION_THRESHOLD;
}

float get_luminance(void)
{
  return LUMINANCE_THRESHOLD;
}

float get_magnitude(void)
{
  return MAGNITUDE_THRESHOLD;
}
