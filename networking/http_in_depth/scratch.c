#include <assert.h>
#include <stdio.h>
#include <zlib.h>

int main(int argc, char argv[]) {
  int num = 4242;
  char fmt[] = "Content-length: %d\r\n";
  char str[30]; 
  int size = snprintf(str, 30, fmt, num);
  printf("%s", str);
}
