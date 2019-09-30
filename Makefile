#Copyright Bianca Ciuche 2018

# compiler
CC = gcc

# compiler flags
CFLAGS = -Wall -Wextra -std=c99

# name of the exec
EXE = snowfight

build: main.c functions.c struct.c functions.h
	$(CC) $(CFLAGS) *.c -o $(EXE)

clean:
	rm -f $(EXE)
