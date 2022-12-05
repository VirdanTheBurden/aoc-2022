#include <stdio.h>
#include <stdlib.h>

const size_t crateSize = 60;

struct CrateStack {
    size_t height;
    size_t start_idx;
    char crates[60]; // = crateSize
};

// Convert a string to an array of integers of size return_size.
// NOTE: The pointer returned is dynamically allocated. After use, call free() on the reference.
// *New and improved!
int *atoiarr(const char *str, size_t bufsize, size_t return_size) {
    int   *arr = malloc(return_size * sizeof *arr); // *NOPAD*
    char   buf[10] = "@@@@@@@@@@";
    size_t bufIdx = 0;
    size_t arrIdx = 0;
    int    isZero = 0;
    int    isFalsePositive;
    int    tmp;

    if (!arr) {
        puts("Memory allocation for buffer failed.");
        return NULL;
    }

    size_t i = 0;

    while (i <= bufsize) {
        if ('0' <= str[i] && str[i] <= '9') {
            buf[bufIdx++] = str[i];
        }

        else {
            isFalsePositive = 1;

            // false positive check
            for (size_t l = 0; l < 11; l++) {
                if ('0' <= buf[l] && buf[l] <= '9') {
                    isFalsePositive = 0;
                }
            }

            if (!isFalsePositive) {
                // check to make sure 0 isn't in buffer
                for (size_t j = 0; j < bufIdx; j++) {
                    if (buf[j] == '0') {
                        // ok, then it's ACTUALLY 0
                        isZero = 1;
                    }
                }

                // now convert
                tmp = atoi(buf);

                if (!isZero && tmp == 0) {
                    // freak the fuck out
                    free(arr);
                    return NULL;
                }

                // ok, we gucci mane
                arr[arrIdx++] = tmp;
                bufIdx = 0;

                // let's clean out the buffer
                for (size_t k = 0; k < 11; k++) {
                    buf[k] = '@';
                }

                isZero = 0;
            }
        }

        i++;
    }

    return arr;
}

// Moves crates around like a BOSS!
void move(struct CrateStack ***crateArrayPtr, int amount, int from, int to) {
    size_t             srcIdx = (*crateArrayPtr)[from]->start_idx;
    size_t             destIdx = (*crateArrayPtr)[to]->start_idx;
    size_t             srcHeight = (*crateArrayPtr)[from]->height;
    size_t             destHeight = (*crateArrayPtr)[to]->height;
    char               carry;

    for (size_t toMove = amount; toMove > 0; toMove--) {
        // pop the carry
        carry = (*crateArrayPtr)[from]->crates[srcHeight + srcIdx];

        // shift back the dest by 1
        for (size_t i = destIdx; i < destIdx + destHeight; i++) {
            (*crateArrayPtr)[to]->crates[i] = (*crateArrayPtr)[to]->crates[i + 1];
        }

        // push the carry
        (*crateArrayPtr)[to]->crates[destHeight + destIdx] = carry;

        // shang the source forward by 1
        for (size_t i = srcIdx + srcHeight; i > srcIdx; i--) {
            (*crateArrayPtr)[from]->crates[i] = (*crateArrayPtr)[from]->crates[i - 1];
        }

        // update vars
        srcIdx++;
        srcHeight--;
        destIdx--;
        destHeight++;
    }

    // pointers get new values
    (*crateArrayPtr)[from]->start_idx = srcIdx;
    (*crateArrayPtr)[to]->start_idx = destIdx;
    (*crateArrayPtr)[from]->height = srcHeight;
    (*crateArrayPtr)[to]->height = destHeight;
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
            tmp = atoiarr(buffer, (size_t)lineSize, 3);

            if (tmp == NULL) {
                // well shit
                continue;
            }

            move(&crateArray, tmp[0], tmp[1] - 1, tmp[2] - 1);
            free(tmp);
        }
    }

    // for the print, the further left, the closer to the top
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