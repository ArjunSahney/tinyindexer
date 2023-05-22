#ifndef PAGEDIR_H
#define PAGEDIR_H

#include "../libcs50/webpage.h"

/* pagedir_init takes one parameter, which is the directory into which the files should be made, and then initializes the directory itself. The functino returns true if it is initialized and false if there is a faliure in initiailizing it.
 */

bool pagedir_init(const char* pageDirectory);

/* pagedir_save takes three parameters; first, it takes the webpage it is trying to save, the aformentioned pagedirectory in which the file it is saved, and finally the docID (simply the doc name). This function creates a document in the given page directory with the title of the document ID and then saves the page's content to the document.
*/


void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

#endif // PAGEDIR_H

