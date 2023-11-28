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

void *Hashmap_set(Hashmap *hm, char *key, void *val) {
  // create/populate node
  Node *hm_entry = malloc(sizeof(Node));
  // hash key
  hm_entry->hash = djb2(key);
  hm_entry->key  = strdup(key); // TODO: free duped string
  hm_entry->val  = val;
  hm_entry->next = NULL;

  int i = hm_entry->hash % hm->num_buckets;
  Node **bucket = hm->buckets + i;
  // check if bucket is empty or not
  if (*bucket == NULL) {
    // if empty, set bucket = &Nod
    *bucket = hm_entry;
  } else {
    // if not empty, add to end of bucket list
    Node *curr = *bucket;
    do {
      if (strcmp(curr->key, key) == 0) {
        hm_entry->next = curr->next;
        free(curr);
        *curr = *hm_entry;  
        break;
      }
      if (curr->next == NULL) {
        return curr->next = hm_entry;
      }

      curr = curr->next;
    } while (curr);
  }
}

Node *Hashmap_WalkBucket(Node * bucket, char *key) {
  if (bucket == NULL) {
    // if empty, set bucket = &Nod
    return bucket;
  } else {
    // if not empty, add to end of bucket list
    Node *curr = bucket;
    do {
      if (strcmp(curr->key, key) == 0) return curr;
      if (curr->next == NULL) return curr;

      curr = curr->next;
    } while (curr);
  }
}

void *Hashmap_get(Hashmap *hm, char *key) {
  uint32_t hash = djb2(key);
  int i = hash%hm->num_buckets;

  Node *bucket = *(hm->buckets+i);
  Node *node = Hashmap_WalkBucket(bucket, key);

  return node->val;
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
 
//   // basic delete functionality
//   Hashmap_delete(h, "item a");
//   assert(Hashmap_get(h, "item a") == NULL);
// 
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

//   Hashmap_free(h);
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
