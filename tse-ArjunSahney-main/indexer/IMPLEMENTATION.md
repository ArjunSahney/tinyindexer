# CS50 TSE Indexer
## Implementation Spec
In this specification document, we will cover the basic implementation of the indexer 
through looking at:
-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures
The primary data structure that will be employed is a created ADT called an index.
index.c will be the module that builds the data structure, which is a hashtable with 
void* as its key and the counter ADT serving as the hashtable's value.

The size of the hashtable will be initialized to a high number as we do not know the size.

## Control Flow:
Within index.c, there will be three functions:

### main:
int main(int argc, char* argv[]);

The main function takes in arguments from stdin, validating that it has the correct number of arguments. Then, it 
checks if the pageDir provided exists and subsequently if the file can be initialized. Finally, it calls indexBuilder 
after parsing the pageDir and filename.

### indexBuild:
void indexBuild(char* pd, char* fp);

The indexBuild function intiailizes an index data structure, which stores words, docID, and frequency. After initializing, 
the function loops through each file until there are none and calls indexPage:

creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage

### indexPage:
void indexPage(int id, webpage_t *wpg, index_t *ind);

The indexPage function gets the next word from the webpage until there aren't any left, checks if the length of the word is
greater than 3, and normalizes that word (converts to lower case). Then, either add the word to the data structure or increment
it:

steps through each word of the webpage,
   skips trivial words (less than length 3),
   normalizes the word (converts to lower case),
   looks up the word in the index,
     adding the word to the index if needed
   increments the count of occurrences of this word in this docID

## Other Modules:

### index.c:
The program creates a new index, inserts and increments word counts, finds word counts, 
deletes the index, gets word counts, and prints the index to a file. It also includes a 
loader function that reads a file and creates an index from it, which is then printed to 
another file.

### normalizeWord.c:
The program converts words with capital letters to entirely lowercase.

## Error Handling and Recovery:
index.c has checks in place to ensure that pointers are not null and that 
parameters have valid values. For example, it checks if the index is null when 
inserting, incrementing, finding, and deleting entries, and if the key is null or 
the counter is negative when inserting or finding entries.

The program also has recovery mechanisms in place to handle errors.
 For example, if a counter cannot be created, the program returns false and 
does not insert the key-value pair into the index. If a file cannot be opened 
for reading or writing, the program prints an error message to stderr and returns
or exits with an error code.

Similarly, indexer.c has checks in place to ensure that the program is provided with valid arguments 
and that files and directories can be opened for reading and writing. If any of these checks fail, the 
program prints an error message to stderr and returns or exits with an error code.

The program also has recovery mechanisms in place to handle errors gracefully.
For example, if a webpage cannot be created from a file, the program prints an
error message to stderr and skips that file. If the output file cannot be opened
for writing, the program prints an error message to stderr and deletes the index 
before exiting with an error code.

## Testing:
Unit Testing:
A program indextest will serve as a unit test for the index module; it reads 
an index file into the internal index data structure, then writes the index out 
to a new index file.

Integration testing:
### Invalid arguments -- no arguments
./indexer 

### Invalid arguments -- 1 argument
./indexer h1 

### Invalid arguments -- 3+ arguments
./indexer h1 test1 test2 test3

### Invalid pageDirectory
./indexer h5 test1

### Invalid indexFile -- invalid directory
./indexer h1 testFolder/test1

### Invalid indexFile -- read only directory
./indexer h1 readDir/ngw

### Invalid indexFile -- read only file
./indexer h1 readOnly

### Valid Index Test
./indexer h1 test1

### Valid Index Test 
./indexer h2 test2

### Valid Index Test
./indexer ../../shared/tse/output/toscrape-1 test3


