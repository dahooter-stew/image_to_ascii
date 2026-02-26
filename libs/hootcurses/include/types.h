#pragma once
#ifndef TYPES
#define TYPES

typedef enum
{
  INIT = 0,
  WINDOW_RESIZE
} EVENT_TYPE;

typedef struct
{
  int type;
} event;

#endif
