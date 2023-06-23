#pragma sonce
#define DEBUG
#include "../frontend/tree/tree.h"
#include "my_ASSERT.h"

//--------------------------------------------------------------------------------------------------------------------------------

int          backend_utils (const char* tree_dir);
char*        read_of_file  (const char* tree_dir);
tree_node_t* build_of_tree (char*    tree_buffer);
