/*
 * Hirschberg Modification to End-Gap Free Alignment
 *
 * Description:
 *     Modifies the End-Gap Free Alignment algorithm to use linear space rather
 *     than a full DP table. This is done by utilising a clever property that
 *     we need to only look at the previous row to compute the answer we want.
 *
 *     Unfortunately, due to program structure, the "dp" class cannot be
 *     utilised here. So the DP table and its helper functions must be
 *     rewritten. They are present in this file. "dp.h" isn't included.
 *
 *     The directions don't state to include a string alignment. Only a score.
 *     So when we are computing the score, we can cheat by simply keeping track
 *     of the largest value encountered while running End-Gap Free Alignment on
 *     the 2 rows.
 *
 * Author:
 *     Clara Nguyen
 */

//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "arg_parse.h"
#include "helper.h"

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"

// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

/*
 * intlen                                                                  {{{2
 *
 * Figures out the length of an integer if it were parsed as a string
 */

size_t intlen(int v) {
	char *buffer;
	size_t len;

	buffer = (char *) calloc(12, sizeof(char));
	sprintf(buffer, "%d", v);
	len = strlen(buffer);
	free(buffer);

	return len;
}

/*
 * min                                                                     {{{2
 *
 * When given two numbers, return the lowest of the two. To find the minimum of
 * more numbers, stack the function calls (e.g. min(min(a, b), c);)
 */

int min(int a, int b) {
	return (a < b) ? a : b;
}

/*
 * max                                                                     {{{2
 *
 * When given two numbers, return the highest of the two. To find the maximum
 * of more numbers, stack the function calls (e.g. max(max(a, b), c);)
 */

int max(int a, int b) {
	return (a > b) ? a : b;
}

//Shortcut macro for parameter printing stuff
#define print_header(counter, flag, str) \
	if (counter++ > 0) printf("\n"); \
	if (!flag) printf("%s", str);

// ----------------------------------------------------------------------------
// DP Object Function Prototypes                                           {{{1
// ----------------------------------------------------------------------------

typedef enum dp_action {
	DP_NULL     = 0,
	DP_MATCH    = 1,
	DP_MISMATCH = 1,
	DP_SPACE1   = 2,
	DP_SPACE2   = 3
} DP_ACTION;

typedef struct dp {
	char       *s[2];
	size_t      l[2], w, h;
	int         val_match;
	int         val_mismatch;
	int         val_space;
	int       **table;
	char      **hl;
	DP_ACTION **actions;
} *DP;

//Function Prototypes
DP   dp_init       (char *, char *, int, int, int);
int  dp_run_egfa   (DP);
void dp_clear      (DP);
void dp_free       (DP);

// ----------------------------------------------------------------------------
// DP Object Function Declarations                                         {{{1
// ----------------------------------------------------------------------------

/*
 * dp_init                                                                 {{{2
 *
 * Initialises the DP object with a blank table and string information.
 */

DP dp_init(char *s1, char *s2, int v1, int v2, int v3) {
	size_t i, j;
	DP obj = (DP) malloc(sizeof(struct dp));

	//String information
	obj->s[0] = strdup(s1);
	obj->s[1] = strdup(s2);

	//String length too...
	obj->l[0] = strlen(obj->s[0]);
	obj->l[1] = strlen(obj->s[1]);

	//Values
	obj->val_match    = v1;
	obj->val_mismatch = v2;
	obj->val_space    = v3;

	//Construct blank table
	obj->w = obj->l[1] + 1;
	obj->h = obj->l[0] + 1;

	//Construction is the same as before, but height of 2 instead.
	obj->table = (int **) malloc(sizeof(int *) * obj->w);
	for (i = 0; i < obj->w; i++)
		obj->table[i] = (int *) calloc(2, sizeof(int));

	obj->hl = (char **) malloc(sizeof(char *) * obj->w);
	for (i = 0; i < obj->w; i++)
		obj->hl[i] = (char *) calloc(2, sizeof(char));

	obj->actions = (DP_ACTION **) malloc(sizeof(DP_ACTION *) * obj->w);
	for (i = 0; i < obj->w; i++)
		obj->actions[i] = (DP_ACTION *) calloc(2, sizeof(DP_ACTION));

	//Return
	return obj;
}

/*
 * dp_run_egfa                                                             {{{2
 *
 * Runs FASTA data through end-gap free alignment.
 */

