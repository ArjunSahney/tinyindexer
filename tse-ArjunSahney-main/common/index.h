#ifndef INDEX_H
#define INDEX_H

#include <stdbool.h>
#include "hashtable.h"
#include "counters.h"
#include "set.h"

// defining the structure of index
typedef hashtable_t index_t;

index_t *index_new(const int num_slots);

bool index_insert(index_t *idx, const char *hashtable_key, const int counter_key);

bool index_increment(index_t *idx, const char *hashtable_key, const int counter_key);

bool index_find(index_t *idx, const char *hashtable_key, const int counter_key);

int index_get(index_t *idx, const char *hashtable_key, const int counter_key);

void index_delete(index_t *idx);

void index_print(hashtable_t *index, FILE *fp);
void index_print_helper(void *arg, const char *key, void *item);
void counters_print_helper(void *arg, const int key, const int count);

// Update the declaration of index_loader to return void * instead of void
void* index_loader(const char *source, const char *destination);

counters_t *index_get_counter(index_t *idx, const char *hashtable_key);

#endif // INDEX_H
