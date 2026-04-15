CC=gcc
CFLAGS=-Wall
BUILD=./build
DECMOC_INCLUDE=../include
DECMOC_BUILD=../build

.PHONY: all clean 

all: parser_test

parser_test: parser_test.c test.h $(DECMOC_BUILD)/parser.o
	$(CC) $(CFLAGS) -I$(DECMOC_INCLUDE) parser_test.c $(DECMOC_BUILD)/parser.o -lm -o $(BUILD)/parser_test

clean:
	rm -rf $(BUILD)/*
