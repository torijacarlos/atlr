CC=gcc
CFLAGS=-Wpedantic -Wextra -Wall -std=c23

all: build


build: 
	@${CC} ${CFLAGS} -fPIC atlr.h -o atlr.so
	@echo "build lib successfully"

