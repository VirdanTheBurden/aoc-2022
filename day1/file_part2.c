#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

struct Elf {
	u_int32_t calories;
};

int main(void) {
	const char *filename = "input.txt";
	FILE       *fptr = fopen(filename, "r");
	char       *inStr;
	struct Elf *allCalories;
	struct stat sb;
	u_int32_t   sum = 0;
	u_int32_t   max = 0;
	size_t      idx = 0;
	struct Elf  tmp;

	if (!fptr) {
		// GET THE FUCK OUT AAAAA
		puts("There is no such file.");
		fclose(fptr);
		return EXIT_FAILURE;
	}

	if (stat(filename, &sb) == -1) {
		// we dun fucked up
		puts("Information about this file could not be found.");
		return EXIT_FAILURE;
	}

	inStr = malloc(sb.st_size);
	allCalories = malloc(sb.st_size * sizeof(struct Elf));

	while(!feof(fptr)) {
		// grabby strings >.<
		fgets(inStr, sb.st_size, fptr);

		if (isspace(inStr[0])) {
			// the current sum is everything we need for one elf
			struct Elf e;

			e.calories = sum;
			sum = 0;

			max = (e.calories > max) ? e.calories : max;

			// add to the elf array
			allCalories[idx] = e;
			idx++;
		}

		else {
			// numbies yaaayyy
			sum += atoi(inStr);
		}

	}

	// Ah, *now* there's a reason for this array!
	// iterates the array elements
    for (int64_t i = 0; i < sb.st_size; i++) {
        
        // iterates the array elements from index 1
        for (int64_t j = i + 1; j < sb.st_size; j++) {
            
            // comparing the array elements, to set array
            // elements in descending order
            if (allCalories[i].calories < allCalories[j].calories) {
                tmp = allCalories[i];
                allCalories[i] = allCalories[j];
                allCalories[j] = tmp;
            }
        }
    }

	sum = 0;

	printf("The top three elves have calorie totals of:\n");
	for (size_t k = 0; k < 3; k++) {
		printf("%d\n", allCalories[k].calories);
		sum += allCalories[k].calories;
	}
	printf("Their sum is: %d", sum);
	free(allCalories);

    return 0;
}
