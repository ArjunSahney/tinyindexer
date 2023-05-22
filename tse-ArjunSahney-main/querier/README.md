# README for Querier
Querier is the final part of the Tiny Search Engine that brings together crawler and indexer and returns queries. It accepts queries from users and returns documents (website URLs) that match the queries based on an index file created by a web crawler. The program groups words using and and or logical operators, with and taking precedence.

## FOR GRADERS: 
There are two primary bugs with the code: 
    1. There is a memory leak that is caused by the index not being freed in the traverse() function. Once I free it there are no memory leaks and the program works and outputs perfectly but a segmentation fault occurs. I've tried continously to get rid of the segmentation fault but it seems to be from an imported module.

    2. This is a minor bug but sometimes and only when it is inputted from a shell script (stdin works perfectly) it can duplicate the output. Otherwise, the program works perfectly. 

## Assumptions: 
The program makes certain assumptions to operate effectively. Firstly, it sets a limit of 100 words and queries each. However, these limits can be adjusted by modifying the MAX_WORDS and MAX_QUERIES definitions in the source code. Additionally, the program utilizes a hashtable as the index data structure. It assumes the availability and accessibility of both the index file and the directory that contains the crawled documents. It is essential for each directory to include a .crawler file, which serves as an indication that it has been crawled.

## Approach: 
Firstly, the input arguments are validated, and the existence of the directory and index file is checked. Once the validation is complete, the program enters a loop to continuously accept queries from the user until the end of the file is reached.

For each query, it undergoes a normalization and parsing process. Non-alphabetic characters are considered invalid during this process.

Subsequently, the parsed query is processed. Words are grouped together based on the presence of "and" and "or" operators. These groups are then traversed to identify the matching documents.

The matching documents are assigned scores, and the scores are merged for the "and" and "or" groups separately.

Finally, the program prints out the documents in descending order of their scores, thereby providing the user with the most relevant results.


