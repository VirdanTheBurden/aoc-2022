#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Elf {
	u_int32_t calories;
};

int main(void) {
	const char  *filename = "input.txt";
	FILE        *fptr = fopen(filename, "r");
	char        *buffer;
	struct Elf **allCalories;
	size_t       bufsize = 8;
	size_t       calorieSize = 400;
	ssize_t      lineSize;
	u_int32_t    sum = 0;
	size_t       idx = 0;
	struct Elf  *tmp;


	if (!fptr) {
		puts("There is no such file.");
		return EXIT_FAILURE;
	}

	if (!(buffer = malloc(bufsize * sizeof *buffer))) {
		fclose(fptr);
		puts("Memory allocation failed.");
		return EXIT_FAILURE;

	}

	if (!(allCalories = malloc(calorieSize * sizeof *allCalories))) {
		free(buffer);
		fclose(fptr);
		puts("Memory allocation failed.");
		return EXIT_FAILURE;
	}

	while ((lineSize = getline(&buffer, &bufsize, fptr)) >= 0) {
		if (lineSize == 2) {
			// the current sum is everything we need for one elf
			struct Elf *e = malloc(sizeof *e);

			if (!e) {
				free(buffer);
				fclose(fptr);
				puts("Memory allocation failed.");
				return EXIT_FAILURE;
			}

			e->calories = sum;
			sum = 0;

			allCalories[idx++] = e;
		}

		else {
			// numbies yaaayyy
			sum += atoi(buffer);
		}
	}

	// sort ascending
	for (size_t i = 0; i < idx; ++i){
   		for (size_t j = i + 1; j < idx; ++j){
      		if (allCalories[i]->calories < allCalories[j]->calories){
         		tmp = allCalories[i];
         		allCalories[i] = allCalories[j];
         		allCalories[j] = tmp;
      		}
   		}
	}

	printf("The elves with the most calories have: %u, %u, and %u each.\n", allCalories[0]->calories, allCalories[1]->calories, allCalories[2]->calories);
	printf("Their sum is: %u.\n", allCalories[0]->calories + allCalories[1]->calories + allCalories[2]->calories);

	free(allCalories);
	free(buffer);

	fclose(fptr);

    return 0;
}
