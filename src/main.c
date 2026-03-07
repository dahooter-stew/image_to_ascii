#include "img_ascii.h"

#include <stdio.h>

#include <unistd.h>

int main(int argc, char** argv)
{
  if (argc < 1)
  {
    printf("No file given\n");
    return -1;
  }
  
  init(argv[1]);
  run();
  on_exit();
}
