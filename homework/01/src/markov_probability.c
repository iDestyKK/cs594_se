/*
 * Markov Probability
 *
 * Description:
 *     Computes the Markov chain probability model of a specific chain (3). We
 *     can think of this as like multinomial model except it relies on the
 *     values of the preceding nucleotides. Order 1 relies on 1 proceeding
 *     nucleotide. Order 2 relies on 2 proceeding. etc.
 *
 *     Users may specify the order if they want. By default, it's 3 (as per the
 *     assignment question wanted). Anything past 4 will likely return negative
 *     infinity though.
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
	char        *line, *str, *search_str;
	FILE        *fp;
	size_t       sz, i, j, pos;
	long double  val;
	size_t       chain_len;
	freq_pair   *pair[2];

	//CNDS
	CN_STRING    training_seq, gen_seq;
	CN_MAP       training_results[3];
	CNM_ITERATOR it, itt;

	//Valid characters in fasta
	const char   opt[4] = { 'A', 'C', 'G', 'T' };

	//Argument check
	if (argc != 3 && argc != 4) {
		fprintf(
			stderr,
			"usage: %s in_training_fasta in_gen_fasta [order]\n",
			argv[0]
		);
		return 1;
	}

	//Get the order of the Markov model (default is 3)
	if (argc == 4)
		chain_len = atoi(argv[3]);
	else
		chain_len = 3;

	//Import the fasta files to CN_Strings
	training_seq = fasta_import_to_cn_string(argv[1]);
	gen_seq      = fasta_import_to_cn_string(argv[2]);

	//Ok, go through every character in the string, complement, print.
	str = cn_string_str(gen_seq);
	sz  = cn_string_len(gen_seq);

	//Construct the search string for CN_Map traversal
	search_str = (char *) malloc(sizeof(char) * chain_len + 2);
	search_str[chain_len + 1] = 0;

	//Compute frequencies
	training_results[0] = report_freq(training_seq, opt, 4, chain_len + 1);
	training_results[1] = report_freq(training_seq, opt, 4, chain_len    );
	training_results[2] = report_freq(training_seq, opt, 4,             1);

	/*
	 * As per Piazza, divide 4-mer hash dictionary values by 3-mer hash values.
	 * e.g. CTTG's probability is divided by CTT's probability.
	 *
	 * We can do this via going through the larger map (trim last char) or
	 * smaller map (add a char). Larger allows us to cheat by setting the last
	 * char to NULL, copying via loop to n - 1, or both.
	 */

	cn_map_traverse(training_results[0], &it) {
		//Duplicate the string into "search_str" without last character.
		for (i = 0; i < chain_len; i++)
			search_str[i] = cn_map_iterator_key(&it, char *)[i];

		search_str[i] = 0;

		//Find key in the map of 1 less character
		cn_map_find(training_results[1], &itt, &search_str);

		//Commence division
		pair[0] = &cn_map_iterator_value(&it , freq_pair);
		pair[1] = &cn_map_iterator_value(&itt, freq_pair);

		pair[0]->prob /= pair[1]->prob;
	}

	//Compute the probability
	val = 0.0f;

	/*
	 * For starters... that pi value in the equation. That's just the first
	 * "chain_len" chars of probability thrown in from the multinomial model.
	 */

	for (i = 0; i < chain_len; i++) {
		//Generate lookup string
		search_str[0] = str[i];
		search_str[1] = 0;

		//Look through the CN_Map, get the probability, add it in.
		cn_map_find(training_results[2], &it, &search_str);

		val += logl(cn_map_iterator_value(&it, freq_pair).prob);
	}

	/*
	 * Ok... now go through every few characters and do logarithm
	 * multiplication via addition.
	 */

	for (i = 0; i < sz - chain_len; i++) {
		//Assign the search query and search. It's guaranteed to exist.
		for (j = 0; j < chain_len + 1; j++)
			search_str[j] = str[i + j];

		cn_map_find(training_results[0], &it, &search_str);

		//Extract probability and add it in
		freq_pair *pair = &cn_map_iterator_value(&it, freq_pair);

		val += logl(pair->prob);
	}

	printf("ln(P) = %.17Lg\n", val);

	//Clean up
	free(search_str);

	cn_string_free(training_seq);
	cn_string_free(gen_seq);

	cn_map_free(training_results[0]);
	cn_map_free(training_results[1]);
	cn_map_free(training_results[2]);

	return 0;
}
