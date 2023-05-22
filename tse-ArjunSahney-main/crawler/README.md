# Web Crawler:

This web crawler is a simple command-line application that crawls a given seed URL up to a specified depth and saves the web pages to a directory. It avoids saving duplicates and handles internal and external URLs.

## Approach:

The web crawler takes seed URL, page directory, and maximum crawling depth as command-line arguments, initializes the seed URL, and creates a new webpage object. It uses a hashtable to store seen webpages and a bag for webpages to crawl. As it crawls, it fetches the webpage, saves it to the directory if the content is available, and avoids duplicates. If the current depth is less than the maximum, it scans for new URLs. After crawling, it cleans up memory by deleting the hashtable and bag.

## Assumptions:
In the development of crawler.c, several assumptions were made. It was assumed that the command-line arguments would be valid and formatted correctly, including a properly formatted seed URL, an existing page directory, and a maximum crawling depth within the acceptable range (0-10). Additionally, it was assumed that the provided helper functions for handling webpages, hashtables, and bags would function as expected. The crawler also assumes that fetched webpages have valid content and that internal URLs should be prioritized for crawling, while external URLs are only logged as seen. Finally, it was assumed that there would be no need to handle cases where the maximum number of hashtable slots (200) was exceeded during the crawling process.
