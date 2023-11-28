#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
  int a;
  int b;
} myarg_t;

typedef struct {
  int x;
  int y;
} myret_t;

void *mythread(void *arg) {
  myarg_t *args = (myarg_t *) arg;
  printf("%d %d\n", args->a, args->b);

  myret_t *ret = malloc(sizeof(myret_t));
  ret->x = args->a*2;
  ret->y = args->b*2;

  return (void *) ret;
}

int main(int argc, char *argv[]) {
  pthread_t p;

  myarg_t args = { 10, 20 };
  int rc = pthread_create(&p, NULL, mythread, &args);

  myret_t *rvals;
  pthread_join(p, (void **) &rvals);

  printf("%d %d\n", rvals->x, rvals->y);

  free(rvals);
  return 0;
}
