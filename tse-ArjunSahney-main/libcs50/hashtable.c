#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "hash.h"
#include "set.h"

typedef struct delete_arg {
    void (*itemdelete)(void *item);
} delete_arg_t;

static void delete_helper(void *arg, const char *key, void *item);

// Defining structure of hashtable
typedef struct hashtable {
  int counter;
  set_t **hashnodes; // array of pointers to sets
} hashtable_t;

// Creating a new hashtable
hashtable_t *hashtable_new(const int num_slots) {
  if (num_slots <= 0) {
    return NULL;
  }
  // allocate memory for hashnodes
  hashtable_t *table = malloc(sizeof(hashtable_t));
  if (table == NULL) {
    return NULL;
  }
  table->hashnodes = calloc(num_slots, sizeof(set_t *));
  if (table->hashnodes == NULL) {
    free(table);
    return NULL;
  }
  table->counter = num_slots;
  return table;
}

// insert operation
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
  // if params not null
  if (ht == NULL || key == NULL || item == NULL) {
    return false;
  }
  int hashkey = hash_jenkins(key, ht->counter);

  // checking if other hashkey and if not then inputting
  if (ht->hashnodes[hashkey] == NULL) {
    ht->hashnodes[hashkey] = set_new();
  }

  return set_insert(ht->hashnodes[hashkey], key, item);
}

// finding something in hashtable
void *hashtable_find(hashtable_t *ht, const char *key) {
  if (ht == NULL || key == NULL) {
    return NULL;
  }
  int hashkey = hash_jenkins(key, ht->counter);
  // if not found return null
  if (ht->hashnodes[hashkey] == NULL) {
    return NULL;
  }
  // returning hashkey
  return set_find(ht->hashnodes[hashkey], key);
}

// iterating through hashtable
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
  // if params null return null
  if (ht == NULL || itemfunc == NULL) {
    return;
  }
  for (int i = 0; i < ht->counter; i++) {
    if (ht->hashnodes[i] != NULL) {
      set_iterate(ht->hashnodes[i], arg, itemfunc);
    }
  }
}

// deleting everything in hashtable
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
  if (ht == NULL) {
    return;
  }
  // using hashtable iterate and deleting each thing
  delete_arg_t delete_arg = { itemdelete };
  hashtable_iterate(ht, &delete_arg, delete_helper);

  // iterating through the slot
  for (int i = 0; i < ht->counter; i++) {
    if (ht->hashnodes[i] != NULL) {
      set_delete(ht->hashnodes[i], itemdelete);
    }
  }

  // freeing memory
  free(ht->hashnodes);
  free(ht);
}
// delete helper function
static void delete_helper(void *arg, const char *key, void *item) {
    (void)key;
    delete_arg_t *delete_arg = arg;
    if (delete_arg->itemdelete != NULL) {
        delete_arg->itemdelete(item);
    }
}


void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
  if (fp == NULL) {
    return;
  }

  if (ht == NULL) {
    fputs("(null)", fp);
    return;
  }

  for (int i = 0; i < ht->counter; i++) {
    fprintf(fp, "hashslot %d: ", i);
    if (ht->hashnodes[i] == NULL) {
      fputs("(empty)", fp);
    } else {
      set_print(ht->hashnodes[i], fp, itemprint);
    }
    fputc('\n', fp);
  }
}
