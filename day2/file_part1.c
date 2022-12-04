// Rock Paper Scissors
// A, X is Rock
// B, Y is Paper
// C, Z is Scissors

#include <stdio.h>
#include <stdlib.h>

u_int8_t calculateRound(char *opponent, char *user) {
    u_int8_t actionOffset;
    u_int8_t actionComparison;
    size_t   stateOffset;
    char     states[3] = {'T', 'L', 'W'};
    // first, calculate action offset
    actionOffset = *user - 'W';

    // find offset relative to ABC (rock, paper, scissors)
    switch (*user) {
        case 'X':
            stateOffset = 0;
            break;

        case 'Y':
            stateOffset = 2;
            break;

        case 'Z':
            stateOffset = 1;
            break;
    }

    for (size_t i = 0; i < 3; i++) {
        // necessary for offset calcs
        if (*opponent == (char)('A' + i)) {
            // assign score
            switch (states[(stateOffset + i) % 3]) {
                case 'T':
                    actionComparison = 3;
                    break;

                case 'W':
                    actionComparison = 6;
                    break;

                case 'L':
                    actionComparison = 0;
                    break;
            }
        }
    }

    return actionComparison + actionOffset;
}


int main(void) {
    const char *filename = "input.txt";
    FILE       *fptr = fopen(filename, "r");
    char       *buffer;
    size_t      bufsize = 4;
    ssize_t     lineSize;
    u_int32_t   totalScore = 0;

    if (!fptr) {
        puts("File not found.");
        return EXIT_FAILURE;
    }

    if (!(buffer = (char *)malloc(bufsize * sizeof(char)))) {
        puts("Allocation failed.");
        return EXIT_FAILURE;
    }

    while ((lineSize = getline(&buffer, &bufsize, fptr)) >= 0) {
        // buffer[0] is opponent, buffer[2] is you
        totalScore += (u_int32_t)calculateRound(&buffer[0], &buffer[2]);
    }

    printf("The total score is: %d\n", totalScore);
    free(buffer);
    fclose(fptr);
    return 0;
}