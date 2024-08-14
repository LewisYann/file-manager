#include <stdio.h>
#include <stdlib.h>

void split_file_by_parts(const char *filename, int n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Calculer la taille du fichier
    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    // Calculer la taille de chaque partie
    long part_size = file_size / n;
    long remainder = file_size % n;

    for (int i = 0; i < n; ++i) {
        char part_filename[256];
        snprintf(part_filename, sizeof(part_filename), "part_%d.txt", i + 1);

        FILE *part_file = fopen(part_filename, "w");
        if (!part_file) {
            perror("Error creating part file");
            fclose(file);
            return;
        }

        long current_part_size = part_size + (i < remainder ? 1 : 0);
        for (long j = 0; j < current_part_size; ++j) {
            int ch = fgetc(file);
            if (ch == EOF) break;
            fputc(ch, part_file);
        }

        fclose(part_file);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <number_of_parts>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[2]);
    if (n <= 0) {
        fprintf(stderr, "Invalid number of parts: %d\n", n);
        return 1;
    }

    split_file_by_parts(argv[1], n);

    printf("File split into %d parts.\n", n);

    return 0;
}
