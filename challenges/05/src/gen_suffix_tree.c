#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_vec.h"
#include "../lib/cnds/cn_cmp.h"
#include "../lib/cnds/cn_map.h"
#include "../lib/cnds/cn_string.h"

// ----------------------------------------------------------------------------
// Helper Functions                                                        {{{1
// ----------------------------------------------------------------------------

/*
 * strsim
 *
 * Goes through each string until a different character is found. Reports how
 * many characters from the start of both strings are the same.
 */

size_t strsim(const char *s1, const char *s2) {
	size_t i, l[2];

	l[0] = strlen(s1);
	l[1] = strlen(s2);

	for (i = 0; i < l[0] && i < l[1]; i++)
		if (s1[i] != s2[i])
			break;

	return i;
}

/*
 * show_help
 *
 * Shows help prompt.
 */

void show_help(char *path) {
	printf("usage: %s -ahlt [word1 [word2 [...]]\n\n", path);

	printf(
		"%s\n%s\n\n%s\n",
		"Suffix Tree/Array Generator by Clara Nguyen.",
		"Takes as many strings as possible, generates a suffix tree, and "
		"prints out data\nthat the user specifies. See arguments for more "
		"information.",
		"Parameters:"
	);

	printf(
		"  -a\t%s\n",
		"Displays the Suffix Array"
	);

	printf(
		"  -h\t%s\n",
		"Shows this help prompt (And terminates the program)"
	);

	printf(
		"  -l\t%s\n",
		"Displays the LCP Array"
	);

	printf(
		"  -t\t%s\n",
		"Displays the Suffix Tree"
	);
}

// ----------------------------------------------------------------------------
// Suffix Tree Structs                                                     {{{1
// ----------------------------------------------------------------------------

typedef struct snode {
	char          data;
	struct snode *parent;
	int           id;
	CN_MAP        children;
} *SNODE;

typedef struct stree {
	int   words;
	SNODE sentinel;
} *STREE;

//Prototypes (Syntax Tree)
STREE  stree_init();
void   stree_insert(STREE, char *);
void   stree_dump(STREE);
CN_VEC stree_array_to_cn_vec(STREE);
void   stree_free(STREE);

//Prototypes (Syntax Tree Node)
SNODE  stree_node_init();
void   stree_node_insert(STREE, SNODE, char *, size_t, size_t);
void   stree_node_dump(SNODE, size_t);
void   stree_node_array_to_cn_vec(SNODE, CN_VEC);
void   stree_node_recursive_free(SNODE);

// ----------------------------------------------------------------------------
// Suffix Tree Function Definitions                                        {{{1
// ----------------------------------------------------------------------------

STREE stree_init() {
	STREE obj = (STREE) malloc(sizeof(struct stree));

	//Sentinel node defaults as well.
	obj->sentinel = stree_node_init();
	obj->words    = 0;

	return obj;
}

void stree_insert(STREE obj, char *str) {
	size_t len, i;

	len = strlen(str);

	//Unleash recursion.
	stree_node_insert(obj, obj->sentinel, str, 0, len);
}

void stree_dump(STREE obj) {
	stree_node_dump(obj->sentinel, 0);
}

CN_VEC stree_array_to_cn_vec(STREE obj) {
	CN_VEC arr;

	arr = cn_vec_init(int);

	stree_node_array_to_cn_vec(obj->sentinel, arr);

	return arr;
}

void stree_free(STREE obj) {
	stree_node_recursive_free(obj->sentinel);

	free(obj);
}

// ----------------------------------------------------------------------------
// Suffix Tree Node Function Definitions                                   {{{1
// ----------------------------------------------------------------------------

SNODE stree_node_init() {
	SNODE obj = (SNODE) calloc(1, sizeof(struct snode));

	//Default values
	obj->data     = 0;
	obj->id       = -1;
	obj->parent   = NULL;
	obj->children = cn_map_init(char, SNODE, cn_cmp_char);

	return obj;
}

