/*
 * Common Functions Helper
 *
 * Description:
 *     Contains common functions which are shared between multiple programs in
 *     this homework assignment.
 *
 * Author:
 *     Clara Nguyen
 */

#ifndef __CS594_HW1_HELPER__
#define __CS594_HW1_HELPER__

//C Includes
#include <stdio.h>
#include <stdlib.h>

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"
#include "../lib/cnds/cn_cmp.h"
#include "../lib/cnds/cn_map.h"

// ----------------------------------------------------------------------------
// Structs                                                                 {{{1
// ----------------------------------------------------------------------------

/* struct for holding pair of size_t and long double */
typedef struct {
	size_t      freq;
	long double prob;
} freq_pair;

// ----------------------------------------------------------------------------
// Helper Function Prototypes                                              {{{1
// ----------------------------------------------------------------------------

void      destruct_key             (CNM_NODE *node);
CN_STRING fasta_import_to_cn_string(const char *);
CN_MAP    report_freq              (CN_STRING, const char *, size_t, size_t);
void      freq_analyse             (CN_MAP, CN_STRING, const char *, size_t,
                                    size_t, size_t, char *);

#endif
