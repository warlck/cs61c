CC=gcc
LD=gcc
CFLAGS=-ggdb -Wall -pedantic -std=gnu99 -O3
LDFLAGS=

EX1_PROG=matrixMultiply

EX2_PROG=transpose

PROGS=$(EX1_PROG) $(EX2_PROG)

all: $(PROGS)

ex1: $(EX1_PROG)
	$(CC) -o $(EX1_PROG) $(CFLAGS) $(EX1_PROG).c
	./$(EX1_PROG)
	./$(EX1_PROG) 2

ex2: $(EX2_PROG)
	$(CC) -o $(EX2_PROG) $(CFLAGS) $(EX2_PROG).c
	./$(EX2_PROG)

clean:
	-rm -rf core *.o *~ "#"*"#" Makefile.bak $(PROGS) *.dSYM
