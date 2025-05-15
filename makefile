CC=gcc
CFLAGS=-Wpedantic -Wextra -Wall -std=c23
NOISE_WARNINGS=-Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-missing-braces

all: build

test: 
	@${CC} ${CFLAGS} -lm -DATLR_DEBUG ${NOISE_WARNINGS} -O2 test.c -o test.o
	@echo "-- testing"
	@./test.o
	@rm ./test.o

debug: 
	@${CC} ${CFLAGS} -DATLR_DEBUG -g -fPIC atlr.h -o atlr.debug.so
	@echo "-- debug: build lib successfully"

release: 
	@${CC} ${CFLAGS} -O2 -fPIC atlr.h -o atlr.release.so
	@echo "-- release: build lib successfully"


build:  debug release

.PHONY: test
