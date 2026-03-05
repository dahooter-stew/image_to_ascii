#include "hootcurses.h"
#include "screen_surface.h"
#include "queue.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static render_context* CURRENT_CONTEXT = NULL;
static event_queue_node* EVENT_QUEUE = NULL;

static struct termios original_termios;

static void disable_raw_mode()
{
  tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

static void enable_raw_mode()
{
  tcgetattr(STDIN_FILENO, &original_termios);
  struct termios new_termios = original_termios;

  atexit(disable_raw_mode);

  new_termios.c_lflag &= ~(ICANON | ECHO);

  new_termios.c_cc[VMIN] = 1;
  new_termios.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

static surface_size get_terminal_size(void)
{
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  return (surface_size) {
    .width = w.ws_col,
    .height = w.ws_row
  };
}

static void push_string_resizable_char(resizable_array_char* ansi_string, char* str)
{
  int i = 0;
  while (str[i])
  {
    push_char(ansi_string, str[i]);
    i++;
  }
}

static void move_cursor(int x, int y)
{
  printf("\033[%d;%dH", y, x);
}

static int frame_difference(void)
{
  surface_size size = get_surface_size(get_surface());

  int count = 0;
  for (int i = 0; i < size.width * size.height; i++)
  {
    fragment c = surface_index(get_surface())[i];
    fragment p = surface_index(get_previous_surface())[i];

    if (!fragment_equal(c, p))
      count++;
  }
  return count;
}

static void check_resize(void)
{
  surface_size curr = get_terminal_size();
  surface_size prev = get_surface_size(get_surface());

  if (curr.width != prev.width || curr.height != prev.height)
  {
    EVENT_QUEUE = event_enqueu(EVENT_QUEUE, (event){.type = WINDOW_RESIZE});

    get_surface()->data = realloc(get_surface()->data, curr.width * curr.height * sizeof(fragment));
    get_surface()->width = curr.width;
    get_surface()->height = curr.height;
    get_previous_surface()->data = realloc(get_previous_surface()->data, curr.width * curr.height * sizeof(fragment));
    get_previous_surface()->width = curr.width;
    get_previous_surface()->height = curr.height;

    reserve_char(CURRENT_CONTEXT->ansi_string, curr.width * curr.height * 30);

    redraw();
  }
}

static void ansi_reset(void)
{
  CURRENT_CONTEXT->ansi_string->length = 0;
}

render_context* create_context(screen_surface* surface, char* title)
{
  resizable_array_char* buffer = allocate_array_char(surface->width * surface->height * 30);

  screen_surface* prev = create_surface((surface_size){ .width = surface->width, .height = surface->height });

  clear_surface(surface);
  clear_surface(prev);

  render_context* context = malloc(sizeof(render_context));
  context->surface = surface;
  context->previous_frame = prev;
  context->ansi_string = buffer;
  context->title = title;

  return context;
}

void set_current_context(render_context* context)
{
  if (!context)
  {
    CURRENT_CONTEXT = NULL;
    return;
  }

  CURRENT_CONTEXT = context;
}

screen_surface* get_surface(void)
{
  return CURRENT_CONTEXT->surface;
}

screen_surface* get_previous_surface(void)
{
  return CURRENT_CONTEXT->previous_frame;
}

render_context* get_context(void)
{
  return CURRENT_CONTEXT;
}

void init_hootcurses(char* title)
{
  enable_raw_mode();
  printf("\e[?25l");
  fflush(stdout);

  screen_surface* surface = create_surface(get_terminal_size());
  render_context* context = create_context(surface, title);
  set_current_context(context);

  EVENT_QUEUE = create_event_queue_node((event){.type = INIT});
}

void exit_hootcurses(void)
{
  delete_event_queue(EVENT_QUEUE);
  free_array_char(CURRENT_CONTEXT->ansi_string);
  delete_surface(CURRENT_CONTEXT->surface);
  delete_surface(CURRENT_CONTEXT->previous_frame);
}

void blit_surface(void)
{
  ansi_reset();
  push_string_resizable_char(CURRENT_CONTEXT->ansi_string, "\033[H");

  screen_surface* s = get_surface();
  surface_size size = get_surface_size(s);
  fragment* data = surface_index(s);

  char temp[32];

  for (int y = 0; y < size.height; y++)
  {
    for (int x = 0; x < size.width; x++)
    {
      int i = y * size.width + x;

      snprintf(temp, sizeof(temp),
         "\x1b[38;2;%d;%d;%dm",
         data[i].r,
         data[i].g,
         data[i].b);

      push_string_resizable_char(CURRENT_CONTEXT->ansi_string, temp);
      push_char(CURRENT_CONTEXT->ansi_string, data[i].chr);
    }
  }

  push_string_resizable_char(CURRENT_CONTEXT->ansi_string, "\x1b[0m");
}

void diff_surface(void)
{
  // static char TEMP_BUFFER[30];
  surface_size size = get_surface_size(get_surface());
  // bool previous_diff = false;
  // fragment prev_color = (fragment){.r = 0, .g = 0, .b = 0, .chr = 0};
  ansi_reset();

  for (int x = 0; x < size.width; x++)  
  {
    for (int y = 0; y < size.height; y++)
    {
      fragment* c = surface_at(get_surface(), x, y);
      fragment* p = surface_at(get_previous_surface(), x, y);

      bool is_different = !fragment_equal(*c, *p);
      // bool new_color = color_equal(prev_color, *c);

      if (is_different)
      {
        // if (!previous_diff)
        move_cursor(x + 1, y + 1);
        printf("\x1b[38;2;%d;%d;%dm%c\x1b[0m", c->r, c->g, c->b, c->chr);
      }

      // prev_color = *c;
      // previous_diff = is_different;
    }
  }
}

void redraw(void)
{
  clear_surface(get_surface());
  display_context(); 
}

void display_context(void)
{
  int difference = frame_difference();
  surface_size curr_size = get_surface_size(get_surface());
  int surface_area_half = curr_size.width * curr_size.height / 2;

  if (difference > surface_area_half)
  {
    blit_surface();
    write(1, CURRENT_CONTEXT->ansi_string->data, CURRENT_CONTEXT->ansi_string->length);
  }
  else
    diff_surface();

  for (int i = 0; i < curr_size.width * curr_size.height; i++)
    surface_index(get_previous_surface())[i] = surface_index(get_surface())[i];
}

bool poll_events(event* e)
{
  check_resize();

  if (!EVENT_QUEUE)
      return false;

  event event_node = event_dequeue(&EVENT_QUEUE);
  *e = event_node;

  return event_node.type != INIT;
}