int dp_run_egfa(DP obj) {
	size_t    i, j;
	int       v, cost[3], mval;
	DP_ACTION a;

	dp_clear(obj);

	mval = INT_MIN;

	/*
	 * Very first row is just... well... default values lol. But let's set the
	 * second row. This'll make the following loop simpler.
	 */

	for (i = 0; i < obj->w; i++)
		obj->table[i][1] = 0;

	//Go through. This time, it's even more than personal.
	for (j = 1; j < obj->h; j++) {
		//Second row to first one.
		//TODO: Flip X and Y so we can use memcpy instead.
		for (i = 0; i < obj->w; i++)
			obj->table[i][0] = obj->table[i][1];

		//First value is default.
		obj->table[0][1] = 0;

		for (i = 1; i < obj->w; i++) {
			//Generate costs and choose the lowest one
			cost[0] = obj->table[i - 1][0];
			cost[1] = obj->table[i    ][0] + obj->val_space;
			cost[2] = obj->table[i - 1][1] + obj->val_space;

			//Make cost[0] worth more depending on matching
			if (obj->s[0][j - 1] == obj->s[1][i - 1])
				cost[0] += obj->val_match;
			else
				cost[0] += obj->val_mismatch;

			//v = max(cost[0], max(cost[1], cost[2]));
			v = cost[0], a = DP_MATCH;

			if (cost[1] > v)
				v = cost[1], a = DP_SPACE1;

			if (cost[2] > v)
				v = cost[2], a = DP_SPACE2;

			//Update the maximum value
			mval = max(mval, v);

			obj->table  [i][1] = v;
			obj->actions[i][1] = a;
		}
	}

	return mval;
}

/*
 * dp_run_clear                                                            {{{2
 *
 * Clears out the DP table, setting all values to 0.
 */

void dp_clear(DP obj) {
	size_t i;

	for (i = 0; i < obj->w; i++)
		memset(obj->table[i], 0, sizeof(int) * 2);

	for (i = 0; i < obj->w; i++)
		memset(obj->actions[i], 0, sizeof(DP_ACTION) * 2);
}

/*
 * dp_free                                                                 {{{2
 *
 * Frees the DP object.
 */

void dp_free(DP obj) {
	size_t i;

	//Free strings and table
	free(obj->s[0]);
	free(obj->s[1]);

	//Free the table/actions
	for (i = 0; i < obj->w; i++) {
		free(obj->table[i]);
		free(obj->actions[i]);
		free(obj->hl[i]);
	}

	free(obj->table);
	free(obj->actions);
	free(obj->hl);

	//Free self
	free(obj);
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char ** argv) {
	size_t i, j, x, y, sz;
	int maxv;
	CN_STRING f1, f2;
	DP obj;
	PARAM_T params;

	//Argument check
	if (argc != 7) {
		fprintf(
			stderr,
			"usage: %s -qs file1 file2 match_val mismatch_val gap_val\n\n"
			"Parameter Information:\n"
			"\t-q\tQuiet(er). Stops printing headers before each section\n"
			"\t-s\tDisplay the end-gap free alignment score\n"
			"flag)\n"
			"\nThe information printed out is in the order you define the "
			"arguments. So \"-as\" will print\nthe alignment, then the score. "
			"And \"-sa\" will do the same but in reverse order.\n",
			argv[0]
		);
		return 1;
	}

	params = arg_init();

	//Parse arguments
	arg_parse(params, argv[1]);

	f1 = fasta_import_to_cn_string(argv[2]);
	f2 = fasta_import_to_cn_string(argv[3]);

	//Construct the DP table
	obj = dp_init(
		cn_string_str(f1), cn_string_str(f2),
		atoi(argv[4]), atoi(argv[5]), atoi(argv[6])
	);

	//Ok... do it.
	maxv = dp_run_egfa(obj);

	sz = strlen(argv[1]);

	for (i = 0; i < sz; i++) {
		switch (argv[1][i]) {
			case 's':
				print_header(j, params->flag_quiet, "ALIGNMENT SCORE:\n");
				printf("%d\n", maxv);
				break;
		}
	}

	//Free memory
	dp_free(obj);
	arg_free(params);
	cn_string_free(f1);
	cn_string_free(f2);

	//We're done here.
	return 0;
}
