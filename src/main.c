#include "img_ascii.h"
#include "timer.h"

#include <stdio.h>

#include <unistd.h>

int main(int argc, char** argv)
{
  timer time = create_timer();
  delay(&time, 5.0f);

  if (argc < 1)
  {
    printf("No file given\n");
    return -1;
  }
  
  init(argv[1]);
  run();
  on_exit();
}
