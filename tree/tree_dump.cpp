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
        print_val_node (TREE_DUMP_SET, parent->value);
    }
    else if (parent->node_type == TYPE_VAR)
    {
        graph_dump_set->nodes->fillcolor = "#7FC7FF";
        print_str_node (TREE_DUMP_SET, parent->name);
    }
    else if (parent->node_type == TYPE_FUNC)
    {
        graph_dump_set->nodes->fillcolor = "#DC143C";

        switch ( (int) parent->value)
        {
            case LIB_PRINT:
                print_str_node (TREE_DUMP_SET, "print");
                break;
            case LIB_WRITELN:
                print_str_node (TREE_DUMP_SET, "writeln");
                break;
            default:
                print_str_node (TREE_DUMP_SET, parent->name);
        }
    }
    else if (parent->node_type >= OP_LESS && parent->node_type <= OP_LESS_EQ)
    {
        graph_dump_set->nodes->fillcolor = "#3EB489";

        switch (parent->node_type)
        {
            case OP_EQ:
                print_char_node (TREE_DUMP_SET, OP_EQ);
                break;
            case OP_COMP_EQ:
                print_str_node (TREE_DUMP_SET, "==");
                break;
            case OP_LESS:
                print_str_node (TREE_DUMP_SET, "\\<");
                break;
            case OP_ABOVE:
                print_str_node (TREE_DUMP_SET, "\\>");
                break;
            case OP_ABOVE_EQ:
                print_str_node (TREE_DUMP_SET, "\\>=");
                break;
            case OP_LESS_EQ:
                print_str_node (TREE_DUMP_SET, "\\<=");
                break;
        }
    }
    else
    {
        graph_dump_set->nodes->fillcolor = "#77DD77";

        switch (parent->node_type)
        {
            case OP_GART_N:
                graph_dump_set->nodes->fillcolor = "#EDD19C";
                print_char_node (TREE_DUMP_SET, ';');
                break;
            case OP_COMMA:
                graph_dump_set->nodes->fillcolor = "#EDD19C";
                print_char_node (TREE_DUMP_SET, ',');
                break;
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
            case OP_IF:
                graph_dump_set->nodes->fillcolor = "#7442C8";
                print_str_node  (TREE_DUMP_SET, "if");
                break;
            case OP_END:
                graph_dump_set->nodes->fillcolor = "#EDD19C";
                print_str_node  (TREE_DUMP_SET, "end!");
                break;
            case OP_BEGIN:
                graph_dump_set->nodes->fillcolor = "#EDD19C";
                print_str_node  (TREE_DUMP_SET, "begin:");
                break;
            case OP_RETURN:
                graph_dump_set->nodes->fillcolor = "#DC143C";
                print_str_node  (TREE_DUMP_SET, "return");
                break;
        }
    }

    if (parent->left != NULL)
    {
        graph_dump_set->edges->fillcolor = "#7FC7FF";
        graph_dump_set->edges->color     = "#7FC7FF";
        make_edge  (graph_dump_set, &parent->value, &parent->left->value, *graph_dump_set->edges);
        tree_print (graph_dump_set, parent->left);
    }
    if (parent->right != NULL)
    {
        graph_dump_set->edges->fillcolor = "#DC143C";
        graph_dump_set->edges->color     = "#DC143C";
        make_edge  (graph_dump_set, &parent->value, &parent->right->value, *graph_dump_set->edges);
        tree_print (graph_dump_set, parent->right);
    }
    return 0;
}
