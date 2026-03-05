#include "hootcurses.h"
#include "image.h"
#include "sobel.h"

#include <stdio.h>

int main(int argc, char** argv)
{
  if (argc < 1)
  {
    printf("No file given\n");
    return -1;
  }
  printf("%s", argv[1]);

  init_hootcurses("img_ascii");
  surface_size size = get_surface_size(get_surface());

  // printf("LOAD IMAGE\n");
  image img = load_image(argv[1]);
  // printf("RESIZE IMAGE\n");
  image resized = img_fit_to_terminal(&img, size.width, size.height);
  if (!resized.data)
  {
    printf("invalid\n");
    return -1;
  }

  // printf("GET SOBEL\n");
  sobel vecs = img_sobel(&resized);
  // printf("GET ANGLES\n");
  angles ang = get_angles_ascii(&vecs);
  // angles ang_downsized = downsize_angles(&ang, resized.width, resized.height);

  while (true)
  {
    event e;
    while (poll_events(&e))
    {
      if (e.type == WINDOW_RESIZE)
      {
        size = get_surface_size(get_surface());

        delete_image(&resized);
        delete_sobel(&vecs);
        delete_angles(&ang);

        resized = img_fit_to_terminal(&img, size.width, size.height);
        if (!resized.data)
        {
          printf("invalid resized\n");
          return -1;
        }

        vecs = img_sobel(&resized);
        if (!vecs.data)
        {
          printf("invalid sobel\n");
          return -1;
        }

        ang = get_angles_ascii(&vecs);
        if (!ang.data)
        {
          printf("invalid angles\n");
          return -1;
        }
      }
    }

    // printf("DISPLAY IMAGE\n");
    display_image(get_surface(), &resized, &ang);

    // printf("DISPLAY CONTEXT\n");
    display_context();
  }

  delete_image(&img);
  delete_image(&resized);
  delete_sobel(&vecs);
  delete_angles(&ang);
  exit_hootcurses();
}
