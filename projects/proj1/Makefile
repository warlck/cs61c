BINARIES=depth_map quadtree
CFLAGS=-Wall -g -std=c99

.PHONY: clean

default: all

all: ${BINARIES}

depth_map: calc_depth.c depth_map.c utils.c
	gcc ${CFLAGS} -o $@ $^ -lm

quadtree: quadtree.c make_qtree.c utils.c
	gcc ${CFLAGS} -o $@ $^ -lm

check: ${BINARIES}
	python check.py

clean:
	rm -rf ${BINARIES} test/output/*

