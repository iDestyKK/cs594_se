/*
 * DNA Sequence Reverse Complement Generator
 *
 * Description:
 *     Takes DNA sequences and prints out the reverse complement of them. Any
 *     other character is printed out without conversion. The output is a file
 *     with the header ">reversed" with 70 characters per line, as implied by
 *     the input file "lambda.fasta". The output file will have UNIX-style line
 *     endings (\n rather than \r\n).
 *
 * Author:
 *     Clara Nguyen
 */

//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"

int main(int argc, char **argv) {
	char       *line, *str;
	FILE       *fp, *op;
	size_t      len, sz, i, j, pos;
	ssize_t     nread;
	CN_STRING   seq;
	const char  opt[4] = { 'A', 'G', 'C', 'T' };
	const char  rev[4] = { 'T', 'C', 'G', 'A' };

	//Argument check
	if (argc != 3) {
		fprintf(stderr, "usage: %s in_fasta out_fasta\n", argv[0]);
		return 1;
	}

	line = NULL;

	//Open file for reading
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "fatal: Failed to open \"%s\": ", argv[1]);
		perror("");
		return 2;
	}

	seq = cn_string_init();

	//Read in lines and reverse 
	while (nread = getline(&line, &len, fp) != -1) {
		sz = strlen(line);

		//If a comment, ignore it
		if (sz > 0 && line[0] == '>') {
			free(line);
			line = NULL;
			continue;
		}

		//Erase newline & carriage return if possible
		for (i = 0; i < 2; i++)
			if (line[sz - 1] == '\n' || line[sz - 1] == '\r')
				line[sz -- - 1] = 0;

		//Add to the sequence string
		cn_string_concat_from_cstr(seq, line);

		//Clean up
		free(line);
		line = NULL;
	}

	if (line)
		free(line);

	fclose(fp);

	//Open file for writing
	op = fopen(argv[2], "w");
	if (op == NULL) {
		fprintf(stderr, "fatal: Failed to open \"%s\": ", argv[2]);
		perror("");
		cn_string_free(seq);
		return 3;
	}

	//Default header
	fprintf(op, ">reversed\n");

	//Ok, go through every character in the string, complement, print.
	str = cn_string_str(seq);
	sz  = cn_string_len(seq);

	for (i = 0; i < sz; i++) {
		//Go backwards
		pos = sz - i - 1;

		//70 Characters per line (implied via input sample)
		if (i > 0 && i % 70 == 0)
			fputc('\n', op);

		//Print complement
		for (j = 0; j < 4; j++) {
			if (str[pos] == opt[j]) {
				fputc(rev[j], op);
				break;
			}
		}

		//Any other characters should print unaltered
		if (j == 4)
			fputc(str[pos], op);
	}

	fputc('\n', op);

	//Clean up
	cn_string_free(seq);
	fclose(op);

	return 0;
}
