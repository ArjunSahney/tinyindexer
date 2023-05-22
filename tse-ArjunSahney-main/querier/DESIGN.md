# Design Specifcations for Querier:
Here we focus on: 
- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

## User interface: 
While the user has to input a pageDirectory and a indexFilename, that will be covered in inputs and outputs. The user's interface to the program is through the command line:

The user should simply input the query with or without "and" or "or" statements (with an "and" precedence) as below:
```bash
$ querier dogs and cats
```
```bash
$ querier dogs or cats
```
```bash
$ querier dogs 
```

There are a number of boundary cases including querying "and"/"or" first or last:
```bash
$ querier and dogs
```

However, in general the query response is relatively straightforward with high optionality for the user.

## Inputs and Outputs:
*Inputs:* there are two file inputs that the user must provide: a pageDirectory for the directory produced by Crawler and an indexFilename that is the file produced by indexer of that respective pageDirectory.
     - The program should validate that it recieved the two command line arguments 
     - After this it will prompt the user to provide the query as aforementioned 


*Outputs*: the resulting output will be solely from stdout. The output will simply be a list of queries that match the resulting query in an ideal state as displayed below: 

``` bash
Query: exit
Matches 3 documents (ranked):
score	2 doc	6: http://cs50tse.cs.dartmouth.edu/tse/wikipedia/Unix.html
score	1 doc	7: http://cs50tse.cs.dartmouth.edu/tse/wikipedia/C_(programming_language).html
score	1 doc  59: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/category/books/poetry_23/index.html
```

Or the output will return an error or no queries found:
``` bash
Query? Tiny Search Engine
Query: tiny search engine
No documents match.
```

## Functional Decomposition into Modules:
These are the functions that are to be created:

     1. `validateQuery`: Validates and processes the user query.
     2. `parseQuery`: Parses the normalized query and separates it into clusters of "and" and "or" words.
     3. `traverse`: Traverses through the clusters of words, retrieves matching counters from the index, and merges them.
     4. `andTraverse`: Performs an "and" traversal on a cluster of words, finding matching keys in the index and returning the counters.
     5. `formatPrint`: Prints the formatted output of the matching documents based on their scores and IDs.
     6. `find_max_counter`: Helper function to find the maximum counter value in a counter set.
     7. `remove_key`: Removes a key from a counter set.
     8. `format`: Formats and prints the details of a matching document, including its score, ID, and URL.
     9. `main`: The main function that initializes the program, validates command-line arguments, checks directory and file existence, and enters a loop for query validation and processing.

 ## Pseudocode: 
 This outlines the basic logical and algorithmic flow of the program from a macro level.
 '''
 - Import libraries.

- Define constants and data structures.

- Define function prototypes.

- Define main function:
    - Validate command-line arguments.
    - Initialize path directory and index file.
    - Check directory and file existence.
    - Enter query validation and processing loop.

- Define helper function: alphabetic(word)
    - Check if all characters in the word are alphabetic.

- Define function: validateQuery(pathDirectory, indexFile)
    - Get user query.
    - Normalize and validate the query.
    - Parse the query into word clusters.
    - Call parseQuery function.

- Define function: parseQuery(query, pathDirectory, indexFile)
    - Create word clusters from the query.
    - Call traverse function.

- Define function: traverse(entries, pathDirectory, indexFile, orNumber)
    - Load index and create website score counter.
    - Loop through word clusters.
    - Retrieve counters for each cluster.
    - Merge counters for "and" and "or" clusters.
    - Print formatted output of matching documents.

- Define function: andTraverse(entries, index, i)
    - Initialize seen counter with the first word.
    - Loop through remaining words in cluster.
    - Update seen counter based on matches.
    - Remove keys not present in current counter.
    - Return seen counter.

- Define function: formatPrint(websiteCounter, pathDirectory)
    - Loop until all matching documents are printed.
    - Find maximum counter value.
    - Remove key with maximum counter value.
    - Print formatted output for document.

- Define helper function: find_max_counter(arg, key, count)
    - Update maximum counter value if count is greater.

- Define helper function: remove_key(ctrs, key)
    - Set count of key in counter set to 0.

- Define function: format(pathDirectory, id, score)
    - Create formatted string for printing.
    - Construct file path for document.
    - Open file, read URL, and close file.
    - Print formatted string with document details.

- Call main function to start the program.


 '''
 ## Major Data Structures:
     1. **Triple Pointer Array**: Used to hold word clusters derived from the user query.

     2. **max_counter_t Structure**: A structure to store the maximum counter value and its corresponding key during iteration.

     3. **counters_t Structure**: A data structure representing a counter set, used to store website scores and IDs.

     4. **index_t Structure**: A data structure representing the index, used to store and retrieve counters for each word.

## Testing Plan: 
     The provided testing approach involves running a series of test cases using the querier program. Here's a summary of the test cases:

          1. Too many arguments: Tests the case when there are too many command-line arguments passed to the querier program.

          2. Too few arguments: Tests the case when there are too few command-line arguments passed to the querier program.

          3. Incorrect Page Directory: Tests the case when an incorrect page directory path is provided as an argument.

          4. Incorrect Index File: Tests the case when an incorrect index file path is provided as an argument.

          5. Valid "and" Test: Tests a valid query using the "and" operator.

          6. Valid "or" Test: Tests a valid query using the "or" operator.

          7. Capital Multiple Test: Tests a query with capitalized words and multiple operators.

          8. Nothing Found: Tests a query that doesn't match any documents.

          9. Complicated Tests: Tests queries with multiple operators and keywords.

          10. Fuzz Testing: Performs fuzz testing by generating random queries and feeding them into the querier program.

     
