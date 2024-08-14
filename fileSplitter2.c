#include <stdio.h>
#include <stdlib.h>

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
        snprintf(part_filename, sizeof(part_filename), "part_%d.txt", part_num++);

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
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <size_in_bytes>\n", argv[0]);
        return 1;
    }

    long size = atol(argv[2]);
    if (size <= 0) {
        fprintf(stderr, "Invalid size: %ld\n", size);
        return 1;
    }

    split_file_by_size(argv[1], size);

    return 0;
}
