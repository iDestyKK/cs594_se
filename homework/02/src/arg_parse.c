#include "arg_parse.h"

PARAM_T arg_init() {
	PARAM_T obj = (PARAM_T) malloc(sizeof(struct param_t));

	//Default values
	obj->flag_align     = 0;
	obj->flag_dp        = 0;
	obj->flag_quiet     = 0;
	obj->flag_string    = 0;
	obj->flag_highlight = 0;

	return obj;
}

void arg_parse(PARAM_T obj, char *args) {
	size_t i, sz;
	sz = strlen(args);

	for (i = 0; i < sz; i++) {
		switch (args[i]) {
			case '-':
				//Generic. Can ignore (it's optional too btw).
				break;

			case 'a':
				obj->flag_align = 1;
				break;

			case 'm':
				obj->flag_dp = 1;
				break;

			case 'q':
				obj->flag_quiet = 1;
				break;

			case 's':
				obj->flag_string = 1;
				break;

			case 't':
				obj->flag_dp = 1;
				obj->flag_highlight = 1;
				break;

			default:
				fprintf(
					stderr,
					"Warning: Invalid option \"-%c\". Ignored.\n",
					args[i]
				);
				break;
		}
	}
}

void arg_free(PARAM_T obj) {
	free(obj);
}
