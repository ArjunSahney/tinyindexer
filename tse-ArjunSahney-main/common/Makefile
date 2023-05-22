# Makefile for common module
# Arjun Sahney, CS50

OBJECTS = pagedir.o index.o word.o
LIBRARY = common.a
LIB_CS50 = ../libcs50

COMPILER_FLAGS = -Wall -pedantic -std=c11 -ggdb -I$(LIB_CS50)
COMPILER = gcc

# Building lib
$(LIBRARY): $(OBJECTS)
	ar cr $(LIBRARY) $(OBJECTS)

# object files depend on the header files
pagedir.o: pagedir.c $(LIB_CS50)/webpage.h $(LIB_CS50)/mem.h
	$(COMPILER) $(COMPILER_FLAGS) -c $<

index.o: index.c index.h $(LIB_CS50)/hashtable.h $(LIB_CS50)/counters.h
	$(COMPILER) $(COMPILER_FLAGS) -c $<

word.o: word.c word.h
	$(COMPILER) $(COMPILER_FLAGS) -c $<

.PHONY: clean

# clean up
clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f $(LIBRARY)
