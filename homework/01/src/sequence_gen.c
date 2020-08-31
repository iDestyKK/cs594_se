/*
 * Sequence Generator
 *
 * Description:
 *     Generates a sequence of characters when given training data, seed,
 *     Markov model order, and length. The minimum number of characters
 *     generated must be equal to the Markov model order given.
 *
 *     Only the training data is required. When the other options are omit, the
 *     default settings are used which correspond to Question 7 in the
 *     homework.
 *
 * Author:
 *     Clara Nguyen
 */

//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Helper with common functions
#include "helper.h"

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"
#include "../lib/cnds/cn_cmp.h"
#include "../lib/cnds/cn_map.h"

// ----------------------------------------------------------------------------
// Helper Function Prototypes                                              {{{1
// ----------------------------------------------------------------------------

int bucket_select(long double, long double, long double, long double);

// ----------------------------------------------------------------------------
// Helper Function Definitions                                             {{{1
// ----------------------------------------------------------------------------

/*
 * bucket_select
 *
 * Given 4 options, chooses a random number and then returns an index based on
 * where that random number lands. The random number is ensured to land between
 * the sum of the 4 given numbers.
 */

int bucket_select(long double a, long double b, long double c, long double d) {
	long double total, t, r, val[4];
	size_t i;

	//Generate random number between 0.0 and total of all 4 given arguments
	total = a + b + c + d;
	t = 0;
	r = ((long double) rand()) / RAND_MAX;
	r *= total;

	//Assign numbers to an array
	val[0] = a;
	val[1] = b;
	val[2] = c;
	val[3] = d;

	for (i = 0; i < 4; i++) {
		t += val[i];
		if (r < t)
			return i;
	}

	//Return the last number I guess
	return 3;
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char **argv) {
	//Initial Variables
	char        *line, *str, *search_str, *ins;
	FILE        *fp;
	size_t       sz, i, j, pos;
	long double  val, prob[4];
	size_t       chain_len, seq_len, seed;
	int          select;
	freq_pair   *pair[2];

	//CNDS
	CN_STRING    training_seq, gen_str, substr;
	CN_MAP       markov_model, multinomial_model;
	CNM_ITERATOR it, itt;

	//Valid characters in fasta
	const char   opt[4] = { 'A', 'C', 'G', 'T' };

	//Argument check
	if (argc != 2 && argc != 5) {
		fprintf(
			stderr,
			"usage: %s in_training_fasta [length order seed]\n",
			argv[0]
		);
		return 1;
	}

	//Argument parsing
	if (argc == 5) {
		//User specified their own
		seq_len   = atoi(argv[2]);
		chain_len = atoi(argv[3]);
		seed      = atoi(argv[4]);

		//Markov Order check
		if (chain_len < 1) {
			fprintf(
				stderr,
				"fatal: Markov Order (%d) must be larger than 0.\n",
				chain_len
			);
			return 2;
		}

		//Invalid sequence length check
		if (seq_len < chain_len) {
			fprintf(
				stderr,
				"fatal: Sequence length (%d) must be >= Markov Order (%d).\n",
				seq_len,
				chain_len
			);
			return 3;
		}
	}
	else {
		//Default Parameters
		seq_len   = 20000;
		chain_len = 3;
		seed      = time(NULL);
	}

	srand(seed);

	//Import the fasta file to CN_String
	training_seq = fasta_import_to_cn_string(argv[1]);

	//Ok, go through every character in the string, complement, print.
	//str = cn_string_str(gen_seq);
	//sz  = cn_string_len(gen_seq);

	//Compute frequencies
	markov_model      = report_freq(training_seq, opt, 4, chain_len + 1);
	multinomial_model = report_freq(training_seq, opt, 4,             1);

	//Setup the sequence string and insert c-string
	gen_str = cn_string_init();

	/*
	 * STEP 1: Generate the first "chain_len" characters based on multinomial
	 * model
	 */

	ins = (char *) malloc(sizeof(char) * 2);
	ins[1] = 0;

	//Put multinomial probabilities into array
	for (i = 0; i < 4; i++) {
		ins[0] = opt[i];
		cn_map_find(multinomial_model, &it, &ins);

		prob[i] = cn_map_iterator_value(&it, freq_pair).prob;
	}

	//Generate the characters
	for (i = 0; i < chain_len; i++) {
		//Pick a character based on multinomial model probabilities
		select = bucket_select(prob[0], prob[1], prob[2], prob[3]);

		ins[0] = opt[select];
		cn_string_concat_from_cstr(gen_str, ins);
	}


	/*
	 * STEP 2: Generate the remaining characters based on the markov model
	 */

	for (; i < seq_len; i++) {
		/*
		 * Put the markov probabilities into array. This relies on the previous
		 * "chain_len" characters in the string.
		 */

		for (j = 0; j < 4; j++) {
			//Grab the n - 1 characters
			substr = cn_string_substr(gen_str, i - chain_len, chain_len);

			//Generate string with the last char and combine
			ins[0] = opt[j];
			cn_string_concat_from_cstr(substr, ins);

			//Search the tree and plug in the probability.
			search_str = cn_string_str(substr);
			cn_map_find(markov_model, &it, &search_str);

			prob[j] = cn_map_iterator_value(&it, freq_pair).prob;

			//Free. Restart if needed.
			cn_string_free(substr);
		}

		//Pick a character based on markov model probabilities
		select = bucket_select(prob[0], prob[1], prob[2], prob[3]);

		ins[0] = opt[select];
		cn_string_concat_from_cstr(gen_str, ins);
	}

	free(ins);

	printf("%s\n", cn_string_str(gen_str));

	//Clean up
	//free(search_str);

	cn_string_free(training_seq);
	cn_string_free(gen_str);
	//cn_string_free(substr);

	cn_map_free(markov_model);
	cn_map_free(multinomial_model);

	return 0;
}
