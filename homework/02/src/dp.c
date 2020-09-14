#include "dp.h"

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

	obj->table = (int **) malloc(sizeof(int *) * obj->w);
	for (i = 0; i < obj->w; i++)
		obj->table[i] = (int *) calloc(obj->h, sizeof(int));

	obj->hl = (char **) malloc(sizeof(char *) * obj->w);
	for (i = 0; i < obj->w; i++)
		obj->hl[i] = (char *) calloc(obj->h, sizeof(char));

	obj->actions = (DP_ACTION **) malloc(sizeof(DP_ACTION *) * obj->w);
	for (i = 0; i < obj->w; i++)
		obj->actions[i] = (DP_ACTION *) calloc(obj->h, sizeof(DP_ACTION));

	//Return
	return obj;
}

/*
 * dp_run_global                                                           {{{2
 *
 * Runs FASTA data through global alignment.
 */

void dp_run_global(DP obj) {
	size_t    i, j;
	int       v, cost[3];
	DP_ACTION a;

	dp_clear(obj);

	//Set default values
	for (i = 0; i < obj->w; i++)
		obj->table[i][0] = obj->val_space * i;

	for (i = 0; i < obj->h; i++)
		obj->table[0][i] = obj->val_space * i;

	//Go through. This time, it's personal.
	for (j = 1; j < obj->h; j++) {
		for (i = 1; i < obj->w; i++) {
			//Generate costs and choose the lowest one
			cost[0] = obj->table[i - 1][j - 1];
			cost[1] = obj->table[i    ][j - 1] + obj->val_space;
			cost[2] = obj->table[i - 1][j    ] + obj->val_space;

			//Make cost[0] worth more depending on matching
			if (obj->s[0][j - 1] == obj->s[1][i - 1])
				cost[0] += obj->val_match;
			else
				cost[0] += obj->val_mismatch;

#if DEBUG == 1
			printf("CONSIDER (%d, %d): %d %d %d", i, j, cost[0], cost[1], cost[2]);
#endif

			//v = max(cost[0], max(cost[1], cost[2]));
			v = cost[0], a = DP_MATCH;

			if (cost[1] > v)
				v = cost[1], a = DP_SPACE1;

			if (cost[2] > v)
				v = cost[2], a = DP_SPACE2;

#if DEBUG == 1
			printf(" -> %d\n", v);
#endif

			obj->table  [i][j] = v;
			obj->actions[i][j] = a;
		}
	}
}

/*
 * dp_run_local                                                            {{{2
 *
 * Runs FASTA data through local alignment.
 */

void dp_run_local(DP obj) {
	size_t    i, j;
	int       v, cost[3];
	DP_ACTION a;

	dp_clear(obj);

	//Set default values
	for (i = 0; i < obj->w; i++)
		obj->table[i][0] = 0;

	for (i = 0; i < obj->h; i++)
		obj->table[0][i] = 0;

	//Go through. This time, it's personal.
	for (j = 1; j < obj->h; j++) {
		for (i = 1; i < obj->w; i++) {
			//Generate costs and choose the lowest one
			cost[0] = obj->table[i - 1][j - 1];
			cost[1] = obj->table[i    ][j - 1] + obj->val_space;
			cost[2] = obj->table[i - 1][j    ] + obj->val_space;

			//Make cost[0] worth more depending on matching
			if (obj->s[0][j - 1] == obj->s[1][i - 1])
				cost[0] += obj->val_match;
			else
				cost[0] += obj->val_mismatch;

#if DEBUG == 1
			printf("CONSIDER (%d, %d): %d %d %d", i, j, cost[0], cost[1], cost[2]);
#endif

			//v = max(cost[0], max(cost[1], cost[2]));
			v = cost[0], a = DP_MATCH;

			if (cost[1] > v)
				v = cost[1], a = DP_SPACE1;

			if (cost[2] > v)
				v = cost[2], a = DP_SPACE2;

			if (v < 0)
				v = 0;

#if DEBUG == 1
			printf(" -> %d\n", v);
#endif

			obj->table  [i][j] = v;
			obj->actions[i][j] = a;
		}
	}
}

