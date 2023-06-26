#pragma once
#include "frontend.h"

//---------------------------------------REC_DESCENT_FUNC-------------------------------------------------------------------------

tree_node_t* get_gart_node (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_func      (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_if        (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_operator  (lex_stat_t* lex_stat, prog_data_t* prog_stat);

tree_node_t* get_begin     (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_assign    (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_comma     (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_ident     (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_pm_sign   (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_num       (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_md_sign   (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_brac      (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_deg       (lex_stat_t* lex_stat, prog_data_t* prog_stat);
tree_node_t* get_comp      (lex_stat_t* lex_stat, prog_data_t* prog_stat);
