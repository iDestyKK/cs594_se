/*
 * Global Alignment Utility
 *
 * Description:
 *     Useful tool for comparing and printing out specific data for differences
 *     between two FASTA files. Score, aligned string, DP table printing with
 *     backtrack highlighting supported.
 *
 *     This variant runs the two FASTA files through Global Alignment.
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
	size_t i, j, sz, nf, indx;
	CN_VEC f, dp_objs;
	CN_STRING *sit;
	DP *objs;
	PARAM_T params;

	//Argument check
	if (argc < 7) {
		fprintf(
			stderr,
			"usage: %s -amqst file1 file2 [file3 [file4 [...]]] match_val"
			" mismatch_val gap_val\n\n"
			"Parameter Information:\n"
			"\t-a\tDisplay strings post-alignment\n"
			"\t-m\tDisplay the DP table\n"
			"\t-q\tQuiet(er). Stops printing headers before each section\n"
			"\t-s\tDisplay the global alignment score\n"
			"\t-t\tHighlight the traceback in red via ANSI (enables \"-m\" "
			"flag)\n"
			"\nThe information printed out is in the order you define the "
			"arguments. So \"-am\" will print\nthe alignment, then the DP "
			"table. And \"-ma\" will do the same but in reverse order.\n",
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

	//Construct the DP table for all possible pairs/combinations
	sit  = (CN_STRING *) cn_vec_data(f      );
	objs = (DP        *) cn_vec_data(dp_objs);

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
		}
	}
/*
	//Ok, print out whatever was specified
	sz = strlen(argv[1]);
	j = 0;

	for (i = 0; i < sz; i++) {
		switch (argv[1][i]) {
			case 'a':
				print_header(j, params->flag_quiet, "STRING ALIGNMENT:\n");
				dp_print_align(obj, obj->w - 1, obj->h - 1, 1);
				break;

			case 'm':
				if (params->flag_highlight)
					dp_backtrack(obj, obj->w - 1, obj->h - 1, 1);

				print_header(j, params->flag_quiet, "DP TABLE:\n");
				dp_print_table(obj);
				break;

			case 's':
				print_header(j, params->flag_quiet, "ALIGNMENT SCORE:\n");
				printf("%d\n", obj->table[obj->w - 1][obj->h - 1]);
				break;
		}
	}
	*/

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
	cn_vec_free(f);

	//We're done here.
	return 0;
}
