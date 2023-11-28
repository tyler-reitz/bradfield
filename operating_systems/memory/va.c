#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("location of main: %p\n", main);
  printf("location of heap: %p\n", malloc(100e6));
  int x;
  printf("location of stack: %p\n", &x);
  while(1);
  return x;
}
