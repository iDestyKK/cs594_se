#include "helper.h"

/*
 * Destructor to free the C-String key in a CN_Map.
 */

void destruct_key(CNM_NODE *node) {
	char *v = *(char **) node->key;

	if (v != NULL)
		free(v);
}

/*
 * fasta_import_to_cn_string
 *
 * Imports a fasta file into a CN_String, if possible. Program exits with an
 * error message if such a file doesn't exist.
 */

CN_STRING fasta_import_to_cn_string(const char *path) {
	FILE    *fp;
	char    *line;
	ssize_t  nread;
	size_t   len, sz, i;
	CN_STRING str;

	line = NULL;

	//Open training file for reading
	fp = fopen(path, "r");
	if (fp == NULL) {
		fprintf(stderr, "fatal: Failed to open \"%s\": ", path);
		perror("");
		exit(2);
	}

	str = cn_string_init();

	//Read in lines from the training data
	while (nread = getline(&line, &len, fp) != -1) {
		sz = strlen(line);

		//If a comment, ignore it
		if (sz > 0 && line[0] == '>') {
			free(line);
			line = NULL;
			continue;
		}

		//Erase newline & carriage return if possible
		for (i = 0; i < 2; i++)
			if (line[sz - 1] == '\n' || line[sz - 1] == '\r')
				line[sz -- - 1] = 0;

		//Change "N" into "A" if possible
		for (i = 0; i < sz; i++)
			if (line[i] == 'N')
				line[i] = 'A';

		//Add to the sequence string
		cn_string_concat_from_cstr(str, line);

		//Clean up
		free(line);
		line = NULL;
	}

	if (line)
		free(line);

	fclose(fp);

	return str;
}
