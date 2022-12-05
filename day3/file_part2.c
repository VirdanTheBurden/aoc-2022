#include "../include/helpers.h"

int contains(const char *str, int c, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (str[i] == c) {
            return 1;
        }
    }

    return 0;
}

char getPriority(const char *buf1, const char *buf2, const char *buf3) {
    char   possibleMatches[27]; // in case ALL letters get in here somehow
    size_t idx = 0;

    // first, find matches in first and second buffer
    for (size_t i = 0; i < strlen(buf1); i++) {
        for (size_t j = 0; j < strlen(buf2); j++) {
            if (buf1[i] == buf2[j] && contains(possibleMatches, buf1[i], idx) == 0) {
                possibleMatches[idx++] = buf1[i];
            }
        }
    }

    // now check third buffer
    for (size_t i = 0; i < idx - 1; i++) {
        for (size_t j = 0; j < strlen(buf3); j++) {
            if (possibleMatches[i] == buf3[j]) {
                return buf3[j];
            }
        }
    }

    return '@'; // somehow ya got here...
}

int main(void) {
    const char *filename = "input.txt";
    FILE       *fptr = fopen(filename, "r");
    size_t      bufsize = 50;
    ssize_t     lineSize;
    char       *buffer1;
    char       *buffer2;
    char       *buffer3;
    char        similar;
    u_int8_t    priority;
    u_int32_t   totalPriority = 0;

    if (!fptr) {
        fclose(fptr);
        puts("File cannot be found.\n");
        return EXIT_FAILURE;
    }

    if (!(buffer1 = malloc(bufsize * sizeof *buffer1))) { // *NOPAD*
        fclose(fptr);
        puts("Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    if (!(buffer2 = malloc(bufsize * sizeof *buffer2))) { // *NOPAD*
        free(buffer1);
        fclose(fptr);
        puts("Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    if (!(buffer3 = malloc(bufsize * sizeof *buffer3))) { // *NOPAD*
        free(buffer1);
        free(buffer2);
        fclose(fptr);
        puts("Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    while (1) {
        lineSize = getline(&buffer1, &bufsize, fptr);
        lineSize = getline(&buffer2, &bufsize, fptr);
        lineSize = getline(&buffer3, &bufsize, fptr);

        if (!(lineSize >= 0)) {
            break; // cave johnson, we're done here
        }

        similar = getPriority(buffer1, buffer2, buffer3);
        printf("match: %c\n", similar);

        // find priority
        if ('a' <= similar && similar <= 'z') { // lower cases
            priority = similar - 96;
        }

        else { // must be uppers
            priority = similar - 38;
        }

        totalPriority += (u_int32_t)priority;
    }

    printf("The total priority is: %d\n", totalPriority);
    free(buffer1);
    free(buffer2);
    free(buffer3);
    fclose(fptr);
    return 0;
}