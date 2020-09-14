//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg_parse.h"
#include "helper.h"
#include "dp.h"

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"

// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

//Shortcut macro for parameter printing stuff
#define print_header(counter, flag, str) \
	if (counter++ > 0) printf("\n"); \
	if (!flag) printf("%s", str);

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char ** argv) {
	size_t i, j, sz;
	CN_STRING f1, f2;
	DP obj;
	PARAM_T params;

	//Argument check
	if (argc != 7) {
		fprintf(
			stderr,
			"usage: %s -amqst file1 file2 match_val mismatch_val gap_val\n\n"
			"Parameter Information:\n"
			"\t-a\tDisplay strings post-alignment\n"
			"\t-m\tDisplay the DP table\n"
			"\t-q\tQuiet(er). Stops printing headers before each section\n"
			"\t-s\tDisplay the global alignment score\n"
			"\t-t\tHighlight the traceback in red via ANSI (enables \"-m\" "
			"flag)\n"
			"\nThe information printed out is in the order you define the "
			"arguments. So \"-am\" will print\nthe alignment, then the DP "
			"table. And \"-ma\" will do the same but in reverse order.\n",
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
	dp_run_global(obj);

	//Ok, print out whatever was specified
	sz = strlen(argv[1]);
	j = 0;

	for (i = 0; i < sz; i++) {
		switch (argv[1][i]) {
			case 'a':
				print_header(j, params->flag_quiet, "STRING ALIGNMENT:\n");
				dp_print_align(obj, obj->w - 1, obj->h - 1, 1);
				break;

			case 'm':
				if (params->flag_highlight)
					dp_backtrack(obj, obj->w - 1, obj->h - 1, 1);

				print_header(j, params->flag_quiet, "DP TABLE:\n");
				dp_print_table(obj);
				break;

			case 's':
				print_header(j, params->flag_quiet, "ALIGNMENT SCORE:\n");
				printf("%d\n", obj->table[obj->w - 1][obj->h - 1]);
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
