/*
 * Allele Counter
 *
 * Description:
 *     Just chuck strings into a map and print out how many unique keys were
 *     inserted. Honestly I'm not sure how much more simpler this can be...
 *
 * Author:
 *     Clara Nguyen
 */

//C Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_cmp.h"
#include "../lib/cnds/cn_map.h"

// ----------------------------------------------------------------------------
// CNDS Configuration Functions                                            {{{1
// ----------------------------------------------------------------------------

/*
 * Map destructor for freeing a C-String key in a CN_Map.
 */

void destruct_key(CNM_NODE *node) {
	char *v = *(char **) node->key;

	if (v != NULL)
		free(v);
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char **argv) {
	CN_MAP       alleles;
	CNM_ITERATOR it;
	int          i;
	char        *dup, dummy;

	//Treat the map like a set
	alleles = cn_map_init(char *, char, cn_cmp_cstr);
	cn_map_set_func_destructor(alleles, destruct_key);

	dummy = 0;

	//Every argument past the program paramaters will be inserted into CN_Map
	for (i = 1; i < argc; i++) {
		//Ensure no duplicates
		cn_map_find(alleles, &it, &argv[i]);

		if (cn_map_at_end(alleles, &it)) {
			//Insertion
			dup = strdup(argv[i]);
			cn_map_insert(alleles, &dup, &dummy);
		}
	}

	//Print out the map size
	printf("%d\n", cn_map_size(alleles));

	//Clean up. We're done here.
	cn_map_free(alleles);

	return 0;
}
