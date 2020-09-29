/*
 * Viterbi Algorithm - A toy example
 *
 * Description:
 *     Implements the example demonstrated in the following PDF (just so I know
 *     what I am doing):
 *     https://www.cis.upenn.edu/~cis262/notes/Example-Viterbi-DNA.pdf
 *
 *     May not even be included in the final submission. It's just a hack.
 *
 * Author:
 *     Clara Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//First dimension
#define H 0
#define L 1

//Second dimension (DNA)
#define A 0
#define C 1
#define G 2
#define T 3

// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

//DNA Letter to ID
size_t l2id(char c) {
	switch (c) {
		case 'A': return A;
		case 'C': return C;
		case 'G': return G;
		case 'T': return T;
	};

	return A;
}

double maxd_ext(double a, double b, char *ch) {
	if (a > b) {
		*ch = 0;
		return a;
	}

	*ch = 1;
	return b;
	//return (a > b) ? a : b;
}

void dump_table(double *dp[2], char *ch[2], size_t len) {
	size_t i, j;

	for (j = 0; j < 2; j++) {
		for (i = 0; i < len; i++) {
			printf("%c", (ch[j][i] == 0) ? '-' : 'X');
			printf("[%7.4lg]", dp[j][i]);
		}
		printf("\n");
	}
}

void dump_bt_seq(double *dp[2], char *ch[2], size_t len) {
	int    i;
	char   s;
	char  *seq;

	//Generate string to store results in
	seq = (char *) malloc(sizeof(char) * (len + 1));
	seq[len] = 0;
	
	//Get the lowest of the final results
	maxd_ext(dp[H][len - 1], dp[L][len - 1], &s);

	//Now then... backtrack.
	for (i = len - 1; i >= 0; i--) {
		seq[i] = ((s == 0) ? 'H' : 'L');

		//Flip if we came from the other way
		if (ch[s][i] == 1)
			s = !s;
	}

	printf("%s\n", seq);
	free(seq);
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char **argv) {
	double *dp[2];
	char   *ch[2];
	char   *seq;
	size_t  len, i;

	if (argc != 2) {
		fprintf(stderr, "usage: %s sequence\n", argv[0]);
		return 1;
	}

	double HMM[2][4] = {
		/* A    C    G    T    */
		{  0.2, 0.3, 0.3, 0.2  },
		{  0.3, 0.2, 0.2, 0.3  }
	};

	                       /* H2L  L2H */
	double swp[2][2] = {
		/* HH   HL   */
		{  0.5, 0.5  },

		/* LH   LL   */
		{  0.4, 0.6  }
	};

	seq = argv[1];
	len = strlen(seq);

	dp[H] = (double *) calloc(len, sizeof(double));
	dp[L] = (double *) calloc(len, sizeof(double));
	ch[H] = (char   *) calloc(len, sizeof(char  ));
	ch[L] = (char   *) calloc(len, sizeof(char  ));

	//First states
	dp[H][0] = log2(0.5) + log2(HMM[H][l2id(seq[0])]);
	dp[L][0] = log2(0.5) + log2(HMM[L][l2id(seq[0])]);

	//Now then... the others.
	for (i = 1; i < len; i++) {
		dp[H][i] = log2(HMM[H][l2id(seq[i])]) + maxd_ext(
			log2(swp[H][H]) + dp[H][i - 1],
			log2(swp[L][H]) + dp[L][i - 1],
			&ch[H][i]
		);

		dp[L][i] = log2(HMM[L][l2id(seq[i])]) + maxd_ext(
			log2(swp[L][L]) + dp[L][i - 1],
			log2(swp[H][L]) + dp[H][i - 1],
			&ch[L][i]
		);
	}

	dump_table(dp, ch, len);

	dump_bt_seq(dp, ch, len);

	//Clean up. We're done here
	free(dp[0]);
	free(dp[1]);
	free(ch[0]);
	free(ch[1]);

	return 0;
}
