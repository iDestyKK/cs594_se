/*
 * DNA Sequence Generator
 *
 * Description:
 *     Generates a random DNA sequence of a specified length. A seed must be
 *     specified.
 *
 * Author:
 *     Clara Nguyen
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	size_t  len, seed, i;
	char   *str;

	const char opt[4] = {
		'A', 'G', 'C', 'T'
	};

	//Argument Check
	if (argc != 3) {
		fprintf(stderr, "usage: %s len seed\n", argv[0]);
		return 1;
	}

	//Extract the length
	len  = atoi(argv[1]);
	seed = atoi(argv[2]);

	//If no size, generate nothing.
	if (len == 0)
		return 0;

	str = (char *) calloc(len + 1, sizeof(char));

	//Generate the string
	srand(seed);

	for (i = 0; i < len; i++)
		str[i] = opt[rand() % 4];

	printf("%s\n", str);

	free(str);

	return 0;
}
