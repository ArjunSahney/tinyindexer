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

// Definitions of max words and queries assumptions
#define MAX_WORDS 100
#define MAX_QUERIES 100

// Creating a specific structure for the max counter
typedef struct {
    int key;
    int value;
} max_counter_t;

// Initializing functions
void validateQuery(char* pathDirectory, char* indexFile);
void parseQuery(char* query, char* pathDirectory, char* indexFile);
void traverse(char*** entries, char* pathDirectory, char* indexFile, int orNumber);
counters_t* andTraverse(char*** entries, index_t* index, int i);
void formatPrint(counters_t* websiteCounter, char* pathDirectory);
void find_max_counter(void *arg, const int key, const int count);
void remove_key(counters_t *ctrs, const int key);
void format(char* pathDirectory, int id, int score);


int main(int argc, char* argv[]){
    // Checking number of arguments validity
    if (argc != 3){
        fprintf(stderr, "Please provide pathDirectory and indexFile (2 args).");
        return 1;
    }
    // Initializing path directory
    char *pathDirectory = argv[1];

    // Checking if directory exists
    struct stat directory;
    if (stat(pathDirectory, &directory) == 0){
        if (!S_ISDIR(directory.st_mode)){
        fprintf(stderr, "Directory does not exist\n");
        return 1;
        }
    } 
    else {
        fprintf(stderr, "Error or directory does not exist: %s\n", pathDirectory);
        return 1;
    }

    // Check if .crawler file exists in the directory
    char crawler_path[strlen(pathDirectory) + strlen("/.crawler") + 1];
    strcpy(crawler_path, pathDirectory);
    strcat(crawler_path, "/.crawler");

    struct stat crawler_stat;
    if (stat(crawler_path, &crawler_stat) != 0 || !S_ISREG(crawler_stat.st_mode)) {
        fprintf(stderr, ".crawler file not found in the directory\n");
        return 1;
    }

    // Initializing index file and checking readability
    char *indexFile = argv[2];
    FILE *file = fopen(indexFile, "r");
    if (file == NULL){
        fprintf(stderr, "Error opening file");
        return 1;
    }
    fclose(file);

    // Looping through validate query -- providing new query every time
    while (true) {
    if (feof(stdin)) {
        break;  // break the loop if EOF is reached
    }
    validateQuery(pathDirectory, indexFile);
    }
    

    return 0;
}

// Alphabetic helper function to determine if word is alphabetic 
bool alphabetic(const char *word) {
    for (int i = 0; word[i]; i++) {
        if (!isalpha(word[i])) {
            return false;
        }
    }
    return true;
}

void validateQuery(char* pathDirectory, char* indexFile){
    // Initializing query to arbitrary size 
    char query[300];
    printf("Query: ");
    // Getting query from stdin
    fgets(query, sizeof(query), stdin);

    if (strcmp(query, "\n") == 0) {
        return;
    }

    query[strcspn(query, "\n")] = 0; // getting rid of newline

    // Initializing variables for checks, wrods, and counters
    bool firstCheck = false;
    char* lastWord;
    int andOrCounter = 0;

    char *words = strtok(query, " "); // separate by spaces

    // Create a new string to hold the normalized query
    char normalizedQuery[300] = "";

    // Looping through query
    while (words != NULL) {
        // normalizing word
        normalizeWord(words);

        // Concatenate the normalized word and a space to the normalizedQuery string
        strncat(normalizedQuery, words, sizeof(normalizedQuery) - strlen(normalizedQuery) - 1);
        strncat(normalizedQuery, " ", sizeof(normalizedQuery) - strlen(normalizedQuery) - 1);

        // Checking if firstCheck is false
        if (firstCheck != true){
            // Validating that the first word is not and/or
            if (strcmp(words, "and") == 0 || strcmp(words, "or") == 0) {
                printf("Error: the first word is 'and' or 'or'\n");
                return;
            }
            firstCheck = true;
        }
        // Checking if any non alphabetic characters
        if (!alphabetic(words)){
            printf("Error: the letter contains a non-alphabetic character\n");
            return;
        }

        // Seeing if and/or in a row
        if (strcmp(words, "and") == 0 || strcmp(words, "or") == 0) {
            andOrCounter++;
            if (andOrCounter > 1) {
                printf("Error: more than one 'and' or 'or' in a row\n");
                return;
            }
        } else {
            andOrCounter = 0;
        }
    
        lastWord = words;
        words = strtok(NULL, " ");
    }
    // Checking if last word is and/or
    if (strcmp(lastWord, "and") == 0 || strcmp(lastWord, "or") == 0){
        printf("Error: Last word is 'and' or 'or'\n");
        return;
    }

    // Remove trailing space from normalizedQuery
    if (normalizedQuery[strlen(normalizedQuery) - 1] == ' ') {
        normalizedQuery[strlen(normalizedQuery) - 1] = 0;
    }
    // Printing normalizedQuery and calling parse function
    printf("\nQuery: %s\n", normalizedQuery);
    parseQuery(normalizedQuery, pathDirectory, indexFile);
}



