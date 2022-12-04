// Rock Paper Scissors (Variant)
// A, B, C = Rock, Paper, Scissors
// X, Y, Z = Lose, Draw, Win

#include <stdio.h>
#include <stdlib.h>

char convert(char *toConvert) {
    switch (*toConvert) {
        case 'L':
            return 'X';

        case 'T':
            return 'Y';

        case 'W':
            return 'Z';
    }

    return 'A'; // if you somehow manage to give something that's not LTW
}

u_int8_t calculateRound(char *opponent, char *action) {
    u_int8_t actionOffset;
    u_int8_t actionComparison;
    size_t   stateOffset;
    char     states[3] = {'T', 'W', 'L'};

    // calculate result of move
    switch (*action) {
        case 'X':
            actionComparison = 0;
            break;

        case 'Y':
            actionComparison = 3;
            break;

        case 'Z':
            actionComparison = 6;
            break;
    }

    // get the proper offset
    switch (*opponent) {
        case 'A':
            stateOffset = 0;
            break;

        case 'B':
            stateOffset = 2;
            break;

        case 'C':
            stateOffset = 1;
            break;
    }

    // find associated letter
    for (size_t i = 0; i < 3; i++) {
        if (*action == (char)(convert(&states[(stateOffset + i) % 3]))) {
            // calculate action offset for letter
            actionOffset = ('A' + (char)i) - '@';
            break;
        }
    }

    // calculate reward for action
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

    if (!(buffer = malloc(bufsize * sizeof *buffer))) { // *NOPAD*
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