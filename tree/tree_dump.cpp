#include "tree.h"

void graph_dump (tree_node_t* tree_node)
{
    MY_ASSERT (tree_node != NULL);

    dump_graph_t graph_dump_set = {};
    init_graph (&graph_dump_set, TREE_GRAPH_DOT_DIR);
    tree_print (&graph_dump_set, tree_node);

    run_graphviz (&graph_dump_set, TREE_GRAPH_DOT_DIR, TREE_LOGS_PNG_DIR);
}

int tree_print (dump_graph_t* graph_dump_set, tree_node_t* parent)
{
    MY_ASSERT (parent != NULL);

    if (parent->node_type == TYPE_NUM)
    {
        graph_dump_set->nodes->fillcolor = "#FF8C69";
        graph_dump_set->nodes->label     = "TYPE_NUM";
        print_int_node (TREE_DUMP_SET, parent->value);
    }
    else if (parent->node_type == TYPE_VAR)
    {
        graph_dump_set->nodes->fillcolor = "#7FC7FF";
        graph_dump_set->nodes->label     = "TYPE_VAR";
        print_char_node (TREE_DUMP_SET, parent->value);
    }
    else
    {
        graph_dump_set->nodes->fillcolor = "#77DD77";
        graph_dump_set->nodes->label     = "TYPE_OP";

        switch (parent->node_type)
        {
            case OP_ADD:
                print_char_node (TREE_DUMP_SET, OP_ADD);
                break;
            case OP_SUB:
                print_char_node (TREE_DUMP_SET, OP_SUB);
                break;
            case OP_MUL:
                print_char_node (TREE_DUMP_SET, OP_MUL);
                break;
            case OP_DIV:
                print_char_node (TREE_DUMP_SET, OP_DIV);
                break;
            case OP_POW:
                print_char_node (TREE_DUMP_SET, OP_POW);
                break;
            case OP_EQ:
                print_char_node (TREE_DUMP_SET, OP_EQ);
                break;
            case CONST_EXP:
                graph_dump_set->nodes->fillcolor = "#FF8C69";
                graph_dump_set->nodes->label     = "CONST_VAL";
                print_str_node  (TREE_DUMP_SET, "e");
                break;
            case OP_SQR:
                print_str_node  (TREE_DUMP_SET, "sqr");
                break;
            case OP_LN:
                print_str_node  (TREE_DUMP_SET, "ln");
                break;
            case OP_LOG:
                print_str_node  (TREE_DUMP_SET, "log");
                break;
            case OP_SIN:
                print_str_node  (TREE_DUMP_SET, "sin");
                break;
            case OP_COS:
                print_str_node  (TREE_DUMP_SET, "cos");
                break;
            case OP_TG:
                print_str_node  (TREE_DUMP_SET, "tg");
                break;
            case OP_CTG:
                print_str_node  (TREE_DUMP_SET, "ctg");
                break;
        }
    }

    if (parent->left != NULL)
    {
        make_edge  (graph_dump_set, &parent->value, &parent->left->value, *graph_dump_set->edges);
        tree_print (graph_dump_set, parent->left);
    }
    if (parent->right != NULL)
    {
        make_edge  (graph_dump_set, &parent->value, &parent->right->value, *graph_dump_set->edges);
        tree_print (graph_dump_set, parent->right);
    }
    return 0;
}
