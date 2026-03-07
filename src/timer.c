#include "timer.h"

#include <time.h>
#include <stdio.h>

long long get_current_milliseconds(void)
{
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC, &ts);

  return (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000);
}

timer create_timer(void)
{
  float time = (float)get_current_milliseconds() / 1000.0f;
  return (timer){
    time, time 
  };
}

float timer_restart(timer* time)
{
  time->current_time = (float)get_current_milliseconds() / 1000.0f;
  float difference = time->current_time - time->previous_time;
  time->previous_time = time->current_time;

  return difference;
}

float timer_elapsed(timer* time)
{
  time->current_time = (float)get_current_milliseconds() / 1000.0f;
  float difference = time->current_time - time->previous_time;

  return difference;
}

void delay(timer* time, float seconds)
{
  struct timespec req = {0, 1000000};
  timer_restart(time);

  while (timer_elapsed(time) < seconds)
  {
    nanosleep(&req, NULL);
  }
}
