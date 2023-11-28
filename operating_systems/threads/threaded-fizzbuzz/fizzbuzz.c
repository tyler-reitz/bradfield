#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

volatile int n = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *fizz(void *arg) {
  while (1) {
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&cond, &lock);    
    if (n % 3 == 0) {
      printf("\tfizz");
    }
    pthread_mutex_unlock(&lock);
  }
}

void *buzz(void *arg) {
  while (1) {
    pthread_mutex_lock(&lock);
    pthread_cond_wait(&cond, &lock);    
    if (n % 5 == 0) {
      printf("\tbuzz");
    }
    pthread_mutex_unlock(&lock);
  }
}

int main () {
  // start two threads, running fizz and buzz respectively
  pthread_t t1, t2;
  pthread_create(&t1, NULL, fizz, NULL);
  pthread_create(&t2, NULL, buzz, NULL);
  // every 100ms, update n randomly from the range [0, 16), indefinitely
  while (1) {
    pthread_mutex_lock(&lock);
    n = rand() & 0xf;
    printf("\n%d:", n);
    pthread_mutex_unlock(&lock);
    pthread_cond_broadcast(&cond);
    usleep(100000);
  }
}
