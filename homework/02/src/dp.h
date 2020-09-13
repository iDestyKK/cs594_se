#ifndef __DP_HELPER__
#define __DP_HELPER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

size_t intlen(int);
int min(int, int);
int max(int, int);

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
void dp_run_global (DP);
void dp_clear      (DP);
void dp_backtrack  (DP);
void dp_print_table(DP);
void dp_print_align(DP);
void dp_free       (DP);

#endif
