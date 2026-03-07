#pragma once
#ifndef TIMER
#define TIMER

typedef struct
{
  float current_time;
  float previous_time;
} timer;

long long   get_current_milliseconds(void);
timer       create_timer(void);
float       timer_restart(timer* time);
float       timer_elapsed(timer* time);
void        delay(timer* time, float seconds);

#endif
