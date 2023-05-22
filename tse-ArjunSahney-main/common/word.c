#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "word.h"

/* Check word.h for detailed explanation

*/
void normalizeWord(char* word) {
    // initializing counter and length of word
    int i;
    int len = strlen(word);
    
    // Looping through word and converting to lower case
    for (i = 0; i < len; i++) {
        word[i] = tolower(word[i]);
    }
}
