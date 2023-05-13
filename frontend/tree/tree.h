#pragma once
#define DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "tree_define.h"
#include "./graph_lib/graphviz.h"
#include "../../logs/log_file.h"
#include "my_ASSERT.h"

#define BUF_OF_64_ELEM 64


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct stat;
typedef double tree_data_type;

//---------------------------------------------------TREE_STRUCT---------------------------------------------------------------------------------------------------------------------------------------------

struct tree_node_t
{
    int            node_type;
    int            code_of_error = 0;
    char           name[BUF_OF_64_ELEM];
    tree_data_type value;
    tree_node_t*   right;
    tree_node_t*   left;
};

struct tree_t
{
    tree_node_t* root = NULL;
    size_t       size = 0;
};

//---------------------------------------------------CODE_OF_NODES-------------------------------------------------------------------------------------------------------------------------------------------

enum NODE_TYPE
{
    TYPE_NUM  = 20,
    TYPE_VAR  = 120,
    TYPE_FUNC = 220,
    OP_GART_N = 59,

    //OPERATORS
    OP_IF      = 105,
    OP_BEGIN   = 106,
    OP_END     = 107,
    OP_OPEN_BR = 108,
    OP_CLOSE_BR= 109,
    OP_THEN    = 110,
    OP_PROG_END= 111,

    //MATH_OPER:
    OP_ADD  = 43,
    OP_SUB  = 45,
    OP_MUL  = 42,
    OP_DIV  = 47,
    OP_POW  = 94,

    //LOGIC_OPER
    OP_LESS     = 60,
    OP_EQ       = 61,
    OP_COMP_EQ  = 62,
    OP_ABOVE    = 63,
    OP_ABOVE_EQ = 64,
    OP_LESS_EQ  = 65,

    //MATH_FUNC:
    OP_SIN  = 50,
    OP_COS  = 51,
    OP_LN   = 52,
    OP_SQR  = 54,
    CONST_EXP = 101,
    OP_LOG  = 56,
    OP_TG   = 57,
    OP_CTG  = 58,
};

enum string_func_cond
{
    STR_NOT_SKIP_SPACE = 0,
    STR_SKIP_SPACE     = 1,
};

//---------------------------------------------------LOGS_ENUM---------------------------------------------------------------------------------------------------------------------------------------------

enum TREE_CODE_OF_PRINT
{
    T_NODE_SUC_CREATED      =  1,
    T_L_EDGE_SUC_CREATED    =  2,
    T_R_EDGE_SUC_CREATED    =  3,
    T_TREE_PRINT            =  4,
    T_NODE_SUC_DELETED      =  5,

    T_FAIL_OF_CREATING_NODE = -1,
    T_TYPE_NUM_HAS_CHILD    = -3,
    T_DIVISION_BY_ZERO      = -4,
    T_UNSUPPORTED_OPER      = -5,
};

enum TREE_SYNTAX_MES
{
    S_START_OF_BR_SEQ     = 100,
    S_NUM_READ            = 101,
    S_ADD_SUB_OP_READ     = 102,
    S_MUL_DIV_OP_READ     = 103,
    S_START_OF_NEG_SEQ    = 104,

    S_NO_CLOSED_BRACKETS   = -100,
    S_NO_NUMBER            = -101,
    S_NO_MUL_OR_DIV_OP     = -102,
    S_UNREC_SYNTAX_ERROR   = -103,
    S_ERROR                = -104,
};

//-------------------------------------------TREE_FUNC-----------------------------------------------------------------------------------------------------------------------------------------------

tree_t*      constructor        (tree_t* pine);
tree_node_t* tree_new_num_node  (tree_data_type value);
tree_node_t* tree_new_const_node(tree_data_type value);
tree_node_t* tree_new_var_node  (int node_type, char var_name = 'x');
tree_node_t* tree_new_op_node   (int node_type, tree_node_t* l_child = NULL, tree_node_t* r_child = NULL);
tree_node_t* tree_search        (tree_node_t* tree_root, tree_node_t* node);

tree_node_t* tree_link_l        (tree_node_t* parent, tree_node_t* child);
tree_node_t* tree_link_r        (tree_node_t* parent, tree_node_t* child);
tree_node_t* tree_remove        (tree_t* pine, tree_node_t* node);
tree_node_t* tree_delete        (tree_node_t* tree_node);

//------------------------------------------------------------------------------------------------------------------------

void         graph_dump         (tree_node_t* tree_node);
int          tree_print         (dump_graph_t* graph_dump_set, tree_node_t* parent);
void         write_tree_logs    (int code_of_print, tree_node_t* node = NULL, const char* file_name = nullptr, const char* func_name = nullptr, int num_of_line = 0);
void         write_extra_logs   (const char* fmt,...);
void         signal_handler     (int signal);
char*        read_file          (const char* file_dir);
void         syntax_error       (int num_of_error, const char* buffer, const char* file_name = NULL, const char* func_name = NULL, int num_of_line = 0, int node_code = 0);

int strncomp (const char* str1, const char* str2, size_t num_of_elem, int skip_space = 0);
