#include <stdio.h>
#include <string.h>
#include <ctype.h>

void str_to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int count_word_occurrences(const char *filename, const char *word) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    int count = 0;
    char buffer[1024];
    char lower_word[1024];
    strncpy(lower_word, word, sizeof(lower_word));
    str_to_lower(lower_word);

    while (fscanf(file, "%1023s", buffer) == 1) {
        str_to_lower(buffer);
        if (strcmp(buffer, lower_word) == 0) {
            count++;
        }
    }

    fclose(file);
    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <word>\n", argv[0]);
        return 1;
    }

    int occurrences = count_word_occurrences(argv[1], argv[2]);
    if (occurrences >= 0) {
        printf("Number of occurrences of the word '%s': %d\n", argv[2], occurrences);
    }

    return 0;
}
