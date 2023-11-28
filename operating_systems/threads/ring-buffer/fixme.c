#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef int job;

pthread_mutex_t lock;
pthread_cond_t cond;

struct JobQueue {
    job *buffer;
    int capacity;
    int size;  // number of current items (<= capacity)
    int wp;  // locations at which to write
    int rp;  // ... and read
    pthread_mutex_t lock;
    pthread_cond_t cond_w;
    pthread_cond_t cond_r;
};

struct JobQueue* JQ_init(int capacity) {
    struct JobQueue *jq = malloc(sizeof(struct JobQueue));
    jq->buffer = malloc(capacity * sizeof(job));
    jq->capacity = capacity;
    jq->size = 0;
    jq->wp = 0;
    jq->rp = 0;
    jq->lock = pthread_mutex_init(&lock, NULL);
    jq->cond_w = pthread_cond_init(&cond_w, &
    return jq;
}

void JQ_free(struct JobQueue *jq) {
    free(jq->buffer);
    free(jq);
}

void JQ_add(struct JobQueue*jq, job item) {
    // TODO block if size == capacity
    pthread_mutex_lock(&lock);
    while (jq->size >= jq->capacity) pthread_cond_wait(&cond, &lock);
    jq->buffer[jq->wp] = item;
    printf("(%d) %d -> [%d]\n", gettid(), item, jq->wp);
    jq->wp = (jq->wp + 1) % jq->capacity;
    jq->size++;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
}

job JQ_get(struct JobQueue*jq) {
    // TODO block if size == 0
    pthread_mutex_lock(&lock);
    while (jq->size <= 0) pthread_cond_wait(&cond, &lock);
    job item = jq->buffer[jq->rp];
    printf("(%d)        [%d] -> %d\n", gettid(), jq->rp, item);
    jq->rp = (jq->rp + 1) % jq->capacity;
    jq->size--;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
    return item;
}

void *producer(void *arg) {
  struct JobQueue *jq = (struct JobQueue *)arg;
  int i = 0;
  while (1)
    JQ_add(jq, i++ % 100);
}

void *consumer(void *arg) {
  struct JobQueue *jq = (struct JobQueue *)arg;
  while (1)
    JQ_get(jq);
}

int main () {
  // printf("Starting basic test\n");
  struct JobQueue *jq = JQ_init(8);
  int i;
  for (i = 0; i < 5; i++)
    JQ_add(jq, i);
  for (i = 0; i < 5; i++)
    JQ_get(jq);
  for (i = 0; i < 5; i++)
    JQ_add(jq, i);
  for (i = 0; i < 5; i++)
    JQ_get(jq);
  JQ_free(jq);
  printf("Starting concurrent test\n");
  // start n producers, m consumers in threads
  // producer just write incrementing integers to jq indefinitely
  // consumers just read/print them
  int n = 2;
  int m = 2;
  pthread_t prod[n], cons[m];
  jq = JQ_init(4);
  for (i = 0; i < n; i++)
    pthread_create(&prod[i], NULL, producer, jq);
  for (i = 0; i < m; i++)
    pthread_create(&cons[i], NULL, consumer, jq);
  for (i = 0; i < n; i++)
    pthread_join(prod[i], NULL);
  for (i = 0; i < m; i++)
    pthread_join(cons[i], NULL);
  JQ_free(jq);
  printf("OK\n");
}
