#include <stdio.h>

void count_lines_in_file(const char *filename, int *empty_lines, int *non_empty_lines) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    *empty_lines = 0;
    *non_empty_lines = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        int is_empty = 1;

        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] != '\n' && line[i] != '\r' && line[i] != ' ' && line[i] != '\t') {
                is_empty = 0;
                break;
            }
        }

        if (is_empty) {
            (*empty_lines)++;
        } else {
            (*non_empty_lines)++;
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int empty_lines = 0;
    int non_empty_lines = 0;

    count_lines_in_file(argv[1], &empty_lines, &non_empty_lines);

    printf("Number of empty lines: %d\n", empty_lines);
    printf("Number of non-empty lines: %d\n", non_empty_lines);

    return 0;
}
