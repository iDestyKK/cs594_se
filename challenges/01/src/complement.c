/*
 * DNA Sequence Complement Generator
 *
 * Description:
 *     Takes DNA sequences and prints out the complement of them. Any other
 *     character is printed out without conversion.
 *
 * Author:
 *     Clara Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	const char opt[4] = { 'A', 'G', 'C', 'T' };
	const char rev[4] = { 'T', 'C', 'G', 'A' };

	int ci, i;

	while (ci = getchar()) {
		if (ci == EOF)
			break;

		for (i = 0; i < 4; i++) {
			if (ci == opt[i]) {
				putchar(rev[i]);
				break;
			}
		}

		if (i == 4)
			putchar(ci);
	}
}
