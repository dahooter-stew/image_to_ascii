#include "sobel.h"

#include <stdlib.h>
#include <math.h>

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
      if (mag < 0.25f)
        index = 0;

      *angles_at(&ang, x, y) = index;
    }
  }

  return ang;
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
