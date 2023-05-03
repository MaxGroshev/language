#pragma once

#include "./frontend/tree/tree.h"
#include "./frontend/frontend.h"
#include "./logs/log_file.h"
#include "brackets_func.h"
#include "./frontend/DSL.h"


//---------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------

tree_node_t* dif_node   (const tree_node_t* tree_node, char dif_var);
tree_node_t* copy_node  (const tree_node_t* tree_node);
tree_node_t* simpl_func (tree_node_t* tree_node, char dif_var, int* change_flag);
tree_node_t* simpl_node (tree_node_t* tree_node, char dif_var);
