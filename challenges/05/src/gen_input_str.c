#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(int argc, char **argv) {
	size_t i, l;

	if (argc != 2) {
		fprintf(stderr, "usage: %s word\n", argv[0]);
		exit(1);
	}

	l = strlen(argv[1]);
	for (i = 0; i < l; i++)
		printf("%s\n", argv[1] + i);

	exit(0);
}
