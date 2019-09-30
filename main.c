// Copyright Bianca Ciuche 2018

#include "./functions.h"

int main(int argc, char *argv[]) {
    (void) argc;
    // unused parameter

    char input_file[NMAX], output_file[NMAX];
    filenames(argv[0], input_file, output_file, sizeof(input_file),
                 sizeof(output_file));

    FILE *in = fopen(input_file, "rt");
    FILE *out = fopen(output_file, "wt");

    glacier area;
    // area - glacier's name
    area = read_glacier(in);

    elf *helper = malloc((area.P + 1) * sizeof(elf));
    // *helper - memorises the elfs
    read_elfs(helper, area, in, out);

    area = commands(helper, area, in, out);

    fclose(in);
    fclose(out);
    free(area.matrix);
    free(helper);
    return 0;
}
