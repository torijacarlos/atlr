CC=gcc
CFLAGS=-Wpedantic -Wextra -Wall -std=c23

all: build

debug: 
	@${CC} ${CFLAGS} -DATLR_DEBUG -g -fPIC atlr.h -o atlr.debug.so
	@echo "-- debug: build lib successfully"

release: 
	@${CC} ${CFLAGS} -O2 -fPIC atlr.h -o atlr.release.so
	@echo "-- release: build lib successfully"


build:  debug release

