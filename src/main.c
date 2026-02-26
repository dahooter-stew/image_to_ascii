#include "hootcurses.h"
#include "image.h"

const char* lum = " .+=#@";

int main(void)
{
  init_hootcurses("TEST");
  surface_size size = get_surface_size(get_surface());

  image img = load_image("test.jpg");
  image resized = img_resize(&img, size.width, size.height);
  if (!resized.data)
  {
    printf("invalid\n");
    return -1;
  }

  clear_surface(get_surface());

  for (int x = 0; x < resized.width; x++)
  {
    for (int y = 0; y < resized.height; y++)
    {
      unsigned char r = image_at(&resized, x, y)[0];
      unsigned char g = image_at(&resized, x, y)[1];
      unsigned char b = image_at(&resized, x, y)[2];
      float luminance = luminance_at(&resized, x, y);

      *surface_at(get_surface(), x, y) = (fragment){
        .r = r, .g = g, .b = b, .chr = lum[(int)(luminance * 6)]
      };
    }
  }

  display_context();

  delete_image(&img);
  delete_image(&resized);

  // while (true)
  // {
  //   event e;
  //   while (poll_events(&e))
  //   {
  //   }

  //   clear_surface(get_surface());


  //   display_context();
  // }
}