/*
 * dp_run_egfa                                                             {{{2
 *
 * Runs FASTA data through end-gap free alignment.
 */

void dp_run_egfa(DP obj) {
	size_t    i, j;
	int       v, cost[3];
	DP_ACTION a;

	dp_clear(obj);

	//Set default values
	for (i = 0; i < obj->w; i++)
		obj->table[i][0] = 0;

	for (i = 0; i < obj->h; i++)
		obj->table[0][i] = 0;

	//Go through. This time, it's personal.
	for (j = 1; j < obj->h; j++) {
		for (i = 1; i < obj->w; i++) {
			//Generate costs and choose the lowest one
			cost[0] = obj->table[i - 1][j - 1];
			cost[1] = obj->table[i    ][j - 1] + obj->val_space;
			cost[2] = obj->table[i - 1][j    ] + obj->val_space;

			//Make cost[0] worth more depending on matching
			if (obj->s[0][j - 1] == obj->s[1][i - 1])
				cost[0] += obj->val_match;
			else
				cost[0] += obj->val_mismatch;

#if DEBUG == 1
			printf("CONSIDER (%d, %d): %d %d %d", i, j, cost[0], cost[1], cost[2]);
#endif

			//v = max(cost[0], max(cost[1], cost[2]));
			v = cost[0], a = DP_MATCH;

			if (cost[1] > v)
				v = cost[1], a = DP_SPACE1;

			if (cost[2] > v)
				v = cost[2], a = DP_SPACE2;

#if DEBUG == 1
			printf(" -> %d\n", v);
#endif

			obj->table  [i][j] = v;
			obj->actions[i][j] = a;
		}
	}
}

/*
 * dp_run_clear                                                            {{{2
 *
 * Clears out the DP table, setting all values to 0.
 */

void dp_clear(DP obj) {
	size_t i;

	for (i = 0; i < obj->w; i++)
		memset(obj->table[i], 0, sizeof(int) * obj->h);

	for (i = 0; i < obj->w; i++)
		memset(obj->actions[i], 0, sizeof(DP_ACTION) * obj->h);
}

/*
 * dp_backtrack                                                            {{{2
 *
 * Performs a backtrack on a processed DP (a "dp_run_*" command must be used
 * prior) to highlight where the most optimal alignment occurred. Running the
 * "dp_print_table" function after this will have the most optimal alignment
 * print in red.
 *
 * Because multiple algorithms are supported here, the user is given the
 * flexibility to choose where to start the backtrack via (start_x, start_y).
 * They are also allowed to choose whether to force it to seek (0, 0) once one
 * axis coordinate has reached 0.
 *
 * For Global Alignment, "force_start" should be set to 1, and starting x, y
 * coordinates should be set to the bottom right of the table. For all others,
 * "force_start" should be set to 0, and starting x, y coordinates should be
 * set to the location of the highest value in the table.
 */

void dp_backtrack(DP obj, size_t start_x, size_t start_y, size_t force_start) {
	int i, j, force;
	char *res[2];

	//Go from bottom right to top left;
	force = 0;
	i = start_x;
	j = start_y;

	obj->hl[i][j] = 1;

	while (!force) {
		switch(obj->actions[i][j]) {
			case DP_NULL:
				force = 1;
				break;

			case DP_MATCH:
				i--, j--;
				break;

			case DP_SPACE1:
				j--;
				break;

			case DP_SPACE2:
				i--;
				break;
		}

		if (i < 0 || j < 0)
			break;

		obj->hl[i][j] = 1;
	}

	//If at a null spot, we can try to approach the top-left corner
	if (i != -1 && j != -1 && force_start == 1) {
		//Go left
		for (; i > 0; i--)
			obj->hl[i][j] = 1;

		//Go up
		for (; j > 0; j--)
			obj->hl[i][j] = 1;

		obj->hl[0][0] = 1;
	}
}

/*
 * dp_print_table                                                          {{{2
 *
 * Prints out the DP table. If "dp_backtrack" was called before this, the
 * optimal path will be shown in red.
 */

