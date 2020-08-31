/*
 * Multinomial Probability
 *
 * Description:
 *     Computes the log (natural logarithm) of a sequence under the multinomial
 *     model.
 *
 * Author:
 *     Clara Nguyen
 */

//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Helper with common functions
#include "helper.h"

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"
#include "../lib/cnds/cn_cmp.h"
#include "../lib/cnds/cn_map.h"

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char **argv) {
	//Initial Variables
	char        *str, *search_str;
	FILE        *fp;
	size_t       sz, i, j, pos;
	long double  val;

	//CNDS
	CN_STRING    training_seq, gen_seq;
	CN_MAP       train_results, gen_results;
	CNM_ITERATOR it, itt;

	//Valid characters in fasta
	const char   opt[4] = { 'A', 'C', 'G', 'T' };

	//Argument check
	if (argc != 3) {
		fprintf(stderr, "usage: %s in_training_fasta in_gen_fasta\n", argv[0]);
		return 1;
	}

	//Import fasta files
	training_seq = fasta_import_to_cn_string(argv[1]);
	gen_seq      = fasta_import_to_cn_string(argv[2]);

	//Ok, go through every character in the string, complement, print.
	str = cn_string_str(training_seq);
	sz  = cn_string_len(training_seq);

	search_str = (char *) malloc(sizeof(char) * 2);
	search_str[1] = 0;

	//Compute frequencies
	train_results = report_freq(training_seq, opt, 4, 1);
	gen_results   = report_freq(gen_seq     , opt, 4, 1);

	//Compute the probability
	val = 0.0f;

	//Iterate through the results map and use clever logarithmic math to cheat
	for (i = 0; i < 4; i++) {
		//Generate the search string and traverse CN_Map for probability data
		search_str[0] = opt[i];
		cn_map_find(train_results, &it , &search_str);
		cn_map_find(gen_results  , &itt, &search_str);

		freq_pair *pair[2];
		pair[0] = &cn_map_iterator_value(&it , freq_pair);
		pair[1] = &cn_map_iterator_value(&itt, freq_pair);

		//Math and stuff
		val += pair[1]->freq * logl(pair[0]->prob);
	}

	//Print out. We're done here.
	printf("ln(P) = %.17Lg\n", val);

	//Clean up
	cn_string_free(training_seq);
	cn_map_free(train_results);
	cn_map_free(gen_results);

	return 0;
}
