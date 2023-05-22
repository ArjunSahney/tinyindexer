L = ../libcs50
M = ../common

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I $M -I $L
LLIBS = $M/common.a $L/libcs50-given.a

OBJ = querier.o word.o index.o counters.o hashtable.o

all: querier fuzzquery

querier: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LLIBS)

querier.o: querier.c $M/word.h $M/index.h $L/counters.h $L/hashtable.h
	$(CC) $(CFLAGS) -c querier.c

word.o: $M/word.c $M/word.h
	$(CC) $(CFLAGS) -c $M/word.c

index.o: $M/index.c $M/index.h
	$(CC) $(CFLAGS) -c $M/index.c

counters.o: $L/counters.c $L/counters.h
	$(CC) $(CFLAGS) -c $L/counters.c

hashtable.o: $L/hashtable.c $L/hashtable.h
	$(CC) $(CFLAGS) -c $L/hashtable.c

fuzzquery: fuzzquery.o
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

#Phony Targets
.PHONY: clean test valgrind

clean:
	rm -f core *core.*
	rm -f querier fuzzquery *~ *.o

test: 
	bash -v testing.sh

valgrind:
	echo review home | valgrind ./querier ../../shared/tse/output/toscrape-1 ../../shared/tse/indices/toscrape-1.ndx
