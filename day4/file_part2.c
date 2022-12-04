#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Elf {
    int start;
    int end;
};

int *atoiarr(const char *str, size_t bufsize) {
    int   *arr = malloc(4 * sizeof *arr);
    char   buf[3] = {'a', 'a', '\0'};
    size_t bufIdx = 0;
    size_t arrIdx = 0;

    if (!arr) {
        puts("Memory allocation for buffer failed.");
        return NULL;
    }

    size_t i = 0;
    while (i < bufsize) {
        if (bufIdx == 2) {
            // try convert to int1

            arr[arrIdx++] = atoi(buf);
            bufIdx = 0;
        }

        else {

            buf[bufIdx++] = str[i];
            if (str[i] == '-' || str[i] == ',') {
                bufIdx = 2;
                continue;
            }
                    
        }
        i++;
    }

    return arr;

}

int main(void) {
    const char *filename = "input.txt";
    FILE       *fptr = fopen(filename, "r");
    size_t      bufsize = 8;
    char       *buffer;
    ssize_t     lineSize;
    u_int32_t   totalOverlaps;
    int        *tmp;

    if (!fptr) {
        puts("File cannot be found.");
        return EXIT_FAILURE;
    }

    if (!(buffer = malloc(bufsize * sizeof *buffer))) {
        fclose(fptr);
        puts("Memory allocation for string buffer failed.");
        return EXIT_FAILURE;
    }

    while ((lineSize = getline(&buffer, &bufsize, fptr)) >= 0) {
        tmp = atoiarr(buffer, bufsize);

        struct Elf e1;
        struct Elf e2;

        e1.start = tmp[0];
        e1.end = tmp[1];
        e2.start = tmp[2];
        e2.end = tmp[3];

        int eisContain = e2.start <= e1.end && e2.end >= e1.start;

        // check for containment
        if (eisContain) {
            totalOverlaps++;
        }

        free(tmp);

        
    }

    printf("The total amount of overlaps are: %u\n", totalOverlaps);

    free(buffer);
    fclose(fptr);

}