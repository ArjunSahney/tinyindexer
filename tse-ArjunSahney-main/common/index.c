#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // Include stdint.h
#include "index.h"
#include "hashtable.h"
#include "counters.h"
#include "file.h"
#include "set.h"

// Helper function to delete counters
static void counters_delete_helper(void *item) {
    if (item != NULL) {
        counters_delete((counters_t *)item);
    }
}


// creating new index
index_t *index_new(const int num_slots) {
    return hashtable_new(num_slots);
}

bool index_insert(index_t *idx, const char *hashtable_key, const int counter_key) {
    if (idx == NULL || hashtable_key == NULL || counter_key < 0) {
        return false;
    }

    counters_t *counter = hashtable_find(idx, hashtable_key);
    if (counter == NULL) {
        counter = counters_new();
        if (counter == NULL) {
            return false;
        }
        hashtable_insert(idx, hashtable_key, counter);
    }

    return counters_set(counter, counter_key, 1);
}

bool index_increment(index_t *idx, const char *hashtable_key, const int counter_key) {
    if (idx == NULL || hashtable_key == NULL || counter_key < 0) {
        return false;
    }

    counters_t *counter = hashtable_find(idx, hashtable_key);
    if (counter == NULL) {
        counter = counters_new();
        if (counter == NULL) {
            return false;
        }
        hashtable_insert(idx, hashtable_key, counter);
    }

    counters_add(counter, counter_key);
    return true;
}

bool index_find(index_t *idx, const char *hashtable_key, const int counter_key) {
    if (idx == NULL || hashtable_key == NULL || counter_key < 0) {
        return false;
    }

    counters_t *counter = hashtable_find(idx, hashtable_key);
    if (counter == NULL) {
        return false;
    }

    int count = counters_get(counter, counter_key);
    return count > 0;
}

void index_delete(index_t *idx) {
    hashtable_delete(idx, counters_delete_helper);
}

int index_get(index_t *idx, const char *hashtable_key, const int counter_key) {
    if (idx == NULL || hashtable_key == NULL || counter_key < 0) {
        return 0;
    }

    counters_t *counter = hashtable_find(idx, hashtable_key);
    if (counter == NULL) {
        return 0;
    }

    return counters_get(counter, counter_key);
}

void index_print(hashtable_t *index, FILE *fp) {
    if (index != NULL && fp != NULL) {
        hashtable_iterate(index, fp, index_print_helper);
    }
}

void index_print_helper(void *arg, const char *key, void *item) {
    FILE *fp = (FILE *)arg;
    counters_t *counters = item;
    fprintf(fp, "%s", key);
    counters_iterate(counters, fp, counters_print_helper);
    fprintf(fp, "\n");
}

void counters_print_helper(void *arg, const int key, const int count) {
    FILE *fp = arg;
    fprintf(fp, " %d %d", key, count);
}

void* index_loader(const char *source, const char *destination) {
    FILE *src = fopen(source, "r");
    if (src == NULL) {
        fprintf(stderr, "Source file can't be opened");
        return NULL;
    }

    index_t *index = index_new(500);
    if (index == NULL) {
        fclose(src);
        return NULL;
    }

    char line[1000];
    while (fgets(line, sizeof(line), src)) {
        char hashtable_key[200];
        int key, value;

        sscanf(line, "%s", hashtable_key);
        char *token = strtok(line, " ");

        while (token != NULL) {
            token = strtok(NULL, " ");
            if (token == NULL) break;
            sscanf(token, "%d", &key);

            token = strtok(NULL, " ");
            if (token == NULL) break;
            sscanf(token, "%d", &value);

            for (int i = 0; i < value; i++) {
                index_increment(index, hashtable_key, key);
            }
        }
    }
    fclose(src);

    FILE *dest = fopen(destination, "w");
    if (dest == NULL) {
        fprintf(stderr, "Destination file can't be opened");
        if (index != NULL) {
            index_delete(index);
        }
        return NULL;
    }

    index_print(index, dest);
    fclose(dest);

    return index;
}


counters_t *index_get_counter(index_t *idx, const char *hashtable_key) {
    if (idx == NULL || hashtable_key == NULL) {
        return NULL;
    }

    counters_t *counter = hashtable_find(idx, hashtable_key);
    return counter;
}
