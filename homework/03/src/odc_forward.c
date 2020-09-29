/*
 * Occasionally Dishonest Casino - Sequence Prediction via Forward Algorithm
 *
 * Description:
 *     When given a file of dice rolls (1-6 respectively), print out the most
 *     likely state sequence to a file. This variation simply runs the file
 *     through the Forward algorithm.
 *
 * Author:
 *     Clara Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"

//First dimension
#define F 0 //Fair
#define L 1 //Loaded

// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

/*
 * seq_import_to_cn_string
 *
 * Imports sequence data into a CN_String, if possible. Program exits with an
 * error message if such a file doesn't exist.
 */

CN_STRING seq_import_to_cn_string(const char *path) {
	FILE     *fp;
	char     *line;
	ssize_t   nread;
	size_t    len, sz, i;
	CN_STRING str;

	line = NULL;

	//Open the sequence file for reading
	fp = fopen(path, "r");

	if (fp == NULL) {
		fprintf(stderr, "fatal: Failed to open \"%s\": ", path);
		perror("");
		exit(2);
	}

	str = cn_string_init();

	while (nread = getline(&line, &len, fp) != -1) {
		sz = strlen(line);

		//Erase newline & carriage return if possible
		for (i = 0; i < 2; i++)
			if (line[sz - 1] == '\n' || line[sz - 1] == '\r')
				line[sz-- - 1] = 0;

		cn_string_concat_from_cstr(str, line);

		//Clean up
		free(line);
		line = NULL;
	}

	if (line)
		free(line);

	fclose(fp);
	
	return str;
}

/*
 * l2id
 *
 * Converts a character to an ID. More specifically, '1' becomes 0. '2' becomes
 * 1, etc. Anything that isn't ASCII-representation of 1-6 will return 6. If a
 * valid value is passed in, it will always be 0-5 (inclusively).
 */

size_t l2id(char c) {
	if (c >= '1' && c <= '6')
		return c - '1';

	return 6;
}

/*
 * maxd_ext
 *
 * Returns the maximum of 2 doubles (maxd). This is an extended function, also
 * modifying a given "ch" to store which of the first two arguments was chosen.
 * If a > b, ch will be 0. Otherwise, ch will be 1.
 */

double maxd_ext(double a, double b, char *ch) {
	if (a > b) {
		*ch = 0;
		return a;
	}

	*ch = 1;
	return b;
	//return (a > b) ? a : b;
}

/*
 * dump_table
 *
 * Dumps the table to a specified stream "fp". This may be "stdout" or a file
 * opened via fopen.
 */

void dump_table(FILE *fp, long double *dp[2], char *ch[2], size_t len) {
	size_t i, j;

	for (j = 0; j < 2; j++) {
		for (i = 0; i < len; i++) {
			fprintf(fp, "%c", (ch[j][i] == 0) ? '-' : 'X');
			fprintf(fp, "[%7.4Lg]", dp[j][i]);
		}
		fprintf(fp, "\n");
	}
}

/*
 * dump_bt_seq
 *
 * Performs a backtracking from the very end of the table "dp". This is to a
 * specified stream "fp". This may be "stdout" or a file opened via fopen.
 */

void dump_bt_seq(FILE *fp, long double *dp[2], char *ch[2], size_t len) {
	int    i;
	char   s;
	char  *seq;

	//Generate string to store results in
	seq = (char *) malloc(sizeof(char) * (len + 1));
	seq[len] = 0;
	
	//Get the lowest of the final results
	maxd_ext(dp[F][len - 1], dp[L][len - 1], &s);

	//Now then... backtrack.
	for (i = len - 1; i >= 0; i--) {
		seq[i] = ((s == 0) ? 'F' : 'L');

		//Flip if we came from the other way
		if (ch[s][i] == 1)
			s = !s;
	}

	fprintf(fp, "%s\n", seq);
	free(seq);
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char **argv) {
	long double *dp[2];
	char        *ch[2];
	char        *seq;
	CN_STRING    cns_seq;
	size_t       len, i;
	long double  of, ou_a, ou_b;
	FILE        *op;

	if (argc != 3) {
		fprintf(stderr, "usage: %s in_file out_file\n", argv[0]);
		return 1;
	}

	//Configure odds
	of   = ((long double) 1.0) / 6;
	ou_a = ((long double) 1.0) / 10;
	ou_b = ((long double) 1.0) / 2;

	long double HMM[2][6] = {
		        /* 1     2     3     4     5     6     */
		/* F */ {  of  , of  , of  , of  , of  , of    },
		/* L */ {  ou_a, ou_a, ou_a, ou_a, ou_a, ou_b  }
	};

	long double swp[2][2] = {
		/* FF    FL    */
		{  0.95, 0.05  },

		/* LF    LL    */
		{  0.10, 0.90  }
	};

	//We'll do something similar to HW1...
	cns_seq = seq_import_to_cn_string(argv[1]);

	//Extract C-string data from the CN_String
	seq = cn_string_str(cns_seq);
	len = cn_string_len(cns_seq);

	dp[F] = (long double *) calloc(len, sizeof(long double));
	dp[L] = (long double *) calloc(len, sizeof(long double));
	ch[F] = (char        *) calloc(len, sizeof(char       ));
	ch[L] = (char        *) calloc(len, sizeof(char       ));

	//First states. We assume it's always fair at the start. Thus:
	dp[F][0] = swp[F][F] * HMM[F][l2id(seq[0])];
	dp[L][0] = swp[F][L] * HMM[L][l2id(seq[0])];

	//Now then... the others.
	for (i = 1; i < len; i++) {
		//math... lol
		dp[F][i] =
			(dp[F][i - 1] * swp[F][F] * HMM[F][l2id(seq[i])]) +
			(dp[L][i - 1] * swp[L][F] * HMM[F][l2id(seq[i])]);

		dp[L][i] =
			(dp[L][i - 1] * swp[L][L] * HMM[L][l2id(seq[i])]) +
			(dp[F][i - 1] * swp[F][L] * HMM[L][l2id(seq[i])]);
	}

	//Dump information
	//dump_table(stdout, dp, ch, len);
	printf("%Lg\n", dp[F][len - 1] + dp[L][len - 1]);

	//Clean up. We're done here
	free(dp[0]);
	free(dp[1]);
	free(ch[0]);
	free(ch[1]);

	cn_string_free(cns_seq);

	return 0;
}
