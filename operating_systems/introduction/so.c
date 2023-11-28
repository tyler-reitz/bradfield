#include <stdio.h>

void so(void *arg) {
  printf("%p\n", &arg); 
  so(NULL);
}

int main(int argc, char *argv[]) {
  so(NULL);
  return 0;
}
