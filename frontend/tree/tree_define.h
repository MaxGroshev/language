//------------------------------------------------MACROS_OF_DIR-----------------------------------------------------------------------------------------------------------------------------------------------

#define TREE_GRAPH_DOT_DIR   "./frontend/tree/graph_lib/tree_dump.dot"
#define TREE_REC_DESCENT_DIR "./lang.txt"
#define TREE_LOGS_PNG_DIR    "./logs/log_pics"
#define TREE_LOGS_HTML_DIR   "./logs/log_file.html"

//------------------------------------------------MACROS_OF_DUMP_AND_LOGS--------------------------------------------------------------------------------------------------------------------------------------------

#define TREE_DUMP_SET      graph_dump_set, &parent->value, *graph_dump_set->nodes, &parent->right->value, &parent->left->value
#define CUR_POS_IN_PROG    __FILE__, __PRETTY_FUNCTION__, __LINE__

#define T_TREE_SUC_CREATED      "<font color = #8DB6CD size=6>Tree was successfully created</font>\n\n"
#define T_FAIL_OF_CREATING_EDGE "<font color = #red size=6>ERROR of in creating edge</font>\n\n"
#define T_TREE_WAS_CLEARED      "<font color = #8DB6CD size=6>Tree was cleared and deleted </font>\n"
#define T_INT_POWERED           "<font color = #8DB6CD size=6> Value of the node was int powered </font>\n\n"
