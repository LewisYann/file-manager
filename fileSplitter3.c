#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void split_file_by_parts(const char *filename, int n) {


    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    long part_size = file_size / n;
    long remainder = file_size % n;

    for (int i = 0; i < n; ++i) {
        char part_filename[256];
        snprintf(part_filename, sizeof(part_filename), "out/part_%d.txt", i + 1);

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

void split_file_by_size(const char *filename, long size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char buffer[1024];
    int part_num = 1;

    while (!feof(file)) {
        char part_filename[256];
        snprintf(part_filename, sizeof(part_filename), "out/part_%d.txt", part_num++);

        FILE *part_file = fopen(part_filename, "w");
        if (!part_file) {
            perror("Error creating part file");
            fclose(file);
            return;
        }

        long bytes_written = 0;
        while (bytes_written < size && !feof(file)) {
            size_t bytes_to_read = sizeof(buffer);
            if (bytes_written + bytes_to_read > size) {
                bytes_to_read = size - bytes_written;
            }

            size_t bytes_read = fread(buffer, 1, bytes_to_read, file);
            fwrite(buffer, 1, bytes_read, part_file);
            bytes_written += bytes_read;
        }

        fclose(part_file);
    }

    fclose(file);

    printf("File split into parts of %ld bytes each.\n", size);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <filename> -n <number_of_parts> | -size <size_in_bytes>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[2], "-n") == 0) {
        int n = atoi(argv[3]);
        if (n <= 0) {
            fprintf(stderr, "Invalid number of parts: %d\n", n);
            return 1;
        }
        split_file_by_parts(argv[1], n);
    } else if (strcmp(argv[2], "-size") == 0) {
        long size = atol(argv[3]);
        if (size <= 0) {
            fprintf(stderr, "Invalid size: %ld\n", size);
            return 1;
        }
        split_file_by_size(argv[1], size);
    } else {
        fprintf(stderr, "Unknown option: %s\n", argv[2]);
        return 1;
    }

    return 0;
}
