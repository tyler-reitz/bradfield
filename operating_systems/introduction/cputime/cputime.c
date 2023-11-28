#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define SLEEP_SEC 3
#define NUM_MULS 100000000
#define NUM_MALLOCS 100000
#define MALLOC_SIZE 1000

// TODO define this struct
struct profile_times {
  struct timespec start;
};

// TODO populate the given struct with starting information
void profile_start(struct profile_times *t) {
  clock_gettime(CLOCK_REALTIME, &t->start);
}

// TODO given starting information, compute and log differences to now
void profile_log(struct profile_times *t) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);

  printf("[%d]: real %ld.%ld %ld.%ld %ld\n", getpid(), now.tv_sec, now.tv_nsec, t->start.tv_sec, t->start.tv_nsec, 1000000000 - t->start.tv_nsec);
}

int main(int argc, char *argv[]) {
  struct profile_times t;

  // TODO profile doing a bunch of floating point muls
  float x = 1.0;
  profile_start(&t);
  for (int i = 0; i < NUM_MULS; i++)
    x *= 1.1;
  profile_log(&t);

  // TODO profile doing a bunch of mallocs
  profile_start(&t);
  void *p;
  for (int i = 0; i < NUM_MALLOCS; i++)
    p = malloc(MALLOC_SIZE);
  profile_log(&t);

  // TODO profile sleeping
  profile_start(&t);
  sleep(SLEEP_SEC);
  profile_log(&t);
}