CC?=gcc
CFLAGS?=-Wall -std=c99 -g
BINARIES=hello lfsr ll_cycle ll_equal

.PHONY: clean

default: all

all: $(BINARIES)

%: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BINARIES)

