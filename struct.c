// Copyright Bianca Ciuche 2018

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define NMAX 1000

typedef struct {
    int x;      // Line
    int y;      // Column
    int A;      // Cell altitude
    int gloves; // Gloves's size
} cells;

typedef struct {
    int R;      // Circle's radius
    int P;      // Number of players
    int center; // Fixed position of the cener
    cells *matrix;  // Contains properties of
                        // every cell from the matrix
} glacier;

typedef struct {
    char name[NMAX];
    int id;
    int x;      // poz linie
    int y;      // poz col
    int hp;     // gradul de umezeala
    int stamina; // energie
    int dmg;    // dim manusi
    int win;    // winning times
}elf;
