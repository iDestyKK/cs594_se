/*
 * Hamming Distance Matrix Generator
 *
 * Description:
 *     Useful tool for comparing and printing out specific data for differences
 *     between multiple FASTA files. Score and aligned string between all
 *     possible FASTA files is supported.
 *
 * Author:
 *     Clara Nguyen
 */

//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg_parse.h"
#include "helper.h"
#include "dp.h"

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"
#include "../lib/cnds/cn_vec.h"

// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

//Shortcut macro for parameter printing stuff
#define print_header(counter, flag, str) \
	if (counter++ > 0) printf("\n"); \
	if (!flag) printf("%s", str);

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

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char ** argv) {
	size_t i, j, k, l, sz, nf, indx, il;
	int *score, highest;
	CN_VEC f, dp_objs, scores;
	CN_STRING *sit;
	DP *objs;
	PARAM_T params;

	//Argument check
	if (argc < 7 || (argc >= 2 && argv[1][0] != '-')) {
		fprintf(
			stderr,
			"usage: %s -aqs file1 file2 [file3 [file4 [...]]] match_val"
			" mismatch_val gap_val\n\n"
			"Parameter Information:\n"
			"\t-a\tDisplay strings post-alignment\n"
			"\t-q\tQuiet(er). Stops printing headers before each section\n"
			"\t-s\tDisplay the hamming distance matrix of scores\n"
			"\nThe information printed out is in the order you define the "
			"arguments. So \"-as\" will print\nthe alignments, then the "
			"matrix. And \"-sa\" will do the same but in reverse order.\n",
			argv[0]
		);
		return 1;
	}

	params = arg_init();

	//Parse arguments
	arg_parse(params, argv[1]);

	f = cn_vec_init(CN_STRING);
	nf = argc - 5;
	cn_vec_resize(f, nf);

	//Setup CN_VEC of DP objects
	dp_objs = cn_vec_init(DP);
	cn_vec_resize(dp_objs, nf * nf);

	//Read in the files
	for (i = 2; i < argc - 3; i++)
		cn_vec_get(f, CN_STRING, i - 2) = fasta_import_to_cn_string(argv[i]);

	//Setup scores to be seq x seq (all pairs/combinations).
	scores = cn_vec_init(int);
	cn_vec_resize(scores, nf * nf);

	highest = 0;

	//Construct the DP table for all possible pairs/combinations
	sit   = (CN_STRING *) cn_vec_data(f      );
	objs  = (DP        *) cn_vec_data(dp_objs);
	score = (int       *) cn_vec_data(scores );

	for (i = 0; i < nf; i++) {
		for (j = i; j < nf; j++) {
			indx = ij2indx(i, j, nf);

			objs[indx] = dp_init(
				cn_string_str(sit[i]), cn_string_str(sit[j]),
				atoi(argv[argc - 3]),
				atoi(argv[argc - 2]),
				atoi(argv[argc - 1])
			);

			//Ok... do it.
			dp_run_global(objs[indx]);

			//print_header(j, params->flag_quiet, "STRING ALIGNMENT:\n");
			score[indx] = dp_hamming_distance(
				objs[indx],
				objs[indx]->w - 1,
				objs[indx]->h - 1,
				1
			);

			//Assign the highest score to "highest"
			if (score[indx] > highest)
				highest = score[indx];
		}
	}

	sz = strlen(argv[1]);
	k = 0;

	for (l = 0; l < sz; l++) {
		switch (argv[1][l]) {
			case 'a':
				print_header(k, params->flag_quiet, "STRING ALIGNMENT:\n");

				//Print those alignments out
				for (i = 0; i < nf; i++) {
					for (j = i + 1; j < nf; j++) {
						indx = ij2indx(i, j, nf);
						printf("\nDIFF STR%d VS STR%d:\n", i, j);

						dp_print_align(
							objs[indx],
							objs[indx]->w - 1,
							objs[indx]->h - 1,
							1
						);
					}
				}
				
				break;

			case 's':
				print_header(k, params->flag_quiet, "HAMMING DIST. MATRIX:\n");

				//Use length of largest score to determine space needed
				il = intlen(highest);
				
				//Print those scores out
				for (i = 0; i < nf; i++) {
					for (j = 0; j < nf; j++) {
						printf(
							/* only print spaces prior to end of line */
							(j == nf - 1)
								? " %*d"
								: " %*d ",
							il,
							score[ij2indx(i, j, nf)]
						);
					}
					printf("\n");
				}

				break;
		}
	}


	//Free memory
	arg_free(params);

	//Free all DP instances
	for (i = 0; i < nf; i++)
		for (j = i; j < nf; j++)
			dp_free(cn_vec_get(dp_objs, DP, ij2indx(i, j, nf)));

	//Free all FASTA strings
	cn_vec_traverse(f, sit)
		cn_string_free(*sit);

	//Free the vectors that contained... well... everything...
	cn_vec_free(dp_objs);
	cn_vec_free(scores);
	cn_vec_free(f);

	//We're done here.
	return 0;
}
