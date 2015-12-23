.PHONY: all build run clean

all: build run

build:
	gcc -Wall -Wparentheses -D PL_PC -c main.c
	gcc main.o -lm -o main.out

run:
	./main.out

clean:
	rm -rf *.o
	rm -rf *.out
