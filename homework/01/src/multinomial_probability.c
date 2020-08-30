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

void   destruct_key(CNM_NODE *node);
CN_MAP report_freq (CN_STRING, const char *, size_t, size_t);
void   freq_analyse(CN_MAP, CN_STRING, const char *, size_t, size_t, size_t,
                    char *);

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
	char        *line, *str, *search_str;
	FILE        *fp;
	size_t       len, sz, i, j, pos;
	ssize_t      nread;
	CN_STRING    seq;
	size_t       freq[4];
	const char   opt[4] = { 'A', 'C', 'G', 'T' };
	CN_MAP       results;
	CNM_ITERATOR it;
	long double  val;

	//Argument check
	if (argc != 2) {
		fprintf(stderr, "usage: %s in_fasta\n", argv[0]);
		return 1;
	}

	//Default values
	line = NULL;
	freq[0] = freq[1] = freq[2] = freq[3] = 0;

	//Open file for reading
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "fatal: Failed to open \"%s\": ", argv[1]);
		perror("");
		return 2;
	}

	seq = cn_string_init();

	//Read in lines and reverse 
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
		cn_string_concat_from_cstr(seq, line);

		//Clean up
		free(line);
		line = NULL;
	}

	if (line)
		free(line);

	fclose(fp);

	//Ok, go through every character in the string, complement, print.
	str = cn_string_str(seq);
	sz  = cn_string_len(seq);

	//Compute frequencies
	results = report_freq(seq, opt, 4, 1);

	//Compute the probability
	val = 0.0f;

	//Iterate through the results map and use clever logarithmic math to cheat
	cn_map_traverse(results, &it) {
		freq_pair *pair = &cn_map_iterator_value(&it, freq_pair);

		//Print out data structure information
		printf(
			"{ %s, { %d, %.17Lg} }\n",
			cn_map_iterator_key(&it, char *),
			pair->freq, pair->prob
		);

		val += pair->freq * logl(pair->prob);
	}

	/*
	printf("      P  = %.17Lg\n", val);
	printf("log10(P) = %.17Lg\n", log10l(val));
	printf("   ln(P) = %.17Lg\n", logl(val));
	*/

	printf("\nln(P) = %.17Lg\n", val);

	//Clean up
	cn_string_free(seq);
	cn_map_free(results);

	return 0;
}
