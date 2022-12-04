#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

int main(void) {
	const char *filename = "input.txt";
	FILE       *fptr = fopen(filename, "r");
	char       *buffer;
	size_t      bufsize = 8;
	ssize_t     lineSize;
	u_int32_t   sum = 0;
	u_int32_t   max = 0;

	if (!fptr) {
		// GET THE FUCK OUT AAAAA
		puts("There is no such file.");
		fclose(fptr);
		return EXIT_FAILURE;
	}

	buffer = malloc(bufsize * sizeof *buffer);

	if (!buffer) {
		fclose(fptr);
		puts("Memory allocation for buffer failed.");
		return EXIT_FAILURE;
	}

	while((lineSize = getline(&buffer, &bufsize, fptr)) >= 0) {
		if (lineSize == 2) {
			// the current sum is everything we need for one elf
			max = (sum > max) ? sum : max;
			sum = 0;
		}

		else {
			// numbies yaaayyy
			sum += atoi(buffer);
		}

	}

	printf("The maximum amount is %d\n", max);
	free(buffer);
    return 0;
}
