CC=gcc
BUILD=./build
INCLUDE=./include
SOURCE=./src
CFLAGS=-Wall

ifdef D
	CFLAGS += -g
endif

.PHONY: default all parser plotter clean

default: parser

all: dirs parser plotter

$(BUILD)/parser.o: $(SOURCE)/parser.c $(SOURCE)/lexer.c $(SOURCE)/utils.c $(INCLUDE)/parser.h
	$(CC) $(CFLAGS) -I./$(INCLUDE) -c $(SOURCE)/parser.c -o $(BUILD)/parser.o $(FLAGS)

$(BUILD)/parser_main.o: $(SOURCE)/parser.c $(SOURCE)/lexer.c $(SOURCE)/utils.c $(INCLUDE)/parser.h
	$(CC) $(CFLAGS) -I./$(INCLUDE) -c $(SOURCE)/parser.c -o $(BUILD)/parser_main.o -DPARSER_MAIN

parser: $(BUILD)/parser
$(BUILD)/parser: $(BUILD)/parser_main.o
	$(CC) $(CFLAGS) $(BUILD)/parser_main.o -o $(BUILD)/parser -lm

plotter: $(BUILD)/plotter
$(BUILD)/plotter: $(SOURCE)/plotter.c $(INCLUDE)/parser.h $(BUILD)/parser.o
	$(CC) $(CFLAGS) -I./$(INCLUDE) $(SOURCE)/plotter.c $(BUILD)/parser.o -o $(BUILD)/plotter -lm -lraylib

dirs:
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)/*
