LIBRARY_CS50 = ../libcs50
LIBRARY_COMMON = ../common

COMPILER_FLAGS = -Wall -pedantic -std=c11 -ggdb -I$(LIBRARY_CS50) -I$(LIBRARY_COMMON)
C_COMPILER = gcc

APP_NAME = crawler
OBJECT_FILES = crawler.o ../common/pagedir.o
LIB_FILES = $(LIBRARY_CS50)/libcs50-given.a

.PHONY: build testing clean_up test valgrind

$(APP_NAME): $(OBJECT_FILES) $(LIB_FILES)
	$(C_COMPILER) $(COMPILER_FLAGS) $(OBJECT_FILES) $(LIB_FILES) -o $(APP_NAME)

crawler.o: $(LIBRARY_CS50)/webpage.h $(LIBRARY_CS50)/bag.h $(LIBRARY_CS50)/hashtable.h $(LIBRARY_COMMON)/pagedir.h
	$(C_COMPILER) $(COMPILER_FLAGS) -c crawler.c

../common/pagedir.o: ../common/pagedir.h
	$(C_COMPILER) $(COMPILER_FLAGS) -c ../common/pagedir.c -o ../common/pagedir.o

# testing
testing:
	bash -v testing.sh &> testing.out

# new test target
test: $(APP_NAME)
	./testing.sh 2>&1 | tee testing.out

# Valgrind 

valgrind: $(APP_NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_output.txt ./$(APP_NAME) "http://cs50tse.cs.dartmouth.edu/tse/letters/index.html" "h1" "10" || true



# cleaning up
clean_up:
	rm -rf *.dYSM
	rm -rf *~ *.o
	rm -rf $(APP_NAME)

