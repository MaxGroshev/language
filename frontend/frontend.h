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
    char name[BUF_OF_64_ELEM];
};

struct prog_func_t
{
    int  line;
    char name[BUF_OF_64_ELEM];
};

//--------------------------------------------------------------------------------------------------

struct lex_stat_t
{
    int    lex_size;
    int    lex_capacity;
    struct tree_node_t* lexems;
};

//---------------------------------------PROG_LOGIC----------------------------------------------

int frontend_utils (const char* file_dir);

int prog_stat_init       (prog_data_t* prog_stat);
int prog_data_dtor       (char* buffer, prog_data_t* prog_stat);
int lexical_analysis     (char* buffer, lex_stat_t* lex_stat, prog_data_t* prog_stat);
int lexems_init          (lex_stat_t* lex_stat);
tree_node_t* rec_descent   (lex_stat_t* lex_stat, prog_data_t* prog_stat);

//--------------------------------------------------------------------------------------------------

int prog_stat_resize (prog_data_t* prog_stat);
int lexems_resize    (lex_stat_t* lex_stat);

//-------------------------------------------------------------------------------------------------------------------

int    l_strncomp   (const char* str1, const char* str2, size_t num_of_elem, int skip_space, int* pos_in_buf);
double my_strtod    (const char* str, int* pos_in_buf);
int    is_this_op   (const char* str, int code_of_op, const char* buffer, int* pos_in_buf, lex_stat_t* lex_stat);
int    is_exist_var (const char* buffer, int pos_in_buf, prog_data_t* prog_stat);

//-------------------------------------------------------------------------------------------------------------------

int add_new_var      (char* var_name, prog_data_t* prog_stat, lex_stat_t* lex_stat);

//---------------------------------------REC_DESCENT_FUNC-------------------------------------------------------------------------

tree_node_t* get_end       (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_gart_node (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_if        (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_operator  (lex_stat_t* lex_stat, prog_data_t* prog_stat);

tree_node_t* get_begin     (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_assign    (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_sign      (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_ident     (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_pm_sign   (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_num       (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_md_sign   (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_brac      (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_deg       (lex_stat_t* lex_stat, prog_data_t* prog_stat);


tree_node_t* get_comp       (lex_stat_t* lex_stat, prog_data_t* prog_stat);
int is_negative_val         (lex_stat_t* lex_stat);