void parseQuery(char* query, char* pathDirectory, char* indexFile){
    // Creating an triple pointer array that clusters ands together in the same index
    char ***entries = malloc(MAX_QUERIES * sizeof(char**));
    // Allocating memory
    for(int i=0; i<MAX_QUERIES; i++){
        entries[i] = malloc(MAX_WORDS * sizeof(char*));
    }
    // Initializing array traversal 
    int subcounter = 0;
    int counter = 0;

    char *words = strtok(query, " "); // separate by spaces

    // Save the last word for later use
    char *lastWord = NULL;

    // Looping through words
    while (words != NULL) {
        // Grouping "or" words into a different cluster -- index
        if (lastWord && strcmp(lastWord, "or") == 0) {
            entries[counter][subcounter] = NULL;
            subcounter = 0;
            counter++;
        }
        // Grouping "and" words together into one "cluster"
        if ((strcmp(words, "or") != 0) && (strcmp(words, "and") != 0)) {
            entries[counter][subcounter] = words; 
            subcounter++;
        }

        lastWord = words;
        words = strtok(NULL, " ");

    }   
    
    //Calling traverse
    traverse(entries, pathDirectory, indexFile, counter);
    // Freeing entries
    for(int i=0; i<MAX_QUERIES; i++){
        free(entries[i]);
        entries[i] = NULL;
    }
    free(entries);
    entries = NULL;
}


void traverse(char*** entries, char* pathDirectory, char* indexFile, int orNumber){
    // Loading the index  
    const char* indexFile1 = "indexFile1";
    index_t* index = index_loader(indexFile, indexFile1);
    if (index == NULL) {
        fprintf(stderr, "index_loader failed\n");
        return;
    }
    // websiteScore is a counter with website ID and score
    counters_t * websiteScore = counters_new();

    // Looping through 
    for (int i = 0; i < orNumber + 1; i++){
        // Making temp counter collect the score and IDs of matches
        counters_t* tempCounter = andTraverse(entries, index, i);
        // Merge counters for "and" and "or"
        void merge_counters(void *arg, const int key, const int count) {
            counters_t *merged_counters = (counters_t *)arg;
            int merged_count = counters_get(merged_counters, key);
            counters_set(merged_counters, key, merged_count + count);
        }
        counters_iterate(tempCounter, websiteScore, merge_counters);
        counters_delete(tempCounter);
    }
    formatPrint(websiteScore, pathDirectory);
    counters_delete(websiteScore);

    // index_delete(index); -- this is where the memory leak can be stopped
}


counters_t* andTraverse(char*** entries, index_t* index, int i) {
    int j = 0;
    // Initializing bag with all matching keys
    counters_t *seen = index_get_counter(index, entries[i][j]);
    j = 1;

    while (entries[i][j] != NULL) {
        counters_t *temp = index_get_counter(index, entries[i][j]);

        // Define the iterator function for the counters_iterate function
        void update_seen(void *arg, const int key, const int count) {
            counters_t *seen_counters = (counters_t *)arg;
            int seen_count = counters_get(seen_counters, key);

            if (seen_count > 0) {
                // If seen score is greater than temp score, update seen 
                if (seen_count > count) {
                    counters_set(seen_counters, key, count);
                }
            }
        }
        // Iterate over the "temp" counters and update the "seen" counters accordingly
        counters_iterate(temp, seen, update_seen);

        // Define the iterator function to remove keys from seen that are not in temp
        void remove_unseen(void *arg, const int key, const int count) {
            counters_t *temp_counters = (counters_t *)arg;
            int temp_count = counters_get(temp_counters, key);

            // If the key does not exist in temp or has a lower count, remove it from seen
            if (temp_count == 0 || temp_count < count) {
                remove_key(seen, key);
            }
        }
        // Iterate over the "seen" counters and remove the keys that do not exist in "temp" counters
        counters_iterate(seen, temp, remove_unseen);

        j++;
    }

    return seen;
}


void formatPrint(counters_t* websiteCounter, char* pathDirectory) {
    int id;
    int score;
    int count = 0; // count of matching documents
    while (true){
        // Initialize the struct to store the maximum counter value and its key
        max_counter_t max_counter_data;
        max_counter_data.key = -1;
        max_counter_data.value = INT_MIN;

        // Iterate through the counters to find the maximum counter value
        counters_iterate(websiteCounter, &max_counter_data, find_max_counter);

        if (max_counter_data.key != -1) {
            count++; // increment count if there is a match
            id = max_counter_data.key;
            score = max_counter_data.value;
            
            // Remove the key with the maximum counter value
            remove_key(websiteCounter, max_counter_data.key);

            // Print the formatted output for this entry
            format(pathDirectory, id, score);
        } 
        else {
            break;
        }
    }

    // If no matching documents were found, print "No documents match"
    if (count == 0) {
        printf("No documents match\n");
    }
    

    printf("--------------------------------------------------\n");

}

// Helper function to find the maximum counter value
void find_max_counter(void *arg, const int key, const int count) {
    max_counter_t *max_counter_data = (max_counter_t *)arg;
    // If the current count is greater than the current maximum value and count is not zero, update the maximum value and its key
    if (count > max_counter_data->value && count != 0) {
        max_counter_data->value = count;
        max_counter_data->key = key;    
    }
}

// Function to remove a key from the counter set
void remove_key(counters_t *ctrs, const int key) {
    if (ctrs == NULL || key < 0) {
        return;
    }
    counters_set(ctrs, key, 0);
}


void format(char* pathDirectory, int id, int score){
    char printString[200];
    snprintf(printString, sizeof(printString), "Score - %d; Doc - %d", score, id);
    // Construct the file path
    char filePath[200];
    snprintf(filePath, sizeof(filePath), "%s/%d", pathDirectory, id);

    // Open the file
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", filePath);
        return;
    }

    // Read the first line from the file and store it in the 'URL' string
    char URL[200];
    if (fgets(URL, sizeof(URL), file) != NULL) {
        // Remove the trailing newline character if present
        size_t len = strlen(URL);
        if (len > 0 && URL[len - 1] == '\n') {
            URL[len - 1] = '\0';
        }
    } else {
        printf("Error: Unable to read from file '%s'\n", filePath);
        fclose(file);
        return;
    }

    // Close the file
    fclose(file);

    // Print the final string
    printf("%s; URL - %s\n", printString, URL);
}
