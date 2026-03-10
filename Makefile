all:
	gcc -W -Wall -pedantic -O2 -std=c99 main.c chip8.c display.c input.c -o chip8 -lSDL2

clean:
	rm -f chip8

.PHONY: all clean
