#include "hootcurses.h"
#include "image.h"

int main(int argc, char** argv)
{
  if (argc < 1)
    return -1;
  printf("%s", argv[1]);

  init_hootcurses("img_ascii");
  surface_size size = get_surface_size(get_surface());

  image img = load_image(argv[1]);
  image resized = img_fit_to_terminal(&img, size.width, size.height);
  if (!resized.data)
  {
    printf("invalid\n");
    return -1;
  }

  display_image(get_surface(), &resized);

  while (true)
  {
    event e;
    while (poll_events(&e))
    {
      if (e.type == WINDOW_RESIZE)
      {
        size = get_surface_size(get_surface());

        delete_image(&resized);
        resized = img_fit_to_terminal(&img, size.width, size.height);
        if (!resized.data)
        {
          printf("invalid\n");
          return -1;
        }

        display_image(get_surface(), &resized);
      }
    }

    display_context();
  }

  delete_image(&img);
  delete_image(&resized);
}
