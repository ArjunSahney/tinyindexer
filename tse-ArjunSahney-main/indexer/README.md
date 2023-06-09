# Indexer 

## Description:
The indexer is a C program designed to create an inverted index from the content of a directory created by a web crawler. The inverted index maps words to the web pages they were found on, along with the frequency of the word on each page. 
The indexer can be used for efficient searching and ranking of web pages based on user queries.

## Usage:
To compile the indexer, navigate to the indexer directory and run:

make

To run the indexer, use the following command:

./indexer <pagedir> <indexfile>

where <pagedir> is the directory produced by the web crawler and <indexfile> is the output file where the inverted index will be stored.

## Assumptions: 

The indexer assumes that the provided input directory (<pagedir>) has been generated by a 
compatible web crawler and contains a ".crawler" file for verification. It expects valid webpage
files in the input directory, named with increasing integer values, and containing metadata (URL and depth) 
along with valid HTML content. The indexer will create or overwrite the specified output file (<indexfile>) 
and ignore words with fewer than three characters. Additionally, the inverted index data structure is designed
to handle the number of unique words encountered in the input directory.
