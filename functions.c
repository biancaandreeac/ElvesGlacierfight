// Copyright Bianca Ciuche 2018

#include "./functions.h"

// filenames        - creates the filenames
void filenames(char *name, char *input_file, char *output_file,
                  size_t input_lenght, size_t output_lenght) {
    snprintf(input_file, input_lenght, "%s", name);
    snprintf(output_file, output_lenght, "%s", name);
    snprintf(&input_file[strlen(input_file)], sizeof(".in"), "%s", ".in");
    snprintf(&output_file[strlen(output_file)], sizeof(".out"), "%s", ".out");
}


// swapp             - mutually exchange the values of the variables
void swapp(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void swap_names(char *name1, char *name2, size_t s1, size_t s2) {
    char aux[NMAX];
    snprintf(aux, sizeof(aux), "%s", name1);
    snprintf(name1, s1, "%s", name2);
    snprintf(name2, s2, "%s", aux);
}

// is_in_circle     - checks if an elf is on the glacier;
// x, y             - the location's line and column
// R                - the radius of the glacier
// center_x, _y     - the coordonates of the glacier's center
bool is_in_circle(int x, int y, int R, int center_x, int center_y) {
    int sx = 0, sy = 0;
    sx = (x - center_x) * (x - center_x);
    sy = (y - center_y) * (y - center_y);
    if (sx + sy <= R * R) {
        return true;
    }
    if (R == 0) {
        if (x == center_x && y == center_y) {
            return true;
        }
    }
    return false;
}

// find_pos         - finds the position in array of a given location
// x                - helper[i].x
// y                - helper[i].y
int find_pos(int x, int y, glacier area) {
    int i, poz = 0;
    int len = 2 * area.center + 1;

    for (i = 0; i < len * len ; ++i) {
        if (x == area.matrix[i].x && y == area.matrix[i].y) {
            poz = i;
            break;
        }
    }

    return poz;
}

// movement         - det. the difference of altitude between 2 positions
// x1, y1           - initial position
// x2, y2           - final position
// @return          - diff between altitudes
int movement(int x1, int x2, int y1, int y2, glacier area) {
    int dif = 0, pos1, pos2;

    pos1 = find_pos(x1, y1, area);
    pos2 = find_pos(x2, y2, area);

    if (area.matrix[pos1].A > area.matrix[pos2].A) {
        dif = area.matrix[pos1].A - area.matrix[pos2].A;
    } else {
        dif = area.matrix[pos2].A - area.matrix[pos1].A;
    }

    return dif;
}

// copy_     - copies the useful elements
void copy_struct(elf helper[], elf helper_c[], int P) {
    for (int i = 0; i < P; ++i) {
        snprintf(helper_c[i].name, sizeof(helper_c[i].name), "%s",
            helper[i].name);
        helper_c[i].hp = helper[i].hp;
        helper_c[i].win = helper[i].win;
    }
}

void sortt(elf helper[], int n) {
    int i, j;
    for (i = 0; i < n - 1; ++i) {
        for (j = i + 1; j < n; ++j) {
            if (helper[i].hp <= 0 && helper[j].hp > 0) {
                swapp(&helper[i].hp, &helper[j].hp);
                swap_names(helper[i].name, helper[j].name,
                            sizeof(helper[i].name), sizeof(helper[j].name));
                swapp(&helper[i].win, &helper[j].win);
            } else if (helper[i].hp > 0 && helper[j].hp <= 0) {
                continue;
            } else if (helper[i].win < helper[j].win) {
                swapp(&helper[i].hp, &helper[j].hp);
                swap_names(helper[i].name, helper[j].name,
                            sizeof(helper[i].name), sizeof(helper[j].name));
                swapp(&helper[i].win, &helper[j].win);
            } else if (helper[i].win > helper[j].win) {
                continue;
            } else if (strcmp(helper[i].name, helper[j].name) > 0) {
                swapp(&helper[i].hp, &helper[j].hp);
                swap_names(helper[i].name, helper[j].name,
                            sizeof(helper[i].name), sizeof(helper[j].name));
                swapp(&helper[i].win, &helper[j].win);
            }
        }
    }
}

// attack           - checks if there are two elfs on the same position;
//                  if there are, they start playing; it stops when one of them
//                  gets soaking wet
// p                - players number
// n                - elf vector position
void attack(elf helper[], int p, int n, FILE *out) {
    int i, first, sec, id;
    id = helper[n].id;

    for (i = 0; i < p; ++i) {
        if (i == n || helper[i].hp == 0) {
            continue;
        }

        if (helper[i].x == helper[id].x && helper[i].y == helper[id].y) {
            // if they are on the same position

            // who is going to be first
            if (helper[i].stamina > helper[id].stamina) {
                first = i;
                sec = id;
            } else {
                first = id;
                sec = i;
            }

            while (helper[first].hp > 0 && helper[sec].hp > 0) {
                helper[sec].hp -= helper[first].dmg;
                if (helper[sec].hp <= 0) {
                    break;
                }
                helper[first].hp -= helper[sec].dmg;
            }

            if (helper[first].hp <= 0) {
                helper[sec].stamina += helper[first].stamina;
                helper[first].hp = 0;
                helper[sec].win++;
                fprintf(out, "%s sent %s back home.\n",
                        helper[sec].name, helper[first].name);
            } else {
                helper[first].stamina += helper[sec].stamina;
                helper[sec].hp = 0;
                helper[first].win++;
                fprintf(out, "%s sent %s back home.\n",
                        helper[first].name, helper[sec].name);
            }
            break;
        }
    }
}

// read_glacier     - reads the glacier characteristics
glacier read_glacier(FILE *in) {
    glacier area;
    int i, j, n = 0;

    fscanf(in, "%d %d", &area.R, &area.P);

    area.center = area.R;
    int len = 2 * area.R + 1;
    // len - the lenght of a line/column of the matrix
    area.matrix = malloc((len * len + 1) * sizeof(cells));

    for (i = 0; i < len; ++i) {
        for (j = 0; j < len; ++j) {
            area.matrix[n].x = i;
            area.matrix[n].y = j;
            fscanf(in, "%d%d", &area.matrix[n].A, &area.matrix[n].gloves);
            n++;
            // n - puts the altitude and size of gloves on each position
        }
    }

    return area;
}

// read_elfs        - reads the elfs and their characteristics
void read_elfs(elf helper[], glacier area, FILE *in, FILE *out) {
    int i, pos;

    for (i = 0; i < area.P; ++i) {
        fscanf(in, " %[^ \n]s", helper[i].name);
        fscanf(in, "%d %d", &helper[i].x, &helper[i].y);
        fscanf(in, "%d %d", &helper[i].hp, &helper[i].stamina);

        helper[i].win = 0;
        helper[i].id = i;

        if (is_in_circle(helper[i].x, helper[i].y, area.R, area.center,
                          area.center)) {
            pos = find_pos(helper[i].x, helper[i].y, area);
            helper[i].dmg = area.matrix[pos].gloves;
            area.matrix[pos].gloves = 0;
            // he gets the gloves from his cell
        } else {
            fprintf(out, "%s has missed the glacier.\n", helper[i].name);
            helper[i].hp = 0;
        }
    }
}

glacier gloves(int x, int y, int *dmg, glacier area) {
    int pos = find_pos(x, y, area);
    if (*dmg < area.matrix[pos].gloves) {
        swapp(&*dmg , &area.matrix[pos].gloves);
    }
    return area;
}

glacier move_up(int arg, glacier area, elf helper[],
                         FILE *out) {
    int x1 = helper[arg].x;
    int x2 = helper[arg].x - 1;
    int y = helper[arg].y;
    int dif = movement(x1, x2, y, y, area);
    if (dif > helper[arg].stamina) {
        attack(helper, area.P, arg, out);
        return area;
    } else {
        helper[arg].stamina -= dif;
    }
    if (!(is_in_circle(x2, y, area.R, area.center, area.center))) {
        fprintf(out, "%s fell off the glacier.\n", helper[arg].name);
        helper[arg].hp = 0;
        return area;
    }
    helper[arg].x--;
    area = gloves(helper[arg].x, helper[arg].y, &helper[arg].dmg, area);
    attack(helper, area.P, arg, out);
    return area;
}

glacier move_down(int arg, glacier area, elf helper[],
                         FILE *out) {
    int x1 = helper[arg].x;
    int x2 = helper[arg].x + 1;
    int y = helper[arg].y;
    int dif = movement(x1, x2, y, y, area);
    if (dif > helper[arg].stamina) {
        attack(helper, area.P, arg, out);
        return area;
    } else {
        helper[arg].stamina -= dif;
    }
    if (!(is_in_circle(x2, y, area.R, area.center, area.center))) {
        fprintf(out, "%s fell off the glacier.\n", helper[arg].name);
        helper[arg].hp = 0;
        return area;
    }
    helper[arg].x++;
    area = gloves(helper[arg].x, helper[arg].y, &helper[arg].dmg, area);
    attack(helper, area.P, arg, out);
    return area;
}

glacier move_right(int arg, glacier area, elf helper[],
                         FILE *out) {
    int x = helper[arg].x;
    int y1 = helper[arg].y;
    int y2 = helper[arg].y + 1;

    int dif = movement(x, x, y1, y2, area);
    if (dif > helper[arg].stamina) {
        attack(helper, area.P, arg, out);
        return area;
    } else {
        helper[arg].stamina -= dif;
    }
    if (!(is_in_circle(x, y2, area.R, area.center, area.center))) {
        fprintf(out, "%s fell off the glacier.\n", helper[arg].name);
        helper[arg].hp = 0;
        return area;
    }
    helper[arg].y++;
    area = gloves(helper[arg].x, helper[arg].y, &helper[arg].dmg, area);
    attack(helper, area.P, arg, out);
    return area;
}

glacier move_left(int arg, glacier area, elf helper[],
                         FILE *out) {
    int x = helper[arg].x;
    int y1 = helper[arg].y;
    int y2 = helper[arg].y - 1;

    int dif = movement(x, x, y1, y2, area);
    if (dif > helper[arg].stamina) {
        attack(helper, area.P, arg, out);
        return area;
    } else {
        helper[arg].stamina -= dif;
    }
    if (!(is_in_circle(x, y2, area.R, area.center, area.center))) {
        fprintf(out, "%s fell off the glacier.\n", helper[arg].name);
        helper[arg].hp = 0;
        return area;
    }
    helper[arg].y--;
    area = gloves(helper[arg].x, helper[arg].y, &helper[arg].dmg, area);
    attack(helper, area.P, arg, out);
    return area;
}

// move             - executes the command MOVE;
//                  MOVE [id] [UDLR+]
//                  with every letter (UDLR) that is being read, the function:
//                  * verifies if the elf remains on the glacier
//                  * modifies the stamina of the elf
//                  * gives the elf a chance to get bigger gloves
//                  * if he finds another elf on the way they will fight
glacier move(glacier area, elf helper[],
                     FILE *in, FILE *out) {
    char mv;
    // the movement to be done
    int arg;
    // arg is the argument received
    // dif is the difference between altitudes

    fscanf(in, "%d", &arg);

    while (fscanf(in, "%c", &mv)) {
        if (mv == '\n' || arg >= area.P) {
            break;
        }
        if (helper[arg].hp == 0) {
            break;
        }

        if (mv == 'U') {
            area = move_up(arg, area, helper, out);
        } else if (mv == 'D') {
            area = move_down(arg, area, helper, out);
        } else if (mv == 'L') {
            area = move_left(arg, area, helper, out);
        } else if (mv == 'R') {
            area = move_right(arg, area, helper, out);
        }
    }

    return area;
}

// meltdown         - executes the command MELTDOWN;
//                  MELTDOWN [STAMINA]
//                  * the radius of the glacier decreases with an unit
//                  * if an elf is on the edge of the glacier before the
//                  meltdown, it will fall in the water
//                  * the elfs that remain on the glacier will get stamina
glacier meltdown(elf helper[], glacier area, FILE *in,
                            FILE *out) {
    int stamin, i;
    fscanf(in, "%d", &stamin);
    area.R--;

    for (i = 0; i < area.P; ++i) {
        if (helper[i].hp == 0) {
            continue;
        }
        if (is_in_circle(helper[i].x, helper[i].y, area.R, area.center,
                          area.center)) {
            helper[i].stamina += stamin;
        } else {
            fprintf(out, "%s got wet because of global warming.\n",
                     helper[i].name);
            helper[i].hp = 0;
            helper[i].stamina = 0;
        }
    }
    return area;
}

// print_scoreboard - executes the command PRINT_SCOREBOARD
void print_scoreboard(elf helper[], int P, FILE *out) {
    fprintf(out, "SCOREBOARD:\n");
    elf helper_c[P + 1];
    copy_struct(helper, helper_c, P);
    sortt(helper_c, P);

    for (int i = 0; i < P; ++i) {
        fprintf(out, "%s\t", helper_c[i].name);

        if (helper_c[i].hp != 0) {
            fprintf(out, "DRY\t");
        } else {
            fprintf(out, "WET\t");
        }

        fprintf(out, "%d\n", helper_c[i].win);
    }
}

// snowstorm        - executes the command SNOWSTORM;
//                  SNOWSTORM [K]
void snowstorm(elf helper[], glacier area, FILE *in,
                FILE *out) {
    int k, mask = 255, v[4], i;
    // mask = 255 - 00000000 00000000 00000000 11111111
    //            it offers certitude that it will only take the last 8 bits
    // v[0] = dmg
    // v[1] = radius
    // v[2] = y
    // v[3] = x

    fscanf(in, "%d", &k);

    for (i = 3; i >= 0; --i) {
        v[i] = k & mask;
        k = k >> 8;
    }

    for (i = 0; i < area.P; ++i) {
        if (helper[i].hp == 0) {
            continue;
        }
        if (is_in_circle(helper[i].x, helper[i].y, v[1], v[3], v[2])) {
            helper[i].hp -= v[0];
                if (helper[i].hp <= 0) {
                    fprintf(out, "%s was hit by snowstorm.\n", helper[i].name);
                    helper[i].hp = 0;
                }
        }
    }
}

// winner           - prints the winner
// @return          - 1 if there is a winner
//                  0 if there isn't a winner
bool winner(elf helper[], int P, FILE *out) {
    int nr = 0, pos;

    for (int i = 0; i < P; ++i) {
        if (helper[i].hp != 0) {
            nr++;
            pos = i;
        }
    }

    if (nr == 1) {
        fprintf(out, "%s has won.\n", helper[pos].name);
        return true;
    }
    return false;
}

// command          - executes every command
glacier commands(elf helper[], glacier area, FILE *in,
                        FILE *out) {
    char cmd[NMAX];
    while (fscanf(in, " %[^ \n]s", cmd) == 1) {
        if (winner(helper, area.P, out)) {
            break;
        }
        if (strcmp(cmd, "MOVE") == 0) {
            area = move(area, helper, in, out);
        }

        if (strcmp(cmd, "MELTDOWN") == 0) {
            area = meltdown(helper, area, in, out);
        }

        if (strcmp(cmd, "PRINT_SCOREBOARD") == 0) {
            print_scoreboard(helper, area.P, out);
        }

        if (strcmp(cmd, "SNOWSTORM") == 0) {
            snowstorm(helper, area, in, out);
        }
    }
    return area;
}
