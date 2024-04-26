#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_BUCKETS 8
#define MAX_KEY_SIZE 8

uint32_t djb2(const char *s) {
  int32_t h = 5381;
  char ch;

  while ((ch = *s++)) {
    h = (h << 5) + h + ch;
  }

  return h;
}

typedef struct Node {
  char *key;
  void *val;
  uint32_t hash;
  struct Node *next;
  struct Node *prev;
} Node;

typedef struct Hashmap {
  Node **buckets;
  int num_buckets;
} Hashmap;

Hashmap *Hashmap_new(void) {
  Hashmap *hm = malloc(sizeof(Hashmap));

  hm->buckets = calloc(STARTING_BUCKETS, sizeof(Node));
  hm->num_buckets = STARTING_BUCKETS;

  return hm;
}

void Hashmap_free(Hashmap *hm) {
  for (Node **buck=hm->buckets; buck<hm->buckets+STARTING_BUCKETS; buck++) {
    // printf("%p\n", buck);
    for (Node *node=*buck; node; node=node->next) {
      // printf("\t%s\n", node->key);
      free(node->key);
      free(node);
    }
  }

  free(hm->buckets);
  free(hm);
}

Node *Hashmap_entry(char *key, void *val) {
  // create/populate node
  Node *hm_entry = malloc(sizeof(Node));
  // hash key
  hm_entry->hash = djb2(key);
  hm_entry->key  = strdup(key);
  hm_entry->val  = val;
  hm_entry->next = NULL;
  hm_entry->prev = NULL;

  return hm_entry;
}

Node *Hashmap_traverse(Node *bucket, char *key) {
  for (Node *curr=bucket; curr; curr=curr->next) {
    if (strcmp(curr->key, key) == 0) return curr;
    if (curr->next == NULL) return curr;
  }
}

Node **Bucket(Hashmap *hm, uint32_t hash) {
  int i = hash % hm->num_buckets;
  Node **bucket = hm->buckets + i;
  return bucket;
}

void *Hashmap_set(Hashmap *hm, char *key, void *val) {
  Node *hm_entry = Hashmap_entry(key, val);
  Node **bucket = Bucket(hm, hm_entry->hash);
  Node *node = Hashmap_traverse(*bucket, key);

  if (*bucket == NULL) {
    *bucket = hm_entry;
  } else if (strcmp(node->key, key) == 0) {
    if (node->prev == NULL) *bucket = hm_entry;
    hm_entry->prev = node->prev;
    hm_entry->next = node->next;
    free(node->key);
    free(node);
    *node = *hm_entry;
  } else {
    hm_entry->prev = node;
    node->next = hm_entry;
  }
}
 
void *Hashmap_get(Hashmap *hm, char *key) {
  uint32_t hash = djb2(key);

  Node **bucket = Bucket(hm, hash);

  if (*bucket == NULL)
     return *bucket;
  else {
    Node *node = Hashmap_traverse(*bucket, key);
    if (node && strcmp(node->key, key) == 0) return node->val;
    else return NULL; 
  }
}

void Hashmap_delete(Hashmap *hm, char *key) {
  uint32_t hash = djb2(key);

  Node **bucket = Bucket(hm, hash);
  Node *node = Hashmap_traverse(*bucket, key);

  if (node->prev && node->next) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
  }

  if (node->prev == NULL) {
    free(node);
    *bucket = NULL;
  }
}

int main() {
  Hashmap *h = Hashmap_new();
 
  // basic get/set functionality
  int a = 5;
  float b = 7.2;
  Hashmap_set(h, "item a", &a);
  Hashmap_set(h, "item b", &b);
  assert(Hashmap_get(h, "item a") == &a);
  assert(Hashmap_get(h, "item b") == &b);
 
  // using the same key should override the previous value
  int c = 20;
  Hashmap_set(h, "item a", &c);
  assert(Hashmap_get(h, "item a") == &c);
 
  // basic delete functionality
  // Hashmap_delete(h, "item a");
  // assert(Hashmap_get(h, "item a") == NULL);
 
  // handle collisions correctly
  // note: this doesn't necessarily test expansion
  int i, n = STARTING_BUCKETS * 2, ns[n];
  char key[MAX_KEY_SIZE];
  for (i = 0; i < n; i++) {
    ns[i] = i;
    sprintf(key, "item %d", i);
    Hashmap_set(h, key, &ns[i]);
  }

  for (i = 0; i < n; i++) {
    sprintf(key, "item %d", i);
    printf("%s %d\n", key, i);
    assert(Hashmap_get(h, key) == &ns[i]);
  }

  Hashmap_free(h);
//   /*
//      stretch goals:
//      - expand the underlying array if we start to get a lot of collisions
//      - support non-string keys
//      - try different hash functions
//      - switch from chaining to open addressing
//      - use a sophisticated rehashing scheme to avoid clustered collisions
//      - implement some features from Python dicts, such as reducing space use,
//      maintaing key ordering etc. see https://www.youtube.com/watch?v=npw4s1QTmPg
//      for ideas
//      */
//   printf("ok\n");
}
