#pragma once
#include "language.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

double       tree_eval      (tree_node_t* tree_node);
int          write_brackets (FILE* br_write, tree_node_t* tree_node);
tree_node_t* read_brackets  (FILE* br_read,  tree_node_t* tmp_node = NULL);


