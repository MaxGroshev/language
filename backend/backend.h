#pragma sonce
#define DEBUG
//#include "../frontend/tree/tree.h"
#include "../frontend/frontend.h"
#include "my_ASSERT.h"

//---------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------

int          backend_utils (const char* tree_dir);
char*        read_of_file  (const char* tree_dir);
tree_node_t* build_of_tree (char* tree_buffer);
int          write_asm_code(tree_node_t* prog_tree, FILE* prog_file);

#define IS_THIS_OPER(str) l_strncomp (tree_buffer + pos_in_buf, str, strlen (str), STR_SKIP_SPACE, &pos_in_buf)
