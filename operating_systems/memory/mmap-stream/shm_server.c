#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE (1 << 20) // Test with this many bytes of data

int main () {
  int checksum, max, n;

  srand(0x1235);
  max = SIZE / sizeof(int);

  // Construct anonymous shared memory object
  int fd = shm_open("/my-stream", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  // Map anonymous shared memory object into process' address space
  int *p = mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);

  checksum = 0;
  for (int i = 0; i < max; i++) {
    n = rand();
    checksum ^= n;
    p[i] = n;
  }

  printf("Sent %d random ints to client, checksum %d\n", max, checksum);
}
