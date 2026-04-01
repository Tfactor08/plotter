CC=gcc
CFLAGS=-Wall
INCLUDE=../include
BUILD=../build

all: test.c $(BUILD)/parser.o
	$(CC) $(CFLAGS) -I$(INCLUDE) test.c $(BUILD)/parser.o -lm -o test
