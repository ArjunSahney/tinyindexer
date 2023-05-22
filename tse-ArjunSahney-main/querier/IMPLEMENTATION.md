# CS50 TSE Querier
## Implementation Spec

In this document, we will focus on the implementation-specific decisions for the `querier.c` program.

## Data Structures

We use the following data structures:

- `counters_t` from the `counters` module to store the website scores.
- A specific structure `max_counter_t` to store the maximum counter value and its corresponding key.

## Control Flow

The `querier.c` program is implemented in one file, `querier.c`, with several functions. Here is an overview of the control flow:

### `main`

The `main` function is the entry point of the program. It performs the following steps:

1. Checks the number of command-line arguments and validates the directory and index file paths.
2. Enters a loop to continuously validate and parse user queries using the `validateQuery` function.

### `validateQuery`

The `validateQuery` function validates and normalizes the user query. It performs the following steps:

1. Receives the user query from stdin.
2. Validates the query by checking for errors such as the presence of non-alphabetic characters, consecutive "and" or "or" keywords, and "and" or "or" as the first or last word.
3. Normalizes the query by removing leading and trailing spaces and storing the normalized query.
4. Calls the `parseQuery` function with the normalized query, pathDirectory, and indexFile.

### `parseQuery`

The `parseQuery` function parses the normalized query and groups words based on "and" and "or" keywords. It performs the following steps:

1. Initializes a triple pointer array `entries` to cluster "and" words together in the same index.
2. Loops through the normalized query and groups "or" words into a different cluster (index) in the `entries` array.
3. Groups "and" words together within a cluster.
4. Calls the `traverse` function with the `entries` array, pathDirectory, indexFile, and the number of clusters.

### `traverse`

The `traverse` function performs the search and retrieval of matching documents based on the user query. It performs the following steps:

1. Loads the index from the index file using `index_loader` from the `index` module.
2. Initializes a `websiteScore` counter to store website IDs and scores.
3. Loops through each cluster in the `entries` array.
4. Calls the `andTraverse` function with the cluster's `entries`, the index, and the cluster's index.
5. Merges the counters obtained from `andTraverse` into the `websiteScore` counter using `counters_iterate`.
6. Calls the `formatPrint` function with the `websiteScore` counter and the pathDirectory.

### `andTraverse`

The `andTraverse` function performs the search and retrieval of matching documents for each cluster of "and" words. It performs the following steps:

1. Initializes a counter `seen` with the counters associated with the first word in the cluster.
2. Iterates through the remaining words in the cluster.
3. Retrieves the counters associated with each word using `index_get_counter`.
4. Updates the `seen` counter by performing intersection operations with the retrieved counters.
5. Removes keys from `seen` that do not exist in the retrieved counters.
6. Returns the `seen` counter.

### `formatPrint` (continued)

1. Uses `counters_iterate` to find the document with the maximum score.
2. Prints the formatted output for each matching document.
3. If no matching documents are found, it prints "No documents match."

#### `find_max_counter`

The `find_max_counter` function is a helper function used in the `formatPrint` function. It finds the maximum counter value and its corresponding key in a counters data structure. It performs the following steps:

1. Receives the current maximum counter value and key as `arg`.
2. Compares the current count with the maximum counter value stored in `arg`.
3. If the current count is greater than the maximum counter value and not zero, updates the maximum counter value and its key in `arg`.

#### `remove_key`

The `remove_key` function is a helper function used in the `andTraverse` function. It removes a key from a counters data structure. It performs the following steps:

1. Receives a counters data structure `ctrs` and a key `key`.
2. If the counters data structure is not `NULL` and the key is valid (greater than or equal to zero), sets the count of the key to zero using `counters_set`.

#### `format`

The `format` function prints the formatted output for a matching document. It performs the following steps:

1. Constructs the file path for the document using the `pathDirectory` and the document ID.
2. Opens the file using `fopen`.
3. Reads the first line from the file, which represents the URL.
4. Closes the file.
5. Prints the formatted output with the document score, ID, and URL.

## Function Prototypes

Here are the function prototypes for the functions implemented in `querier.c`:

```c
void validateQuery(char* pathDirectory, char* indexFile);
void parseQuery(char* query, char* pathDirectory, char* indexFile);
void traverse(char*** entries, char* pathDirectory, char* indexFile, int orNumber);
counters_t* andTraverse(char*** entries, index_t* index, int i);
void formatPrint(counters_t* websiteCounter, char* pathDirectory);
void find_max_counter(void *arg, const int key, const int count);
void remove_key(counters_t *ctrs, const int key);
void format(char* pathDirectory, int id, int score);
```


## Error Handling and Recovery
- The querier.c program performs error handling and recovery in various places:
    - In the main function, it checks the number of command-line arguments and validates the directory and index file paths. If any errors are encountered, appropriate error messages are printed to stderr, and the program exits with a non-zero status.
    - Error messages are printed to stderr in case of file opening failures or other errors during file operations.
    - The program uses defensive programming tactics to check for invalid parameters or unexpected behavior and handles them accordingly, preventing crashes or undefined behavior.
    - Memory allocation failures are handled by checking the return value of malloc and freeing allocated memory in case of failures.
    - If no matching documents are found in the formatPrint function, it prints "No documents match."

## Testing Plan: 
The testing plan for the querier.c program includes both unit testing and integration/system testing. The unit testing phase involves testing each function individually with different inputs to ensure they produce the expected outputs. The integration/system testing phase includes testing various scenarios such as incorrect arguments, valid "and" and "or" queries, capitalization variations, no matching documents, and complicated queries. Fuzz testing is also performed by generating random queries and feeding them to the querier program for evaluation.
