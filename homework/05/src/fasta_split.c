/*
 * FASTA Split Utility
 *
 * Description:
 *     Takes a FASTA file containing multiple sequences and splits them. This
 *     application assumes that they are separated by a blank line. You may
 *     specify a file prefix, which will be used for generating the split
 *     files.
 *
 * Author:
 *     Clara Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../lib/cnds/cn_vec.h"

// ----------------------------------------------------------------------------
// Helper Structs                                                          {{{1
// ----------------------------------------------------------------------------

// Struct for marking beginning and ending lines in file for each export file
typedef struct {
	size_t start, end;
} FRANGE;

// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

/*
 * file_to_cn_vec                                                          {{{2
 *
 * Attempts to read in all lines from a file at "path" into a CN_VEC "dest".
 * The CN_VEV must be initialised to type "char *" prior to calling this.
 */

int file_to_cn_vec(char *path, CN_VEC dest, int verbose) {
	FILE   *fp;
	char   *line, *dup;
	ssize_t nread;
	size_t  len, sz, i;

	//Open the fasta file.... if we can
	fp = fopen(path, "r");

	//Oh, it failed.
	if (fp == NULL)
		return 0;

	//Now then... read in all lines
	while (nread = getline(&line, &len, fp) != -1) {
		sz = strlen(line);

		//Erase newline & carriage return if possible
		for (i = 0; i < 2; i++)
			if (line[sz - 1] == '\n' || line[sz - 1] == '\r')
				line[sz -- - 1] = 0;

		//Ok, push to the CN_VEC
		dup = strdup(line);
		cn_vec_push_back(dest, &dup);

		//Clean up
		free(line);
		line = NULL;
	}

	if (line)
		free(line);

	if (verbose)
		printf("Lines in file: %d\n", cn_vec_size(dest));

	//We're done here.
	fclose(fp);
	return 1;
}

/*
 * split_count                                                             {{{2
 *
 * Given a CN_VEC<char *> of lines from a file (probably via "file_to_cn_vec"),
 * give the number of files that will be outputted.
 *
 * This is an expensive function. Use it only once, and store it into an int or
 * something.
 */

int split_count(CN_VEC lines, int verbose) {
	size_t lc, fc, i, j, sz;
	char *ptr;

	fc = 0;
	lc = cn_vec_size(lines);

	for (i = 0; i < lc; i++) {
		for (j = i; j < lc; j++) {
			ptr = cn_vec_get(lines, char *, j);
			sz = strlen(ptr);

			//We will detect via blank lines
			if (sz == 0)
				break;
		}

		//If there were like, 2 blank lines in a row... this doesn't count.
		if (i == j)
			continue;
		
		//Increment the file count and lines
		fc++;
		i = j;
	}

	if (verbose)
		printf("Splits detected: %d\n", fc);

	return fc;
}

/*
 * split_lines                                                             {{{2
 *
 * Given a CN_VEC<char *> of lines from a file (probably via "file_to_cn_vec"),
 * and a CN_VEC<FRANGE> after being resized to the number of splits, generate
 * the ranges of lines for each file.
 *
 * This is an expensive function. Use it only once, and store it into an int or
 * something.
 */

int split_lines(CN_VEC lines, CN_VEC files, int verbose) {
	size_t  lc, fc, i, j, sz;
	char   *ptr;
	FRANGE *fptr;

	fc = 0;
	lc = cn_vec_size(lines);

	if (verbose)
		printf("\nLine splits:\n", fc);

	for (i = 0; i < lc; i++) {
		for (j = i; j < lc; j++) {
			ptr = cn_vec_get(lines, char *, j);
			sz = strlen(ptr);

			//We will detect via blank lines
			if (sz == 0)
				break;
		}

		//If there were like, 2 blank lines in a row... this doesn't count.
		if (i == j)
			continue;

		//Set the file range
		fptr = &cn_vec_get(files, FRANGE, fc);

		fptr->start = i;
		fptr->end   = j;

		if (verbose)
			printf("  %d: %d, %d\n", fc, fptr->start, fptr->end);

		//Increment the file count and lines
		fc++;
		i = j;
	}

	return fc;
}

/*
 * split_files                                                             {{{2
 *
 * Given a CN_VEC<char *> of lines from a file (probably via "file_to_cn_vec"),
 * a CN_VEC<FRANGE> after being resized to the number of splits (probably via
 * "split_count", and a file prefix, split files up.
 */

void split_files(CN_VEC lines, CN_VEC files, char *prefix, int verbose) {
	size_t  splits, digits, flen, i, j, f;
	char   *buffer;
	FRANGE *range;
	FILE   *op;
	
	splits = cn_vec_size(files);
	digits = log10((double) (splits - 1)) + 1; /* number of digits for file  */
	flen   = strlen(prefix) + digits + 7;      /* predicted filename size    */
	f = 0;

	buffer = (char *) calloc(flen, sizeof(char));

	if (verbose)
		printf("\nOutputting files...\n");

	cn_vec_traverse(files, range) {
		sprintf(buffer, "%s%0*d.fasta", prefix, digits, f);

		if (verbose)
			printf("  %s...", buffer);

		//Output lines
		op = fopen(buffer, "w");
		for (i = range->start; i <= range->end; i++) {
			fprintf(
				op,
				"%s\n",
				cn_vec_get(lines, char *, i)
			);
		}
		fclose(op);

		if (verbose)
			printf(" Done!\n");

		f++;
	}

	//Free the buffer. We are done.
	free(buffer);
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char ** argv) {
	CN_VEC  files;
	CN_VEC  lines;
	size_t  i, j;
	int     splits, verbose;
	char  **it;

	//Argument Check
	if (argc < 3 || argc > 4) {
		fprintf(
			stderr,
			"usage: %s [-v] in_fasta out_prefix\n",
			argv[0]
		);
		return 1;
	}

	//If there's 4 arguments and that second one is "-v", verbose mode is on.
	verbose = 0;

	if (argc == 4 && strcmp(argv[1], "-v") == 0)
		verbose = 1;

	//Setup the CN_VECs
	lines = cn_vec_init(char *);
	files = cn_vec_init(FRANGE);

	//Read in all lines into the CN_VEC
	if (!file_to_cn_vec(argv[1 + (argc == 4)], lines, verbose)) {
		fprintf(stderr, "fatal: Failed to open \"%s\": ", argv[1]);
		perror("");
		return 2;
	}

	//Setup files based on how many there should be.
	splits = split_count(lines, verbose);
	cn_vec_resize(files, splits);

	//Fill in "files" with line number ranges for the split
	split_lines(lines, files, verbose);

	//Commence splitting and exporting.
	split_files(lines, files, argv[2 + (argc == 4)], verbose);

	//Clean up C-Strings that are inside the lines vector
	cn_vec_traverse(lines, it)
		free(*it);

	//Clean up all CNDS vars
	cn_vec_free(lines);
	cn_vec_free(files);

	//We're done here.
	return 0;
}
