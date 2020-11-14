/*
 * Hamming Distance Matrix Generator
 *
 * Description:
 *     Computes the hamming distance between a series of FASTA files. Any
 *     number of them may be passed in via argv. This program assumes that the
 *     FASTA files contain one sequence per file. If you have a file with many
 *     sequences, run it through the "fasta_split" utility first.
 *
 *     A matrix of the scores will be printed out by ID.
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
#include "../lib/cnds/cn_vec.h"

//Other secret weapons
#include "helper.h"


// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

/*
 * ij2indx                                                                 {{{2
 *
 * Converts i, j 2d matrix coordinate into a 1d array index given the number of
 * sequences we are computing the hamming distance for. This is so we can use a
 * 1D vector to store results rather than a 2D one.
 */

inline int ij2indx(size_t i, size_t j, size_t seq_num) {
	if (j >= i)
		return j + (i * seq_num);

	return i + (j * seq_num);
}

/*
 * hamming_distance                                                        {{{2
 *
 * Computed the hamming distance between two strings in "seqs", namely "s0" and
 * "s1".
 */

int hamming_distance(CN_VEC seqs, size_t s0, size_t s1) {
	char *str[2];
	size_t score, i, slen;

	//Extract C-Strings from the "seqs" CN_VEC
	str[0] = cn_string_str(cn_vec_get(seqs, CN_STRING, s0));
	str[1] = cn_string_str(cn_vec_get(seqs, CN_STRING, s1));

	//Constant time string length lookup
	slen = cn_string_len(cn_vec_get(seqs, CN_STRING, s0));

	//Now then... compute the score
	for (score = 0, i = 0; i < slen; i++)
		score += (str[0][i] != str[1][i]);

	return score;
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char ** argv) {
	size_t     seq_num;
	size_t     i, j;
	int       *it;
	CN_VEC     seqs;
	CN_VEC     scores;
	CN_STRING *sit;

	//Argument check
	if (argc <= 2) {
		fprintf(
			stderr,
			"usage: %s file1 file2 [file3 [file4 [...]]]\n",
			argv[0]
		);
		return 1;
	}

	//Pre-allocate sequences
	seq_num = argc - 1;

	seqs = cn_vec_init(CN_STRING);
	cn_vec_resize(seqs, seq_num);

	//Load them in
	i = 0;
	cn_vec_traverse(seqs, sit)
		*sit = fasta_import_to_cn_string(argv[i++ + 1]);

	//Setup scores to be seq x seq (all pairs/combinations).
	scores = cn_vec_init(int);
	cn_vec_resize(scores, seq_num * seq_num);

	//Generate those scores
	for (i = 0; i < seq_num; i++) {
		//We only need the top-right-half of the matrix. f(i, j) = f(j, i)
		for (j = i; j < seq_num; j++) {
			//Assign vector entry to 0 if equal. Hamming otherwise.
			cn_vec_get(scores, int, ij2indx(i, j, seq_num)) =
				(i == j) ? 0 : hamming_distance(seqs, i, j);
		}
	}

	//Print those scores out
	for (i = 0; i < seq_num; i++) {
		for (j = 0; j < seq_num; j++) {
			it = &cn_vec_get(scores, int, ij2indx(i, j, seq_num));
			printf("%3d ", *it);
		}
		printf("\n");
	}

	//We're done. Free everything.
	cn_vec_traverse(seqs, sit)
		cn_string_free(*sit);

	cn_vec_free(seqs);
	cn_vec_free(scores);

	//We're done here
	return 0;
}
