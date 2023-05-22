#ifndef __WORD_H
#define __WORD_H

// Includes
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/**************** normalizeWord ***********************************/
/* Inputs: char* word

The normalizeWord function takes in a word, loops through it's characters and converts them all to lowercase, returning the word in lowercase. 

*/
void normalizeWord(char* word);

#endif // __WORD_H
