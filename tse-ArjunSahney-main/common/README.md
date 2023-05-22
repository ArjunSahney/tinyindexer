# README for pagedir.c and index

## Index:
The Index module provides a simple and efficient way to maintain a mapping of keys to counters. It uses a hashtable to store the keys and a counters data structure to store the counts associated with each key. This module is particularly useful when you need to track the frequency of items, such as words in a document.

## Assumptions
The Index module is designed with the assumption that the keys to be stored are strings, and the associated counts are integers. It is assumed that the number of keys and their associated counts can grow dynamically, but the initial number of slots in the hashtable should be specified when creating a new index. Additionally, the module assumes that users will primarily perform insertions, incrementations, searches, and retrievals of counts for specific keys.

The provided page directory is a valid path, and the user has permissions to create directories and files in that path.
The webpage_t objects have valid content, including URL, depth, and HTML.
The docID is a unique integer that increments with each new webpage saved.
The filename length will not exceed 1000 characters.
The program will handle errors gracefully, providing meaningful error messages.

## Approaches
The approach taken in the implementation of the Index module is to use a hashtable for efficient key lookups and a counters data structure to store the counts associated with each key. This combination ensures that the module is both memory-efficient and provides fast access to the stored data. The module exposes a set of functions for creating and managing an index, inserting new key-count pairs, incrementing counts, searching for keys, and retrieving counts. The module also provides functions for deleting an index, printing its contents, and loading an index from a file.

pagedir_init: This function initializes the page directory by checking if the directory exists, and creating it if not. It then constructs the pathname for the .crawler file in the directory, opens the file for writing, and closes it. If there is an error while opening the file, the function returns false. Otherwise, it returns true.

pagedir_save: This function saves the webpage data to a file in the page directory. It constructs the pathname for the page file in the page directory using the provided docID. It then opens the file for writing, prints the URL, depth, and contents of the webpage, and closes the file. If there's an error opening the file, a meaningful error message is printed to stderr.
