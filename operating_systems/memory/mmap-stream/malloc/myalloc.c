#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

struct block {
  struct block *next;
  size_t size;
  int free;
};

#define BLOCK_SIZE sizeof(struct block)

void *myalloc(size_t size) {
  static struct block *BSS = NULL;

  if (BSS == NULL) {
    BSS = sbrk(BLOCK_SIZE);

    BSS->size = size;
    BSS->free = 1;
    BSS->next = NULL;
  }

  struct block *heap = BSS;
  do {
    if (size <= heap->size && heap->free) {
      heap->free = 0;

      return heap+1;
    }

    if (!heap->next) {
      heap->next = sbrk(BLOCK_SIZE + size) + BLOCK_SIZE + size;

      heap->next->size = size;
      heap->next->free = 0;
      heap->next->next = NULL;

      return heap->next+1;
    } else {
      heap = heap->next;
    }
  } while(heap);
}

void myfree(void *ptr) {
  struct block *meta = ptr-BLOCK_SIZE;  

  meta->free = 1;
}

int main() {
  int *foo = myalloc(sizeof(int));
  *foo = 42;
  assert(*foo==42);

  char *bar = myalloc(11);
  memcpy(bar, "hello world", 11);

  myfree(foo);

  int *oof = myalloc(sizeof(int));
  *oof = 24;
  assert(*oof==24);

  char *str = myalloc(1000);
  *str = 'A';

  long *ofo = myalloc(sizeof(long));
  *ofo = 15;
  assert(*ofo==15);

  myfree(ofo);

  long *fof = myalloc(sizeof(long));
  *fof = 51;
  assert(*fof==51);

  assert(foo==oof);
  assert(ofo==ofo);
  printf("foo = %p, oof = %p, str = %s\n", foo, oof, str);
}

