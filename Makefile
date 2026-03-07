CC=gcc

parser:
	$(CC) parser.c -o ./build/parser -lm

plotter:
	$(CC) plotter.c -o ./build/plotter -lm -lraylib

clean:
	rm ./build/parser ./build/plotter
