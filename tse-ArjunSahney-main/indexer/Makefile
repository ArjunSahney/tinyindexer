# Makefile for indexer
#
# ArjunSahney

L = ../libcs50
LC = ../common

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(LC)
CC = gcc

PROGS = indexer indextest
OBJS_IND = indexer.o $(LC)/pagedir.o $(LC)/word.o $(LC)/index.o
OBJS_IND_TEST = indextest.o $(LC)/index.o
LLIBS = $L/libcs50-given.a $(LC)/common.a

.PHONY: all clean test valgrind_indexer valgrind_indextest

all: indexer indextest

indexer: $(OBJS_IND) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS_IND) $(LLIBS) -o indexer

indextest: $(OBJS_IND_TEST) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS_IND_TEST) $(LLIBS) -o indextest

indexer.o: $L/hashtable.h $L/counters.h $L/file.h $L/webpage.h $(LC)/index.h $(LC)/pagedir.h $(LC)/word.h

indextest.o: $(LC)/index.h $L/file.h

test:
	bash -v testing.sh

valgrind_indexer: indexer
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./indexer h1 valtest

valgrind_indextest: indextest
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./indextest h1 valtest

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROGS)
	rm -f core

