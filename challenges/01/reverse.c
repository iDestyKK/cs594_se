/*
 * DNA Sequence Reverse
 *
 * Description:
 *     Reverses lines of text read in via stdin... honestly it doesn't even
 *     have to be a DNA sequence.
 *
 * Author:
 *     Clara Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *line = NULL;
	size_t  len, i, sz;
	ssize_t nread;

	//Read in lines and reverse 
	while (nread = getline(&line, &len, stdin) != -1) {
		sz = strlen(line);

		for (i = 0; i < sz; i++) {
			if (line[sz - i - 1] == '\n')
				continue;

			printf("%c", line[sz - i - 1]);
		}

		printf("\n");

		//Clean up
		free(line);
		line = NULL;
	}

	return 0;
}
