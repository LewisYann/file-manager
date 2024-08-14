#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void replace_word_in_file(const char *filename, const char *old_word, const char *new_word)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    // Temp file to store the modified content
    FILE *temp_file = tmpfile();
    if (!temp_file)
    {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file))
    {
        char *pos = buffer;
        while ((pos = strstr(pos, old_word)) != NULL)
        {
            // Write part of the buffer before the old word
            fwrite(buffer, 1, pos - buffer, temp_file);

            // Write the new word
            fwrite(new_word, 1, strlen(new_word), temp_file);

            // Move the position pointer to the end of the old word
            pos += strlen(old_word);

            // Set the buffer pointer to the new position
            memmove(buffer, pos, strlen(pos) + 1);
            pos = buffer;
        }
        // Write the rest of the buffer
        fputs(buffer, temp_file);
    }

    fclose(file);

    // Reopen the original file in write mode
    file = fopen(filename, "w");
    if (!file)
    {
        perror("Error opening file for writing");
        fclose(temp_file);
        return;
    }

    // Rewind the temporary file and copy its content to the original file
    rewind(temp_file);
    while (fgets(buffer, sizeof(buffer), temp_file))
    {
        fputs(buffer, file);
    }

    fclose(temp_file);
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <filename> <old_word> <new_word>\n", argv[0]);
        return 1;
    }

    replace_word_in_file(argv[1], argv[2], argv[3]);

    printf("All occurrences of the word '%s' have been replaced with '%s' in the file '%s'.\n", argv[2], argv[3], argv[1]);

    return 0;
}
