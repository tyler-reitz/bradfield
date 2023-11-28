#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE (1 << 20) // Test with this many bytes of data

int main () {
  int checksum, max, n;
  struct timespec start, end;
  max = SIZE / sizeof(int);

  // construct the client socket, and connect
  int fd = shm_open("/my-stream", O_RDONLY, S_IRUSR);
  int *p = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd, 0);

  // receive a bunch of data
  clock_gettime(CLOCK_MONOTONIC, &start);
  checksum = 0;
  for (int i = 0; i < max; i++) {
    checksum ^= p[i];
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  float secs =
      (float)(end.tv_nsec - start.tv_nsec) / 1e9 + (end.tv_sec - start.tv_sec);
  float mibs = (float)SIZE / secs / (1 << 20);

  printf("Received at %.3f MiB/s. Checksum: %d\n", mibs, checksum);
}
