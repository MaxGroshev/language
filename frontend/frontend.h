#pragma once
#include "./tree/tree.h"
#include "DSL.h"

#define BUF_OF_64_ELEM 64

//-------------------------------LEXEMS_STRUCTS----------------------------------------------------

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

//---------------------------------------UTILS_FUNC---------------------------------------------

int frontend_utils (const char* file_dir);

tree_node_t* rec_descent   (lex_stat_t* lex_stat, prog_data_t* prog_stat);

//-------------------------------------LEXEMS_STRUCT_FUNC-------------------------------------------------------

int prog_stat_init       (prog_data_t* prog_stat);
int lexems_init          (lex_stat_t*  lex_stat);
int prog_stat_resize     (prog_data_t* prog_stat);
int lexems_resize        (lex_stat_t*  lex_stat);
int prog_data_dtor       (char* buffer, prog_data_t* prog_stat);
int lex_dtor             (lex_stat_t* lex_stat);
int lexical_analysis     (char* buffer, lex_stat_t* lex_stat, prog_data_t* prog_stat);

//-------------------------------------TOKENIZATION_FUNC------------------------------------------------------------------

int    l_strncomp       (const char* str1, const char* str2, size_t num_of_elem, int skip_space, int* pos_in_buf);
double my_strtod        (const char* str, int* pos_in_buf);
int    is_this_op       (const char* str, int code_of_op, const char* buffer, int* pos_in_buf, lex_stat_t* lex_stat);
int    is_exist_var     (prog_data_t* prog_stat, const char* var_name = NULL);
int    add_new_num      (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat);
int    add_new_var      (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat);
int    add_new_func     (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat);
int    add_exist_var    (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat);

int    is_negative_val  (lex_stat_t* lex_stat);
