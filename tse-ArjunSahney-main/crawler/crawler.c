#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../common/pagedir.h"
#include <string.h>

// Function prototypes
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/* The main function here takes in arguments from the command line and if there aren't 3 arguments, then it should throw an error. Then, it should call parseargs and crawl.
 
   */

int main(const int argc, char* argv[]){
  // If not 3 arguments, then print errors
    if (argc != 4){
        fprintf(stderr, "Please provide three arguments\n");
        exit(1);
    }
    // initializing max depth 
    int maxDepth = atoi(argv[3]);

    //printing parameters
    printf("Seed URL: %s\n", argv[1]);
    printf("Page directory: %s\n", argv[2]);
    printf("Max depth: %s\n", argv[3]);

    // calling the functions
    parseArgs(argc, argv, &argv[1], &argv[2], &maxDepth);
    crawl(argv[1], argv[2], maxDepth);

    return 0;
}
/**
 * parseArgs: validates and processes input arguments
 * @argc: argument count
 * @argv: array of argument strings
 * @seedURL: pointer to the seed URL string
 * @pageDirectory: pointer to the page directory string
 * @maxDepth: pointer to the maximum crawl depth
 **/

static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth){
    // normalized url should be seed url 
    char* normalizedURL = normalizeURL(*seedURL);

    // if it isn't an internal url, print error
    if (!isInternalURL(normalizedURL)){
        fprintf(stderr, "This is not an accepted URL\n");
        exit(1);
    }
    // if max depth not in range
    if (*maxDepth > 10 || *maxDepth < 0){
        fprintf(stderr, "Max depth should be below 10 and non-negative\n");
        exit(1);
    }
    // if faliure initializing file
    if (!pagedir_init(*pageDirectory)){ 
        fprintf(stderr, "Failure initializing file.\n");
        exit(1);
    }

    *seedURL = normalizedURL;
}

/**
 * crawl: performs the main web crawling process
 * @seedURL: the seed URL string
 * @pageDirectory: the page directory string
 * @maxDepth: maximum crawl depth
 **/

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth){
    // initializing variables
    const int MAX_SLOTS = 200;

    int id = 1;

    // creating webpage, hashtable, and bag
    webpage_t *seed = webpage_new(seedURL, 0, NULL);

    hashtable_t* pages_seen = hashtable_new(MAX_SLOTS);

    hashtable_insert(pages_seen, seedURL, "");

    bag_t* pages_to_crawl = bag_new();

    bag_insert(pages_to_crawl, seed);

    webpage_t *wpg;

    // while bag is not empty 
    while ((wpg = bag_extract(pages_to_crawl)) != NULL){
        if (webpage_fetch(wpg)) {
            if (webpage_getHTML(wpg) != NULL){
                printf("%2d %9s: %s\n", webpage_getDepth(wpg), "Fetched", webpage_getURL(wpg));

                // Check for duplicate
                char *found = hashtable_find(pages_seen, webpage_getURL(wpg));
                // if already seen
                if (found == NULL || strcmp(found, "") == 0) {
                    pagedir_save(wpg, pageDirectory, id);
                    id++;
                }
                
                // if depth is greater than max
                int depth = webpage_getDepth(wpg);
                if (depth < maxDepth){
                    pageScan(wpg, pages_to_crawl, pages_seen);
                }

                webpage_delete(wpg);
            }
        }
        // else faliure
        else{
            fprintf(stderr, "Failure to fetch webpage\n");
            hashtable_delete(pages_seen, NULL);
            bag_delete(pages_to_crawl, NULL);
            exit(1);
        }
    }
    // hashtable delete and bag delete
    hashtable_delete(pages_seen, NULL);
    bag_delete(pages_to_crawl, webpage_delete);
}

/*

pageScan: scans the given webpage for new URLs and adds them to the crawl list

@page: pointer to the webpage to scan

@pagesToCrawl: pointer to the bag of webpages to crawl

@pagesSeen: pointer to the hashtable of seen webpages
*/

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
    // initializing variables 
    int pos = 0;
    int depth = webpage_getDepth(page);

    char* nextURL;

    // while loop to check if next URL is not null
    while ((nextURL = webpage_getNextURL(page, &pos)) != NULL){
        char *normalizedURL = normalizeURL(nextURL);
        
        // if URL is non-internal
        if (isInternalURL(normalizedURL)){
            
            webpage_t* current = webpage_new(normalizedURL, depth+1, NULL);
            // insert hashtable
            if (hashtable_insert(pagesSeen, normalizedURL, "")){
                printf("%2d %9s: %s\n", depth + 1, "Seen", normalizedURL);
                bag_insert(pagesToCrawl, current);
                printf("%2d %9s: %s\n", depth + 1, "Inserted", normalizedURL);          
            // else seen and duplicate 
            }
            else{
                printf("%2d %9s: %s\n", depth + 1, "Seen", normalizedURL);
                printf("%2d %9s: %s\n", depth + 1, "Duplicate", normalizedURL);
                webpage_delete(current);
            }
        }
        else {
            printf("%2d %9s: %s\n", depth + 1, "Seen", normalizedURL);
            printf("%2d %9s: %s\n", depth + 1, "External", normalizedURL);
        }
      free(nextURL);
    }
}
