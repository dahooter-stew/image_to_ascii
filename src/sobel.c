#include "sobel.h"

sobel sobel_x(image* img)
{
  sobel sob;
  sob.width = img->width;
  sob.height = img->height;
  sob.data = malloc(sob.width * sob.height * sizeof(vec2));

  for (int y = 0; y < sob.height; y++)
  {
    for (int x = 0; x < sob.width; x++)
    {


    }
  }

  return sob;
}

sobel sobel_y(image* img)
{
  sobel sob;
  sob.width = img->width;
  sob.height = img->height;
  sob.data = malloc(sob.width * sob.height * sizeof(vec2));

  for (int y = 0; y < sob.height; y++)
  {
    for (int x = 0; x < sob.width; x++)
    {


    }
  }

  return sob;
}

sobel downsize_sobel(sobel* sob, int x, int y)
{

}