void dp_print_table(DP obj) {
	size_t i, j, w, tmp;

	//Figure out the maximum size to properly draw the table
	w = 1;

	for (i = 0; i < obj->w; i++) {
		for (j = 0; j < obj->h; j++) {
			tmp = intlen(obj->table[i][j]);
			w = (tmp > w) ? tmp : w;
		}
	}

	w++;

	//Print out top row's characters
	printf("    ");

	for (i = 0; i < obj->w; i++)
		printf("%*c", w, (i > 0) ? obj->s[1][i - 1] : ' ');

	printf("\n");

	//Top Border
	printf("  +");

	for (i = 0; i < (obj->w * w) + 1; i++)
		printf("-");

	printf("\n");

	//Grid thing
	for (j = 0; j < obj->h; j++) {
		//Print the character
		printf("%c | ", (j > 0) ? obj->s[0][j - 1] : ' ');

		for (i = 0; i < obj->w; i++) {
			//Ditto
			if (obj->hl[i][j])
				printf(RED "%*d" RESET, w, obj->table[i][j]);
			else
				printf("%*d", w, obj->table[i][j]);
		}

		printf("\n");
	}
}

/*
 * dp_print_align                                                          {{{2
 *
 * Prints out the alignment strings based on table data. Blanks will show as
 * "-" to force alignment of the strings visually.
 */

void dp_print_align(DP obj, size_t start_x, size_t start_y, size_t force_start) {
	int i, j, force, dec_amt;
	size_t sz;
	char *res[2];

	//Go from bottom right to top left;
	sz = 0;
	force = 0;
	i = start_x;
	j = start_y;

	while (!force) {
		if (!force_start) {
			if (i <= 0 || j <= 0)
				break;
		}

		switch(obj->actions[i][j]) {
			case DP_NULL:
				force = 1;
				break;

			case DP_MATCH:
				i--, j--;
				break;

			case DP_SPACE1:
				j--;
				break;

			case DP_SPACE2:
				i--;
				break;
		}

		if (i < 0 || j < 0)
			break;

		sz++;
	}

	//If at a null spot, we can try to approach the top-left corner
	if (i != -1 && j != -1 && force_start == 1) {
		dec_amt = 1;

		//Go left
		if (i > 0) {
			for (; i > 0; i--, sz++);
			//dec_amt++;
		}

		//Go up
		if (j > 0) {
			for (; j > 0; j--, sz++);
			//dec_amt++;
		}

		//Deduct accordingly
		sz -= dec_amt;
	}

	//Create strings and do it again
	res[0] = (char *) calloc(sz + 1, sizeof(char));
	res[1] = (char *) calloc(sz + 1, sizeof(char));

	force = 0;
	i = start_x;
	j = start_y;

	while (!force) {
		/*
		if (i <= 0 || j <= 0)
			break;
		*/

		sz--;

#if DEBUG == 1
		printf(
			"AT (%d, %d) %c %c (%d): %d\n",
			i, j,
			obj->s[0][j - 1], obj->s[1][i - 1],
			obj->table[i][j],
			obj->actions[i][j]
		);
#endif

		switch(obj->actions[i][j]) {
			case DP_NULL:
				force = 1;
				break;

			case DP_MATCH:
				res[0][sz] = obj->s[0][j - 1];
				res[1][sz] = obj->s[1][i - 1];
				i--, j--;
				break;

			case DP_SPACE1:
				res[0][sz] = obj->s[0][j - 1];
				res[1][sz] = '-';
				j--;
				break;

			case DP_SPACE2:
				res[0][sz] = '-';
				res[1][sz] = obj->s[1][i - 1];
				i--;
				break;
		}

		if (i < 0 || j < 0)
			break;
	}

	//Ditto. If at a null spot, we can try to approach the top-left corner
	if (i != -1 && j != -1 && force_start == 1) {
		//Go left
		for (; i > 0; i--, sz--) {
			res[0][sz] = '-';
			res[1][sz] = obj->s[1][i - 1];
		}

		//Go up
		for (; j > 0; j--, sz--) {
			res[0][sz] = obj->s[0][j - 1];
			res[1][sz] = '-';
		}
	}

	printf("%s\n%s\n", res[0], res[1]);
	free(res[0]);
	free(res[1]);
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
