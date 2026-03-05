#pragma once
#ifndef KEY_HANDLER
#define KEY_HANDLER

#include <stdbool.h>

enum KEYS
{
  KEYS_START = 0,

  KEY_a = 'a',

  KEYS_END
};

typedef struct
{
  bool pressed;
  bool released;
  bool held;
} KEY_STATUS;

void init_key_handler(void);

void key_listen(void);

KEY_STATUS get_key(char chr);

#endif
