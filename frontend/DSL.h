#pragma once
#include "../language.h"

//-------------------------------------------------------------------------------------------------------------------------------

tree_node_t* make_gart_node ();

//---------------------------------------------------------------------------------------------------------------------

#define Copy_l copy_node (tree_node->left)
#define Copy_r copy_node (tree_node->right)
#define Dif_l  dif_node  (tree_node->left,  dif_var)
#define Dif_r  dif_node  (tree_node->right, dif_var)

//-------------------------------------------------------------------------------------------------------------------------------

tree_node_t* Add (tree_node_t* left_node, tree_node_t* right_node);
tree_node_t* Sub (tree_node_t* left_node, tree_node_t* right_node);
tree_node_t* Mul (tree_node_t* left_node, tree_node_t* right_node);
tree_node_t* Div (tree_node_t* left_node, tree_node_t* right_node);
tree_node_t* Pow (tree_node_t* left_node, tree_node_t* right_node);
tree_node_t* Ln  (tree_node_t* right_node);
tree_node_t* Sin (tree_node_t* right_node);
tree_node_t* Cos (tree_node_t* right_node);
tree_node_t* Tg  (tree_node_t* right_node);
tree_node_t* Ctg (tree_node_t* right_node);
tree_node_t* New_num   (tree_data_type value);
tree_node_t* New_var   (int node_type, char var_name);
tree_node_t* New_const (int value);

//-------------------------------------------------------------------------------------------------------------------------------

int is_arithm_op (tree_node_t* tree_node);

