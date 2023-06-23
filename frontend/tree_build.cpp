#include "frontend.h"

int print_standart (prog_data_t* prog_stat, tree_node_t* tree_node )
{
    FILE* lang_std = fopen ("./tree_stand/std_format.ast", "w");
    MY_ASSERT (lang_std != NULL);
    printf ("%d\n", tree_node->node_type);
    build_logic_tree (prog_stat, lang_std, tree_node);

    fclose (lang_std);
    return 0;
}

//==============================================================================================================

tree_node_t* build_logic_tree (prog_data_t* prog_stat, FILE* lang_std, tree_node_t* tree_node)
{
    MY_ASSERT (tree_node != NULL && lang_std != NULL)

    fprintf (lang_std, "{ ");

    switch (tree_node->node_type)
    {
        case TYPE_NUM:
            fprintf (lang_std, "%lg ", tree_node->value);
            break;
        case TYPE_VAR:
            fprintf (lang_std, "var ", tree_node->name); //Just for a time: improve work with vars
            //if (tree)
            break;
        case TYPE_FUNC:
        {
            if (strcmp ("meow", tree_node->name) == 0)
            {
                fprintf (lang_std, "#%s ",tree_node->name);
                break;
            }
            fprintf (lang_std, "func_%s ",tree_node->name);
            break;
        }
	    case OP_GART_N:     fprintf (lang_std, "; ");      break;
        case OP_BEGIN:      fprintf (lang_std, "; ");      break;
        case OP_END:        fprintf (lang_std, "; ");      break;
        case OP_IF:         fprintf (lang_std, "if ");     break;
        case OP_COMP_EQ:    fprintf (lang_std, "== ");     break;
        case OP_ADD:        fprintf (lang_std, "+ ");      break;
        case OP_EQ:         fprintf (lang_std, "= ");      break;
        case OP_RETURN:     fprintf (lang_std, "return "); break;
    }
    if (tree_node->left != NULL)
    {
        build_logic_tree (prog_stat, lang_std, tree_node->left);
    }
    else fprintf (lang_std, "{ nil } ");
    if (tree_node->right != NULL)
    {
        build_logic_tree (prog_stat, lang_std, tree_node->right);
    }
    else fprintf (lang_std, "{ nil } ");
    fprintf (lang_std, "}");
    printf ("%d\n", tree_node->node_type);
    return 0;

}

//-------------------------------------------------------------------------------------------------------------
