#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "file.h"
#include "word.h"
#include "indexer.h"
#include "index.h"
#include "webpage.h"

void indexBuild(char* pd, char* fp); // function declarations
void indexPage(int id, webpage_t *wpg, index_t *ind); 

int main(int argc, char* argv[]){
  // Check for correct number of arguments
  if (argc != 3){
    fprintf(stderr, "Please provide two arguments\n");
    return 1;
  }

  // Allocate memory
  char pagedir[strlen(argv[1]) + strlen("../crawler/") + 1];

  // concatenating directory name
  strcpy(pagedir, "../crawler/");
  strcat(pagedir, argv[1]);

  // checking if the directory exists
  struct stat directory;
  if (stat(pagedir, &directory) == 0){
    if (!S_ISDIR(directory.st_mode)){
      fprintf(stderr, "Directory does not exist\n");
      return 1;
    }
  } else {
    fprintf(stderr, "Error or directory does not exist: %s\n", pagedir);
    return 1;
  }

  // Check if .crawler file exists in the directory
  char crawler_path[strlen(pagedir) + strlen("/.crawler") + 1];
  strcpy(crawler_path, pagedir);
  strcat(crawler_path, "/.crawler");

  struct stat crawler_stat;
  if (stat(crawler_path, &crawler_stat) != 0 || !S_ISREG(crawler_stat.st_mode)) {
    fprintf(stderr, ".crawler file not found in the directory\n");
    return 1;
  }

  // destination file is the second argument
  const char *fp = argv[2];

  // opening the file to write and if it doesn't work then
  FILE *file = fopen(fp, "w");
  if (file == NULL){
    fprintf(stderr, "Error opening file");
    return 1;
  }
  fclose(file);

  // call to the indexBuild function
  indexBuild(pagedir, (char *)fp);

  return 0;
}

void indexBuild(char* pd, char* fp){
  // initializing the index
  index_t *ind = index_new(800);

  int i = 1;

  // concatenating numerical files for each file
  char files[50];
  snprintf(files, sizeof(files), "%s/%d", pd, i);

  // initializing variables 
  FILE *doc;
  char *url;
  int depth;
  char *html;
  webpage_t *wbpg;

  // while the numbered file exists
  while ((doc = fopen(files, "r")) != NULL){
    // reading the url, depth, and html
    url = file_readLine(doc);
    char *depth_str = file_readLine(doc);
    depth = atoi(depth_str); 
    free(depth_str); // Free depth string after converting to int
    html = file_readUntil(doc, '\0'); 

    // if webpage can be created, create it and call index page
    if ((wbpg = webpage_new(url, depth, html)) != NULL){
      indexPage(i, wbpg, ind);
      webpage_delete(wbpg); 
    }
    else{
      fprintf(stderr, "Error loading the file");
    }

    fclose(doc); // Close the file

    // concatenating another file
    i++;
    snprintf(files, sizeof(files), "%s/%d", pd, i);
  }

  // opening the outfile 
  FILE *out_file = fopen(fp, "w");
  if (out_file == NULL) {
    fprintf(stderr, "Error opening output file: %s\n", fp);
    index_delete(ind);
    exit(1);
  }
  
  // once it finishes building the index, printing it and closing the files
  index_print(ind, out_file);
  fclose(out_file);
  index_delete(ind); // Free the index after using it

}

void indexPage(int id, webpage_t *wpg, index_t *ind) {
  // initializing word and counter
  char *word;
  int counter = 0; 

  // while there are still words to retrieve 
  while ((word = webpage_getNextWord(wpg, &counter)) != NULL) { 
    // ignore all words that are less than 3 characters 
    if (strlen(word) > 3) {
      // normalizing word
      char *normalized_word = normalizeWord(word);
      // seeing if the word exists in the index and if not then inserting, then incrementing
      if (index_find(ind, normalized_word, id)) {
        index_increment(ind, normalized_word, id);

      } else {
        index_insert(ind, normalized_word, id);

      }
      // freeing the memory 
      free(normalized_word); 
    }
    free(word); 
  }
}

