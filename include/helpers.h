// Some universally useful functions, includes, and structures that may or may not appear throughout this AoC run.

#include <stdio.h>
#include <stdlib.h>

// *New and improved!
// Returns a pointer to an array of size return_size that contains UPTO return_size integers from a string.
// If there are less integers found in the string than return_size, 0 will replace remaining indices.
// NOTE: Returns NULL if any integer fails to convert.
int *atoiarr(const char *str, size_t bufsize, size_t return_size) {
    int   *arr = (int *)calloc(return_size, sizeof *arr); // *NOPAD*
    char   buf[11] = "@@@@@@@@@@";
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

    while (i <= bufsize && arrIdx != return_size) {
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