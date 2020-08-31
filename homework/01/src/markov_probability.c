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

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"
#include "../lib/cnds/cn_cmp.h"
#include "../lib/cnds/cn_map.h"

// ----------------------------------------------------------------------------
// Structs                                                                 {{{1
// ----------------------------------------------------------------------------

/* struct for holding pair of size_t and long double */
typedef struct {
	size_t      freq;
	long double prob;
} freq_pair;

// ----------------------------------------------------------------------------
// Helper Function Prototypes                                              {{{1
// ----------------------------------------------------------------------------

void      destruct_key          (CNM_NODE *node);
CN_STRING cn_string_import_fasta(const char *);
CN_MAP    report_freq           (CN_STRING, const char *, size_t, size_t);
void      freq_analyse          (CN_MAP, CN_STRING, const char *, size_t,
                                 size_t, size_t, char *);

// ----------------------------------------------------------------------------
// Helper Function Definitions                                             {{{1
// ----------------------------------------------------------------------------

/*
 * Destructor to free the C-String key in a CN_Map.
 */

void destruct_key(CNM_NODE *node) {
	char *v = *(char **) node->key;

	if (v != NULL)
		free(v);
}

/*
 * fasta_import_to_cn_string
 *
 * Imports a fasta file into a CN_String, if possible. Program exits with an
 * error message if such a file doesn't exist.
 */

CN_STRING fasta_import_to_cn_string(const char *path) {
	FILE    *fp;
	char    *line;
	ssize_t  nread;
	size_t   len, sz, i;
	CN_STRING str;

	line = NULL;

	//Open training file for reading
	fp = fopen(path, "r");
	if (fp == NULL) {
		fprintf(stderr, "fatal: Failed to open \"%s\": ", path);
		perror("");
		exit(2);
	}

	str = cn_string_init();

	//Read in lines from the training data
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

		//Change "N" into "A" if possible
		for (i = 0; i < sz; i++)
			if (line[i] == 'N')
				line[i] = 'A';

		//Add to the sequence string
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
 * report_freq
 *
 * Sets up recursion for a generic way to search a string for a combination
 * of characters specified in "bank" of size "len". If the bank is "ACGT" and
 * the length is 2, for example, it'll search for all possible combinations of
 * the 4 characters with length 2 (AA, AC, AG, AT, CA, etc) in string "str".
 */

CN_MAP report_freq(CN_STRING str, const char *bank, size_t bsz, size_t len) {
	//String to hold the substring we are looking for in the file
	char *search_str = (char *) malloc(sizeof(char) * (len + 1));
	search_str[len] = 0;

	//CN_Map to store frequency results
	CN_MAP results = cn_map_init(char *, freq_pair, cn_cmp_cstr);
	cn_map_set_func_destructor(results, destruct_key);

	//Unleash recursion
	freq_analyse(results, str, bank, bsz, len, 0, search_str);

	//We're done here
	free(search_str);

	return results;
}

/*
 * freq_analyse
 *
 * Where the actual analysis takes place. Construct the search string
 * recursively and then search "str" for occurrences of it and print out the
 * results.
 */

void freq_analyse(
	CN_MAP      results,
	CN_STRING   str,
	const char *bank,
	size_t      bsz,
	size_t      len,
	size_t      pos,
	char       *search_str
) {
	size_t i, j, k, ssz;
	char *str_, *dup;

	freq_pair pair;

	for (i = 0; i < bsz; i++) {
		search_str[pos] = bank[i];

		if (pos < len - 1) {
			freq_analyse(results, str, bank, bsz, len, pos + 1, search_str);
			continue;
		}

		/*
		 * Assume we are at the last letter in the string. Search for
		 * string occurrences.
		 */

		str_ = cn_string_str(str);
		ssz  = cn_string_len(str);
		pair.freq = 0;

		for (j = 0; j < ssz; j++) {
			for (k = 0; k < len && j + k < ssz; k++)
				if (str_[j + k] != search_str[k])
					break;

			//String match
			if (k == len)
				pair.freq++;
		}

		//Store results into CN_Map
		pair.prob = (long double) pair.freq / (ssz - (len - 1));

		dup = strdup(search_str);
		cn_map_insert(results, &dup, &pair);
	}
}

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
	CN_MAP       training_results[2];
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
	 * "chain_len" chars of probability thrown in.
	 */

	//Generate lookup string
	for (i = 0; i < chain_len; i++)
		search_str[i] = str[i];
	
	search_str[i] = 0;

	//Look through the CN_Map, get the probability, add it in.
	cn_map_find(training_results[1], &it, &search_str);

	val += logl(cn_map_iterator_value(&it, freq_pair).prob);

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

	return 0;
}
