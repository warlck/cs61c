UNAME_S := $(shell uname -s)
CC=gcc
LD=gcc
CFLAGS=-ggdb -Wall -pedantic -std=c99
LDFLAGS=

ifeq ($(UNAME_S), Darwin)
    MEMCHECK=valgrind --tool=memcheck --leak-check=full --track-origins=yes --dsymutil=yes --suppressions=osx_vector.supp
endif

ifeq ($(UNAME_S), Linux)
    MEMCHECK=valgrind --tool=memcheck --leak-check=full --track-origins=yes
endif

VECTOR_OBJS=vector.o vector-test.o
VECTOR_PROG=vector-test

all: $(VECTOR_PROG)

$(VECTOR_PROG): $(VECTOR_OBJS)
	$(LD) -o $(VECTOR_PROG) $(VECTOR_OBJS) $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $<

vector-memcheck: $(VECTOR_PROG)
	$(MEMCHECK) ./$(VECTOR_PROG)

clean: 
	-rm -rf core *.o *~ "#"*"#" Makefile.bak $(VECTOR_PROG) *.dSYM
        
vector.c: vector.h
vector-test.c: vector.h

