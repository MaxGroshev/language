#pragma once
#include "./tree/tree.h"
#include "DSL.h"

#define BUF_OF_64_ELEM 64

struct prog_data_t
{
    int var_capacity;
    int func_capacity;

    int str_num  = 0;
    int func_num = 0;
    int var_num  = 0;

    struct prog_var_t*  decl_vars;
    struct prog_func_t* decl_funcs;
};

struct prog_var_t
{
    int  line;
    const char* name;
};

struct prog_func_t
{
    int  line;
    const char* name;
};

//--------------------------------------------------------------------------------------------------

struct lex_stat_t
{
    int    lex_size;
    int    lex_capacity;
    struct tree_node_t* lexems;
};

//--------------------------------------------------------------------------------------------------

int prog_stat_init   (prog_data_t* prog_stat);
int prog_stat_resize (prog_data_t* prog_stat);

int prog_data_dtor   (char* buffer, prog_data_t* prog_stat);


int lexical_analysis (char* buffer, prog_data_t* prog_stat, lex_stat_t* lex_stat);
int lexems_init      (lex_stat_t* lex_stat);
int lexems_resize    (lex_stat_t* lex_stat);

//-------------------------------------------------------------------------------------------------------------------

int    l_strncomp (const char* str1, const char* str2, size_t num_of_elem, int skip_space, int* pos_in_buf);
double my_strtod  (const char* str, int* pos_in_buf);
int    is_this_op (const char* str, int code_of_op, const char* buffer, int* pos_in_buf, lex_stat_t* lex_stat);

//-------------------------------------------------------------------------------------------------------------------

int add_new_var      (char* var_name, prog_data_t* prog_stat, lex_stat_t* lex_stat);

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
tree_node_t* get_area_end (const char* buffer, prog_data_t* prog_stat);

tree_node_t* get_comp     (const char* buffer, prog_data_t* prog_stat);
