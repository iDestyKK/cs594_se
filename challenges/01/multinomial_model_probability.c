/*
 * Multinomial Model Probability Computation
 *
 * Description:
 *     Takes DNA sequences and computes the multinomial model probabilities of
 *     them. Each sequence belongs on its own line;
 *
 * Author:
 *     Clara Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *line = NULL;
	size_t  len, sz, i, j, z;
	ssize_t nread;

	const char opt[4] = { 'A', 'G', 'C', 'T' };
	int        occ[4];

	//Default values
	occ[0] = occ[1] = occ[2] = occ[3] = 0;
	z = 0;

	//Read in lines and reverse
	while (nread = getline(&line, &len, stdin) != -1) {
		sz = strlen(line);

		//Erase newline if possible
		if (line[sz - 1] == '\n')
			line[sz-- - 1] = 0;

		for (i = 0; i < sz; i++) {
			for (j = 0; j < 4; j++)
				if (line[i] == opt[j])
					break;

			if (j == 4)
				continue;

			occ[j]++;
		}

		//Newline if not the first DNA read in
		if (z++ > 0)
			printf("\n");

		//Print out sequence and probabilities
		printf("Sequence: %s\n", line);

		for (j = 0; j < 4; j++) {
			printf(
				"  P(%c) = %d/%d (%Lg)\n",
				opt[j], occ[j], sz,
				(long double) occ[j] / sz
			);
		}

		//Clean up
		free(line);
		line = NULL;
	}

	if (line)
		free(line);

	return 0;
}
