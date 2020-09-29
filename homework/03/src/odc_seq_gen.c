/*
 * Occasionally Dishonest Casino
 *
 * Description:
 *     Implements "The occasionally dishonest casino" from Durbin, Pg. 54. This
 *     simply generates a sequence. It prints the sequence of dice rolls, as
 *     well as what dice was being used for the rolls. You may filter out lines
 *     if you want to see either.
 *
 *     The program is written in an extensible manner. Dice are allowed any
 *     number of sides and may be tweaked. Seed and number of rolls are passed
 *     in as command line arguments.
 *
 * Author:
 *     Clara Nguyen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// ----------------------------------------------------------------------------
// Dice Struct                                                             {{{1
// ----------------------------------------------------------------------------

typedef enum dice_id {
	DICE_FAIR   = 0,
	DICE_UNFAIR = 1
} DICE_ID;

// ----------------------------------------------------------------------------
// Dice Struct                                                             {{{1
// ----------------------------------------------------------------------------

typedef struct dice {
	size_t       sides;
	long double *odds;
	long double  total;
} *DICE;

// ----------------------------------------------------------------------------
// Die Struct Helper Functions                                             {{{1
// ----------------------------------------------------------------------------

DICE dice_init(int sides, ...) {
	va_list valist;
	size_t i;
	DICE obj;

	obj = (DICE) malloc(sizeof(struct dice));

	obj->sides = sides;
	obj->odds  = (long double *) malloc(sizeof(long double) * sides);
	obj->total = 0.0;

	va_start(valist, sides);

	for (i = 0; i < sides; i++) {
		obj->odds[i] = va_arg(valist, long double);
		obj->total += obj->odds[i];

		//Increment by the previous cell as well.
		if (i > 0)
			obj->odds[i] += obj->odds[i - 1];
	}
	
	va_end(valist);

	return obj;
}

size_t dice_roll(DICE obj) {
	long double v;
	int i;

	//Generate number between 0 and total w/ decimal precision.
	v = (((long double) rand()) / RAND_MAX) * obj->total;

	//Figure out what it landed on
	for (i = 0; i < obj->sides; i++)
		if (v < obj->odds[i])
			break;

	return i;
}

void dice_free(DICE obj) {
	free(obj->odds);
	free(obj);
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char **argv) {
	int         rolls, i, seed;
	DICE_ID     die_index;
	char       *run, *fair;
	DICE        d[2];
	long double of, ou_a, ou_b, swp;

	//Argument Check
	if (argc != 3) {
		fprintf(stderr, "usage: %s rolls seed\n", argv[0]);
		return 1;
	}

	//Extract arguments
	rolls = atoi(argv[1]);
	seed  = atoi(argv[2]);

	//Configure odds
	of   = ((long double) 1.0) / 6;
	ou_a = ((long double) 1.0) / 10;
	ou_b = ((long double) 1.0) / 2;

	//Implement the 
	d[0] = dice_init(6, of  , of  , of  , of  , of  , of  );
	d[1] = dice_init(6, ou_a, ou_a, ou_a, ou_a, ou_a, ou_b);

	//Commence rolling
	srand(seed);
	die_index = DICE_FAIR;
	run  = (char *) malloc(sizeof(char) * (rolls + 1));
	fair = (char *) malloc(sizeof(char) * (rolls + 1));

	for (i = 0; i < rolls; i++) {
		//According to page 54, switch die before rolling.
		swp = ((long double) rand()) / RAND_MAX;

		if (die_index == DICE_FAIR && swp < 0.05)
			die_index = DICE_UNFAIR;
		else
		if (die_index == DICE_UNFAIR && swp < 0.1)
			die_index = DICE_FAIR;

		//Now then...
		run [i] = '1' + dice_roll(d[die_index]);
		fair[i] = (die_index == DICE_FAIR) ? 'F' : 'L';
	}

	//Null termination of final character
	run[i] = fair[i] = 0;

	//Print out both the rolls and the die used.
	printf("%s\n%s\n", run, fair);

	//Clean up. We're done here.
	dice_free(d[0]);
	dice_free(d[1]);
	free(run);
	free(fair);

	return 0;
}
