#ifndef QUERIER_H
#define QUERIER_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include "word.h"
#include "index.h"
#include "counters.h"
#include "hashtable.h"

#define MAX_WORDS 100
#define MAX_QUERIES 100

typedef struct {
    int key;
    int value;
} max_counter_t;

/**
 * Validates the user query by checking for various errors and calls the parseQuery function.
 * 
 * @param pathDirectory The path to the directory containing the indexed files.
 * @param indexFile The path to the index file.
 */
void validateQuery(char* pathDirectory, char* indexFile);

/**
 * Parses the user query, normalizes the words, and calls the traverse function.
 * 
 * @param query The user query to be parsed and processed.
 * @param pathDirectory The path to the directory containing the indexed files.
 * @param indexFile The path to the index file.
 */
void parseQuery(char* query, char* pathDirectory, char* indexFile);

/**
 * Traverses the query entries, performs the search operation, and calls the necessary helper functions.
 * 
 * @param entries The parsed and normalized query entries.
 * @param pathDirectory The path to the directory containing the indexed files.
 * @param indexFile The path to the index file.
 * @param orNumber The number of "or" statements in the query.
 */
void traverse(char*** entries, char* pathDirectory, char* indexFile, int orNumber);

/**
 * Performs the "and" operation on a set of query entries and returns the matching counters.
 * 
 * @param entries The parsed and normalized query entries.
 * @param index The index data structure containing the indexed data.
 * @param i The index of the current set of query entries.
 * @return The counters containing the matching document IDs and their scores.
 */
counters_t* andTraverse(char*** entries, index_t* index, int i);

/**
 * Formats and prints the search results based on the website counters.
 * 
 * @param websiteCounter The counters containing the website IDs and their scores.
 * @param pathDirectory The path to the directory containing the indexed files.
 */
void formatPrint(counters_t* websiteCounter, char* pathDirectory);

/**
 * Helper function to find the maximum counter value and its corresponding key.
 * 
 * @param arg A pointer to the max_counter_t struct to store the maximum counter value and key.
 * @param key The current key being iterated.
 * @param count The current count associated with the key.
 */
void find_max_counter(void *arg, const int key, const int count);

/**
 * Removes a key from the counter set.
 * 
 * @param ctrs The counters data structure from which to remove the key.
 * @param key The key to be removed.
 */
void remove_key(counters_t *ctrs, const int key);

/**
 * Formats and prints the details of a matching document based on its ID and score.
 * 
 * @param pathDirectory The path to the directory containing the indexed files.
 * @param id The ID of the matching document.
 * @param score The score associated with the matching document.
 */
void format(char* pathDirectory, int id, int score);

#endif // QUERIER_H
