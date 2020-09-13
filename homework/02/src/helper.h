#ifndef __HELPER__
#define __HELPER__

//CNDS (Clara Nguyen's Data Structures)
#include "../lib/cnds/cn_string.h"
#include "../lib/cnds/cn_cmp.h"
#include "../lib/cnds/cn_map.h"

// ----------------------------------------------------------------------------
// Helper Function Prototypes                                              {{{1
// ----------------------------------------------------------------------------

void      destruct_key             (CNM_NODE *node);
CN_STRING fasta_import_to_cn_string(const char *);

#endif
