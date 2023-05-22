#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include <errno.h>

// Author: Arjun Sahney
// PageDir module

#include <sys/stat.h> // Add this include at the top of the pagedir.c file

bool pagedir_init(const char* pageDirectory) {
    // Check if the directory exists, and create it if not
    struct stat st;
    if (stat(pageDirectory, &st) == -1) {
        if (mkdir(pageDirectory, 0700) == -1) {
            perror("Error creating directory");
            return false;
        }
    }

    // Construct the pathname for the .crawler file in that directory
    char filename[1000];
    strcpy(filename, pageDirectory);
    strcat(filename, "/.crawler");

    // Add print statement to check the filename value
    printf("Filename in pagedir_init: %s\n", filename);

    // Open the file for writing; on error, return false
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s (Error number: %d, Error message: %s)\n", filename, errno, strerror(errno));
        return false;
    }

    // Close the file and return true
    fclose(file);
    return true;
}



void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    // Construct the pathname for the page file in pageDirectory
    char filename[1000];
    snprintf(filename, sizeof(filename), "%s/%d", pageDirectory, docID);

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        // Print the URL
        fprintf(file, "%s\n", webpage_getURL(page));
        
        // Print the depth
        fprintf(file, "%d\n", webpage_getDepth(page));
        
        // Print the contents of the webpage
        fprintf(file, "%s", webpage_getHTML(page));
        
        // Close the file
        fclose(file);
    } else {
        fprintf(stderr, "Failed to open file: %s\n", filename);
    }
}
