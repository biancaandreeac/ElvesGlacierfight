// Copyright Bianca Ciuche 2018

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "./struct.c"

// Function that creates filenames
void filenames(char *name, char *input_file, char *output_file,
                size_t input_lenght, size_t output_lenght);

// Basic functions
void swapp(int *a, int *b);
void swap_names(char *name1, char *name2, size_t s1, size_t s2);
bool is_in_circle(int x, int y, int R, int center_x, int center_y);
int find_pos(int x, int y, glacier area);
int movement(int x1, int x2, int y1, int y2, glacier area);
glacier gloves(int x, int y, int *dmg, glacier area);
void attack(elf helper[], int P, int n, FILE *out);

// Print_scoreboard functions
void copy_struct(elf helper[], elf helper_c[], int P);
void sortt(elf helper[], int n);

// Move functions
glacier move_up(int arg, glacier area, elf helper[],
                        FILE *out);
glacier move_down(int arg, glacier area, elf helper[],
                        FILE *out);
glacier move_right(int arg, glacier area, elf helper[],
                        FILE *out);
glacier move_left(int arg, glacier area, elf helper[],
                        FILE *out);

// Reading functions
glacier read_glacier(FILE *in);
void read_elfs(elf helper[], glacier area, FILE *in, FILE *out);

// Command functions
glacier move(glacier area, elf helper[], FILE *in, FILE *out);
glacier meltdown(elf helper[], glacier area, FILE *in, FILE *out);
void snowstorm(elf helper[], glacier area, FILE *in, FILE *out);
void print_scoreboard(elf helper[], int P, FILE *out);
bool winner(elf helper[], int P, FILE *out);
glacier commands(elf helper[], glacier area, FILE *in,
                       FILE *out);

#endif
