#pragma once
#include "../tree/tree.h"
#include "../logs/log_file.h"
#include "../my_str_func/str_func.h"

#define BUF_OF_64_ELEM 64

//-------------------------------LEXEMS_STRUCTS----------------------------------------------------

struct prog_data_t
{
    int var_capacity;
    int func_capacity;

    int str_num  = 0;
    int func_num;
    int var_num;

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


int          print_standart   (prog_data_t* prog_stat, tree_node_t* tree_node);
tree_node_t* rec_descent      (lex_stat_t* lex_stat, prog_data_t* prog_stat);
int          lexical_analysis (char* buffer, lex_stat_t* lex_stat, prog_data_t* prog_stat);

//------------------------------------PRINT_OF_STANDART------------------------------------------------

tree_node_t* build_std_tree (prog_data_t* prog_stat, FILE* lang_std, tree_node_t* tree_node);

//-------------------------------------LEXEMS_STRUCT_FUNC-------------------------------------------------------

int prog_stat_init       (prog_data_t* prog_stat);
int lexems_init          (lex_stat_t*  lex_stat);
int prog_stat_resize     (prog_data_t* prog_stat);
int lexems_resize        (lex_stat_t*  lex_stat);
int prog_data_dtor       (char* buffer, prog_data_t* prog_stat);
int lex_dtor             (lex_stat_t* lex_stat);

//-------------------------------------TOKENIZATION_FUNC------------------------------------------------------------------

int    is_this_op       (const char* str, int code_of_op, const char* buffer, int* pos_in_buf, lex_stat_t* lex_stat);
int    is_exist_name    (prog_data_t* prog_stat, const char* name, int type);
int    add_std_lib_func (lex_stat_t* lex_stat, int code_of_prog);
int    add_new_num      (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat);
int    add_new_var      (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat);
int    add_new_func     (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat);
int    def_var_or_func  (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat);
int    add_exist_var    (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat, int num_of_var);
int    add_exist_func   (char* buffer, int* pos_in_buf, prog_data_t* prog_stat, lex_stat_t* lex_stat, int num_of_func);


int    is_negative_val  (lex_stat_t* lex_stat);
