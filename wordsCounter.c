#include <stdio.h>
#include <ctype.h>

int count_words_in_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    int count = 0;
    int in_word = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        if (isspace(ch)) {
            if (in_word) {
                in_word = 0;
                count++;
            }
        } else {
            in_word = 1;
        }
    }

    if (in_word) { // If the file ends with a word
        count++;
    }

    fclose(file);
    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int word_count = count_words_in_file(argv[1]);
    if (word_count >= 0) {
        printf("Number of words: %d\n", word_count);
    }

    return 0;
}
