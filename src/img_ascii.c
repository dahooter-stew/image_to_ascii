#include "hootcurses.h"
#include "key_handler.h"
#include "image.h"
#include "timer.h"

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

static void show_image(void)
{
  display_image(get_surface(), &RESIZED, &ANGS);

  int offset = 0;

  for (int i = offset; TITLE[i]; i++, offset++)
    *surface_at(get_surface(), offset, SIZE.height - 1) = (fragment){.r = 137, .g = 215, .b = 67, .chr = TITLE[i]};

  char buff[250];

  offset++;
  snprintf(buff, sizeof(buff), "s: %f,", get_saturation());
  for (int i = 0; buff[i] && offset < SIZE.width; i++, offset++)
    *surface_at(get_surface(), offset, SIZE.height - 1) = (fragment){.r = 255, .b = 255, .g = 255, .chr = buff[i]};

  offset++;
  snprintf(buff, sizeof(buff), "l: %f,", get_luminance());
  for (int i = 0; buff[i] && offset < SIZE.width; i++, offset++)
    *surface_at(get_surface(), offset, SIZE.height - 1) = (fragment){.r = 255, .b = 255, .g = 255, .chr = buff[i]};

  offset++;
  snprintf(buff, sizeof(buff), "m: %f", get_magnitude());
  for (int i = 0; buff[i] && offset < SIZE.width; i++, offset++)
    *surface_at(get_surface(), offset, SIZE.height - 1) = (fragment){.r = 255, .b = 255, .g = 255, .chr = buff[i]};

  display_context();
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

static void key_presses(float dt)
{
  key_listen();

  float saturation_add  = 0.0f; 
  float luminance_add   = 0.0f;
  float magnitude_add   = 0.0f;

  if (get_key('q').pressed)
    RUN = false;

  if (get_key('o').pressed)
    saturation_add = -dt;
  if (get_key('p').pressed)
    saturation_add = dt;

  if (get_key('k').pressed)
    luminance_add = -dt;
  if (get_key('l').pressed)
    luminance_add = dt;

  if (get_key('n').pressed)
    magnitude_add = -dt;
  if (get_key('m').pressed)
    magnitude_add = dt;

  add_saturation_threshold(saturation_add);
  add_luminance_threshold(luminance_add);
  add_maginitude_threshold(magnitude_add);
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
  timer time = create_timer();

  while (RUN)
  {
    handle_events();
    key_presses(timer_restart(&time));
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
