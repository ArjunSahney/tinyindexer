#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "counters.h"

typedef struct counter_node {
    int key;
    int count;
    struct counter_node *next;
} counter_node_t;

typedef struct counters {
    struct counter_node *head;
} counters_t;


counter_node_t *counter_new_node(const int key) {
    counter_node_t *node = malloc(sizeof(counter_node_t));
    if (node == NULL) {
        exit(1);
    }
    node->key = key;
    node->count = 1;
    node->next = NULL;
    return node;
}


counters_t *counters_new(void) {
    counters_t *counter = malloc(sizeof(counters_t));

    if (counter == NULL) {
        exit(1);
    }
    counter->head = NULL;
    return counter;
}

int counters_add(counters_t *ctrs, const int key) {
    if (ctrs == NULL || key < 0) {
        return 0;
    }
    counter_node_t *i = ctrs->head;
    while (i != NULL) {
        if (i->key == key) {
            i->count++;
            return i->count;
        }
        i = i->next;
    }
    counter_node_t *new_counter = counter_new_node(key);
    if (new_counter != NULL) {
        new_counter->next = ctrs->head;
        ctrs->head = new_counter;
        return new_counter->count;
    } else {
        return 0;
    }
}

int counters_get(counters_t *ctrs, const int key) {
    if (key < 0 || ctrs == NULL) {
        return 0;
    }
    counter_node_t *i = ctrs->head;
    while (i != NULL) {
        if (i->key == key) {
            return i->count;
        }
        i = i->next;
    }

    return 0;
}

bool counters_set(counters_t *ctrs, const int key, const int count) {
    if (ctrs == NULL || key < 0 || count < 0) {
        return false;
    }
    counter_node_t *i = ctrs->head;
    while (i != NULL) {
        if (i->key == key) {
            i->count = count;
            return true;
        }
        i = i->next;
    }
    counter_node_t *new_counter = counter_new_node(key);
    if (new_counter != NULL) {
        new_counter->count = count;
        new_counter->next = ctrs->head;
        ctrs->head = new_counter;
        return true;
    } else {
        return false;
    }
}

void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, const int count)) {
    if (ctrs == NULL || itemfunc == NULL) {
        return;
    }
    for (counter_node_t *i = ctrs->head; i != NULL; i = i->next) {
        (*itemfunc)(arg, i->key, i->count);
    }
}

void counters_print(counters_t *ctrs, FILE *fp) {
    if (fp == NULL) {
        return;
    }
    if (ctrs == NULL) {
        fputs("(null)", fp);
    } else {
        fputc('{', fp);
            for (counter_node_t *i = ctrs->head; i != NULL; i = i->next) {
            fprintf(fp, "%d=%d", i->key, i->count);
            if (i->next != NULL) {
                fputs(", ", fp);
            }
        }
        fputc('}', fp);
    }
}

void counters_delete(counters_t *ctrs) {
    if (ctrs == NULL) {
        return;
    }
    counter_node_t *i = ctrs->head;
    while (i != NULL) {
        counter_node_t *next = i->next;
        free(i);
        i = next;
    }
    free(ctrs);
}
