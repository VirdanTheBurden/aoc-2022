#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char *filename = "input.txt";
    FILE       *fptr = fopen(filename, "r");
    size_t      bufsize = 30;
    ssize_t     lineSize;
    char       *buffer;
    char        similar;
    u_int8_t    priority;
    u_int32_t   totalPriority = 0;

    if (!fptr) {
        puts("File cannot be found.\n");
        return EXIT_FAILURE;
    }

    if (!(buffer = (char *)malloc(sizeof(char) * bufsize))) {
        puts("Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    while ((lineSize = getline(&buffer, &bufsize, fptr)) >= 0) {
        strtok(buffer, "\n"); // because we hate newlines where I come from

        // compare both halves
        for (size_t i = 0; i < (size_t)(lineSize - 1) / 2; i++) {
            for (size_t j = (lineSize - 1) / 2; j < (size_t)lineSize; j++) {
                if (buffer[i] == buffer[j]) {
                    similar = buffer[i];
                }
            }
        }

        // find priority
        if ('a' <= similar && similar <= 'z') { // lower cases
            priority = similar - 96;
        }

        else { // must be uppers
            priority = similar - 38;
        }

        totalPriority += priority;
    }

    printf("The total priority is: %d\n", totalPriority);
    free(buffer);
    fclose(fptr);
    return 0;
}