#include "../include/helpers.h"

int main(void) {
    const char *filename = "input.txt";
    FILE       *fptr = fopen(filename, "r");
    size_t      bufsize = 4;
    char       *buffer;
    char        c;
    u_int32_t   idx = 0;
    int         notFound;

    if (!fptr) {
        puts("File could not be opened. Exiitng.");
        return EXIT_FAILURE;
    }

    if (!(buffer = malloc(bufsize * sizeof *buffer))) { // *NOPAD*
        fclose(fptr);
        puts("Memory allocation failed. Exiitng.");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < bufsize; i++) {
        buffer[i] = fgetc(fptr);
        idx++;
    }

    while ((c = fgetc(fptr)) != EOF) {
        notFound = 0;

        // check uniqueness
        for (size_t i = 0; i < bufsize; i++) {
            for (size_t j = i + 1; j < bufsize; j++) {
                if (buffer[i] == buffer[j]) {
                    notFound = 1;
                    break;
                }
            }

            if (notFound) {
                break;
            }
        }

        if (!notFound) {
            break;
        }

        // open new spot for new character
        for (size_t i = 0; i < bufsize - 1; i++) {
            buffer[i] = buffer[i + 1];
        }

        buffer[bufsize - 1] = c;
        idx++;
    }

    printf("Start of packet found at position: %u\n", idx);
    free(buffer);
    fclose(fptr);
    return 0;
}