#include "key_handler.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

static KEY_STATUS KEY_STATUSES[256];
static char read_buffer[256];

static int KEYBOARD_PRESS(void)
{
  struct timeval tv = { 0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);

  return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

void init_key_handler(void)
{
  for (int i = 0; i < 256; i++)
    KEY_STATUSES[i] = (KEY_STATUS){false, false, false};
}

void key_listen(void)
{
  char chr = -1;

  for (int i = 0; i < 256; i++)
  {
    KEY_STATUSES[i] = (KEY_STATUS){false, false, false};
  }

  if (KEYBOARD_PRESS())
  {
    int count = read(STDIN_FILENO, read_buffer, 255);
    for (int i = 0; i < count && count > 0; i++)
    {
      KEY_STATUSES[read_buffer[i]] = (KEY_STATUS){.pressed = true};
    }
  }
}

KEY_STATUS get_key(char chr)
{
  return KEY_STATUSES[chr];
}
