#pragma once
#include "./tree/tree.h"
#include "DSL.h"

struct prog_data_t
{
    int var_capacity;
    int func_capacity;

    int str_num  = 0;
    int func_num = 0;
    int var_num  = 0;

    struct prog_var_t* decl_vars;
    struct prog_func_t* decl_funcs;
};

struct prog_var_t
{
    int  line;
    char name;
};

struct prog_func_t
{
    int  line;
    char name;
};

//--------------------------------------------------------------------------------------------------

int prog_stat_init   (prog_data_t* prog_stat);
int prog_stat_resize (prog_data_t* prog_stat);

int prog_data_dtor   (char* buffer, prog_data_t* prog_stat);

//-------------------------------------------------------------------------------------------------------------------

int add_new_var      (char var_name, prog_data_t* prog_stat);

//---------------------------------------REC_DESCENT_FUNC-------------------------------------------------------------------------

tree_node_t* rec_descent (const char* file_dir);
tree_node_t* get_end     (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_if      (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_operator(const char* buffer, prog_data_t* prog_stat);

tree_node_t* get_begin   (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_assign  (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_sign    (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_ident   (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_pm_sign (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_num     (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_md_sign (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_brac    (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_deg     (const char* buffer, prog_data_t* prog_stat);

tree_node_t* get_line_end (const char* buffer, prog_data_t* prog_stat);
tree_node_t* get_comp     (const char* buffer, prog_data_t* prog_stat);
