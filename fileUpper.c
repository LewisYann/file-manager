#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void to_uppercase(FILE *input, FILE *output) {
    int ch;
    while ((ch = fgetc(input)) != EOF) {
        fputc(toupper(ch), output);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        perror("Error creating output file");
        fclose(input_file);
        return 1;
    }

    to_uppercase(input_file, output_file);

    fclose(input_file);
    fclose(output_file);

    printf("Transformation en majuscules terminée. Résultat dans 'output.txt'.\n");

    return 0;
}
