#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// Dip Switches                                                            {{{1
// ----------------------------------------------------------------------------

#define DEBUG 0

// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

size_t intlen(int v) {
	char *buffer;
	size_t len;

	buffer = (char *) calloc(12, sizeof(char));
	sprintf(buffer, "%d", v);
	len = strlen(buffer);
	free(buffer);

	return len;
}

int min(int a, int b) {
	return (a < b) ? a : b;
}

int max(int a, int b) {
	return (a > b) ? a : b;
}

// ----------------------------------------------------------------------------
// DP "Class" Prototypes                                                   {{{1
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
	DP_ACTION **actions;
} *DP;

//Function Prototypes
DP   dp_init       (char *, char *, int, int, int);
void dp_run        (DP);
void dp_clear      (DP);
void dp_print_table(DP);
void dp_print_align(DP);
void dp_free       (DP);

// ----------------------------------------------------------------------------
// DP Struct Functions                                                     {{{1
// ----------------------------------------------------------------------------

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

	obj->actions = (DP_ACTION **) malloc(sizeof(DP_ACTION *) * obj->w);
	for (i = 0; i < obj->w; i++)
		obj->actions[i] = (DP_ACTION *) calloc(obj->h, sizeof(DP_ACTION));

	//Return
	return obj;
}

void dp_run(DP obj) {
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

void dp_clear(DP obj) {
	size_t i;

	for (i = 0; i < obj->w; i++)
		memset(obj->table[i], 0, sizeof(int) * obj->h);

	for (i = 0; i < obj->w; i++)
		memset(obj->actions[i], 0, sizeof(DP_ACTION) * obj->h);
}

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
			printf("%*d", w, obj->table[i][j]);
		}

		printf("\n");
	}
}

void dp_print_align(DP obj) {
	size_t i, j, sz;
	char *res[2];

	//Go from bottom right to top left;
	sz = 0;
	i = obj->w - 1;
	j = obj->h - 1;

	while (i != 0 && j != 0) {
		if (i < 0 || j < 0)
			break;

		switch(obj->actions[i][j]) {
			case DP_NULL:
				return;

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

		sz++;
	}

	//Create strings and do it again
	res[0] = (char *) calloc(sz + 1, sizeof(char));
	res[1] = (char *) calloc(sz + 1, sizeof(char));

	i = obj->w - 1;
	j = obj->h - 1;

	while (i != 0 && j != 0) {
		if (i < 0 || j < 0)
			break;
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
				return;

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
	}

	printf("%s\n%s\n", res[0], res[1]);
	free(res[0]);
	free(res[1]);
}

void dp_free(DP obj) {
	size_t i;

	//Free strings and table
	free(obj->s[0]);
	free(obj->s[1]);

	//Free the table/actions
	for (i = 0; i < obj->w; i++) {
		free(obj->table[i]);
		free(obj->actions[i]);
	}

	free(obj->table);
	free(obj->actions);

	//Free self
	free(obj);
}


// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char ** argv) {
	size_t i, j;
	DP obj;

	//Argument check
	if (argc != 6) {
		fprintf(
			stderr,
			"usage: %s str1 str2 match_val mismatch_val gap_val\n",
			argv[0]
		);
		return 1;
	}

	//Construct the DP table
	obj = dp_init(
		argv[1], argv[2],
		atoi(argv[3]), atoi(argv[4]), atoi(argv[5])
	);

	//Ok... do it.
	dp_run(obj);

	dp_print_table(obj);
	dp_print_align(obj);

	//Free memory
	dp_free(obj);

	//We're done here.
	return 0;
}
