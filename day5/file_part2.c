#include "../include/helpers.h"

const size_t crateSize = 60;

struct CrateStack {
    size_t height;
    size_t start_idx;
    char crates[60]; // = crateSize
};

// Moves MULTIPLE crates around like a BOSS!
void move(struct CrateStack ***crateArrayPtr, int amount, int from, int to) {
    size_t srcIdx = (*crateArrayPtr)[from]->start_idx;
    size_t destIdx = (*crateArrayPtr)[to]->start_idx;
    size_t srcHeight = (*crateArrayPtr)[from]->height;
    size_t destHeight = (*crateArrayPtr)[to]->height;
    char  *carries = malloc(amount * sizeof *carries); // *NOPAD*

    // pop the carries
    for (size_t i = srcHeight + srcIdx; i > srcHeight + srcIdx - amount; i--) {
        carries[(crateSize - 1) - i] = (*crateArrayPtr)[from]->crates[i];
    }

    // shift back the dest by amount
    for (size_t i = 0; i < (size_t)amount; i++) {
        for (size_t j = destIdx; j < destIdx + destHeight; j++) {
            (*crateArrayPtr)[to]->crates[j] = (*crateArrayPtr)[to]->crates[j + 1];
        }

        destIdx--;
        destHeight++;
    }

    // push the carries
    for (size_t i = amount; i > 0; i--) {
        (*crateArrayPtr)[to]->crates[(destHeight + destIdx - i + 1)] = carries[i - 1];
    }

    // shang the source forward by amount
    for (size_t i = 0; i < (size_t)amount; i++) {
        for (size_t j = srcIdx + srcHeight; j > srcIdx; j--) {
            (*crateArrayPtr)[from]->crates[j] = (*crateArrayPtr)[from]->crates[j - 1];
        }

        srcIdx++;
        srcHeight--;
    }

    // pointers get new values
    (*crateArrayPtr)[from]->start_idx = srcIdx;
    (*crateArrayPtr)[to]->start_idx = destIdx;
    (*crateArrayPtr)[from]->height = srcHeight;
    (*crateArrayPtr)[to]->height = destHeight;
    free(carries); // you've done well my child :3
}

int main(void) {
    const char         *filename = "input.txt";
    FILE               *fptr = fopen(filename, "r");
    size_t              bufsize = 40;
    const size_t        crateArraySize = 9;
    char               *buffer;
    ssize_t             lineSize;
    struct CrateStack **crateArray;
    int                 isSecondPhase = 0;
    int                *tmp;

    if (!fptr) {
        puts("File does not exist.");
        return EXIT_FAILURE;
    }

    if (!(buffer = malloc(bufsize * sizeof *buffer))) { // *NOPAD*
        fclose(fptr);
        puts("Memory allocation failed.");
        return EXIT_FAILURE;
    }

    if (!(crateArray = malloc(crateArraySize * sizeof *crateArray))) { // *NOPAD*
        free(buffer);
        fclose(fptr);
        puts("Memory allocation failed.");
        return EXIT_FAILURE;
    }

    // create crate stacks
    for (size_t i = 0; i < 9; i++) {
        struct CrateStack *cptr = malloc(sizeof *cptr); // *NOPAD*

        if (!cptr) {
            free(buffer);
            free(crateArray);
            fclose(fptr);
            puts("Memory allocation failed.");
            return EXIT_FAILURE;
        }

        // hunky dory~
        cptr->height = 0;
        cptr->start_idx = crateSize - 1;
        crateArray[i] = cptr;
    }

    while ((lineSize = getline(&buffer, &bufsize, fptr)) >= 0) {
        /* Since this input is a bit more special than usual,
           we need to have a bit of foresight on which columns have what crates.
           Fortunately, each crate's letter aligns perfectly with the column
           number and is regularly spaced, so we can just check
           for offsets from the first column.
        */
        if (!isSecondPhase) {
            if (lineSize == 1) { // only '\n'
                isSecondPhase = 1;
                continue;
            }

            // build crate stacks
            for (ssize_t i = 1; i < lineSize; i += 4) {
                if ('A' <= buffer[i] && buffer[i] <= 'Z') {
                    crateArray[(i - 1) / 4]->crates[crateArray[(i - 1) / 4]->start_idx--] = buffer[i]; // brain
                    crateArray[(i - 1) / 4]->height++;
                }
            }
        }

        else {
            // Now let's get moving!
            tmp = atouarr(buffer, (size_t)lineSize, 3);

            if (tmp == NULL) {
                // well shit
                continue;
            }

            move(&crateArray, tmp[0], tmp[1] - 1, tmp[2] - 1);
            free(tmp);
        }
    }

    // for the print, the further left, the closer to the top
    puts("the further left, the closer to the top of the stack");

    for (size_t i = 0; i < 9; i++) {
        printf("crate stack %lu:", i + 1);

        for (size_t j = crateArray[i]->height + crateArray[i]->start_idx; j > crateArray[i]->start_idx; j--) {
            printf(" [%c] ", crateArray[i]->crates[j]);
        }

        puts("");
    }

    printf("The crates on top are: ");

    for (size_t i = 0; i < 9; i++) {
        printf("%c", crateArray[i]->crates[crateArray[i]->start_idx + crateArray[i]->height]);
    }

    puts("");
    free(crateArray);
    free(buffer);
    fclose(fptr);
}