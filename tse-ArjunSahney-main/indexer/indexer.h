#ifndef __INDEXER_H
#define __INDEXER_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "file.h"
#include "word.h"
#include "index.h"
#include "webpage.h"


/**************** main ***********************************/
/*Inputs: arguments from stdin

The main function takes in arguments from stdin, validating that it has the correct number of arguments. Then, it 
checks if the pageDir provided exists and subsequently if the file can be initialized. Finally, it calls indexBuilder 
after parsing the pageDir and filename.
*/
int main(int argc, char* argv[]);

/**************** indexBuild ***********************************/
/* Inputs: pageDir and destination filename

The indexBuild function intiailizes an index data structure, which stores words, docID, and frequency. After initializing, 
the function loops through each file until there are none and calls indexPage.

*/
void indexBuild(char* pd, char* fp);

/**************** indexPage ***********************************/
/*Inputs: id number, webpage, and index ADT

The indexPage function gets the next word from the webpage until there aren't any left, checks if the length of the word is
greater than 3, and normalizes that word (converts to lower case). Then, either add the word to the data structure or increment
it.  

*/
void indexPage(int id, webpage_t *wpg, index_t *ind);


#endif // __INDEXER_H

