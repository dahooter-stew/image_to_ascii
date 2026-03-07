#include "hootcurses.h"
#include "key_handler.h"
#include "image.h"

#include <stdio.h>

#include <unistd.h>



static char* TITLE = NULL;
static bool RUN = false;

static surface_size SIZE;
static image IMG;
static image RESIZED;
static sobel VECS;
static angles ANGS;



static int resize_images(void)
{
  SIZE = get_surface_size(get_surface());

  delete_image(&RESIZED);
  delete_sobel(&VECS);
  delete_angles(&ANGS);

  RESIZED = img_fit_to_terminal(&IMG, SIZE.width - 1, SIZE.height - 1);
  if (!RESIZED.data)
  {
    printf("invalid resized\n");
    return -1;
  }

  VECS = img_sobel(&RESIZED);
  if (!VECS.data)
  {
    printf("invalid sobel\n");
    return -1;
  }

  ANGS = get_angles_ascii(&VECS);
  if (!ANGS.data)
  {
    printf("invalid angles\n");
    return -1;
  }

  return 0;
}

static void handle_events(void)
{
  event e;
  while (poll_events(&e))
  {
    if (e.type == WINDOW_RESIZE) 
    {
      if (resize_images() == -1)
        RUN = false;
    }
  }
}

static void key_presses(void)
{
  key_listen();

  float saturation_add  = 0.0f;
  float luminance_add   = 0.0f;
  float magnitude_add   = 0.0f;

  if (get_key('q').pressed)
    RUN = false;
}

static void show_image(void)
{
  display_image(get_surface(), &RESIZED, &ANGS);
  for (int i = 0; TITLE[i]; i++)
    *surface_at(get_surface(), i, SIZE.height - 1) = (fragment){.r = 137, .g = 215, .b = 67, .chr = TITLE[i]};

  display_context();
}

int init(char* filename)
{
  RUN = true;
  TITLE = filename;

  init_hootcurses(filename);
  init_key_handler();

  IMG       =   load_image(filename);
  RESIZED   =   (image){NULL, 0, 0, 0};
  VECS      =   (sobel){NULL, 0, 0};
  ANGS      =   (angles){NULL, 0, 0};

  resize_images();

  return 0;
}

void run(void)
{
  while (RUN)
  {
    handle_events();
    key_presses();
    show_image();
  }
}

void on_exit(void)
{
  delete_image(&IMG);
  delete_image(&RESIZED);
  delete_sobel(&VECS);
  delete_angles(&ANGS);
  exit_hootcurses();
}
