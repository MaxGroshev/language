#pragma once
#define DEBUG
#include "../tree/tree.h"
#include "../logs/log_file.h"
#include "../my_str_func/str_func.h"
#include "my_ASSERT.h"
//--------------------------------------------------------------------------------------------------------

enum LABEL_STATUS
{
    NOT_MATCHED= 0,
    IS_MATCHED = 1,
};

struct prog_data_t
{
    int label;
    int cond_depth;
};


//--------------------------------------------------------------------------------------------------------------------------------

char*        read_of_file  (const char* tree_dir);
tree_node_t* build_of_tree (char* tree_buffer);
int          write_asm_code(tree_node_t* prog_tree, FILE* prog_file, prog_data_t* prog_data);

int          cond_jmp_def (int node_type, FILE* prog_file, prog_data_t* prog_data);
int          math_opr_def (int node_type, FILE* prog_file);
int          func_def     (tree_node_t* prog_tree, FILE* prog_file);

#define IS_THIS_OPER(str) l_strncomp (tree_buffer + pos_in_buf, str, strlen (str), STR_SKIP_SPACE, &pos_in_buf)