void stree_node_insert(STREE t, SNODE obj, char *str, size_t i, size_t len) {
	CNM_ITERATOR it;
	SNODE n;
	char c;

	c = (i != len) ? str[i] : '$';

	cn_map_find(obj->children, &it, &c);

	if (cn_map_at_end(obj->children, &it)) {
		n = stree_node_init();
		n->data = c;
		n->parent = obj;

		//We hit terminator for new word. Slap the ID in.
		if (i == len)
			n->id = t->words++;

		cn_map_insert(obj->children, &c, &n);
	}
	else
		n = cn_map_iterator_value(&it, SNODE);

	//Carry on
	if (i + 1 <= len)
		stree_node_insert(t, n, str, i + 1, len);
}

void stree_node_dump(SNODE obj, size_t level) {
	CNM_ITERATOR it;

	if (obj->id != -1)
		printf("%*s[%c] (%d)\n", level * 2, " ", obj->data, obj->id);
	else
		printf("%*s[%c]\n", level * 2, " ", obj->data);

	cn_map_traverse(obj->children, &it) {
		stree_node_dump(cn_map_iterator_value(&it, SNODE), level + 1);
	}
}

void stree_node_array_to_cn_vec(SNODE obj, CN_VEC arr) {
	CNM_ITERATOR it;

	if (obj->id != -1)
		cn_vec_push_back(arr, &obj->id);

	cn_map_traverse(obj->children, &it) {
		stree_node_array_to_cn_vec(cn_map_iterator_value(&it, SNODE), arr);
	}
}

void stree_node_recursive_free(SNODE obj) {
	CNM_ITERATOR it;

	cn_map_traverse(obj->children, &it) {
		stree_node_recursive_free(cn_map_iterator_value(&it, SNODE));
	}

	//Release CNDS values in node
	cn_map_free(obj->children);

	//Free self
	free(obj);
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char **argv) {
	size_t i, asz;
	CN_VEC arr;

	//Argument Check
	if (argc < 3) {
		if (argc == 2) {
			//Check for 'h' flag. If it's hit, don't even bother doing anything.
			asz = strlen(argv[1]);

			for (i = 0; i < asz; i++) {
				if (argv[1][i] == 'h') {
					show_help(argv[0]);
					return 0;
				}
			}
		}

		fprintf(stderr, "usage: %s -ahlt [word1 [word2 [...]]\n", argv[0]);
		exit(1);
	}

	STREE tree = stree_init();

	//Go through arguments and inject into tree
	for (i = 2; i < argc; i++)
		stree_insert(tree, argv[i]);

	//Blank string as well
	stree_insert(tree, "");

	//Print out data based on arguments given
	asz = strlen(argv[1]);

	for (i = 0; i < asz; i++) {
		switch (argv[1][i]) {
			case '-':
				break;

			case 'a':
				//Suffix Array
				arr = stree_array_to_cn_vec(tree);

				int *ii; //Iterator

				printf("SUFFIX ARRAY:\n[ ");

				cn_vec_traverse(arr, ii) {
					printf("%d ", *ii);
				}

				printf("]\n");

				cn_vec_free(arr);
				break;

			case 'l':
				//LCP Array
				arr = stree_array_to_cn_vec(tree);

				//(Safely) assume LCP[0] is 0. Rest, compute.
				printf("LCP ARRAY:\n[ 0 ");

				for (i = 1; i < cn_vec_size(arr) - 1; i++) {
					printf(
						"%d ",
						strsim(
							argv[2 + cn_vec_get(arr, int, i)    ],
							argv[2 + cn_vec_get(arr, int, i + 1)]
						)
					);
				}

				printf("]\n");

				cn_vec_free(arr);
				break;

			case 't':
				//Suffix Tree
				printf("SUFFIX TREE:\n");
				stree_dump(tree);
				break;
		}

		//Give the next thing to display some space
		if (i < asz - 1 && i != 0)
			printf("\n");
	}

	stree_free(tree);

	//We're done here.
	return 0;
}
