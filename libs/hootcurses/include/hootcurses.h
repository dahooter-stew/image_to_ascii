#pragma once
#ifndef HOOTCURSES
#define HOOTCURSES

#include "screen_surface.h"
#include "resizable_array.h"
#include "types.h"

#include <stdbool.h>

typedef struct 
{
  screen_surface* surface;
  screen_surface* previous_frame;
  resizable_array_char* ansi_string;
  char* title;
} render_context;

screen_surface*   create_surface(surface_size size);
render_context*   create_context(screen_surface* surface, char* title);
void              set_current_context(render_context* context);

screen_surface*   get_surface(void);
screen_surface*   get_previous_surface(void);
render_context*   get_context(void);

void              init_hootcurses(char* title);

void              blit_surface(void);
void              display_context(void);
void              diff_surface(void);
void              redraw(void);

bool              poll_events(event* e);

#endif
