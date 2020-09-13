#ifndef __ARG_PARSE__
#define __ARG_PARSE__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct param_t {
	unsigned char flag_align,
	              flag_dp,
	              flag_quiet,
	              flag_string,
	              flag_highlight;
} *PARAM_T;

PARAM_T arg_init();
void arg_parse(PARAM_T, char *);
void arg_free(PARAM_T);

#endif
