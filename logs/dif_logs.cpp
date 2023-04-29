#include "../language.h"
#include "log_file.h"

static FILE* LOG_FILE = NULL;

int log_init (const char* log_file_name)
{
    LOG_FILE = fopen (log_file_name, "w");
    MY_ASSERT (LOG_FILE != NULL);
    fprintf   (LOG_FILE, "<pre>\n\n");
    return 0;
}

void write_tree_logs (int code_of_print, tree_node_t* node, const char* file_name, const char* func_name, int num_of_line)
{
    MY_ASSERT (LOG_FILE != NULL);
    switch (code_of_print)
    {
        case T_NODE_SUC_CREATED:
            if (node->node_type == TYPE_VAR)
            {
                fprintf (LOG_FILE, "<font color = #7FC7FF size=5>Created TYPE_VAR node: address (%p) | var    (%c)</font>\n", &node->value, node->node_type);
            }
            else if (node->node_type == TYPE_NUM)
            {
                fprintf (LOG_FILE, "<font color = #FF8C69 size=5>Created TYPE_NUM node: address (%p) | value  (%d)</font>\n", &node->value, node->value);
            }
            else
            {
                fprintf (LOG_FILE, "<font color = #77DD77 size=5>Created TYPE_OP  node: address (%p) | op_code (%d)</font>\n", &node->value, node->node_type);
            }
            break;

        case T_L_EDGE_SUC_CREATED:
            fprintf (LOG_FILE, "<font color = #6018CF size=5>Created edge: parent address (%p)   | op_code (%d); left  (%p)  | op_code (%d)</font>\n",
                                                                             &node->value,   node->node_type, &node->left->value, node->left->node_type);
            break;

        case T_R_EDGE_SUC_CREATED:
            fprintf (LOG_FILE, "<font color = #6018CF size=5>Created edge: parent address (%p)   | op_code (%d); right  (%p) | op_code (%d)</font>\n",
                                                                             &node->value,     node->node_type,  &node->right->value, node->right->node_type);
            break;

        case T_TYPE_NUM_HAS_CHILD:
            fprintf (LOG_FILE, "<font color = red size=4>Node with type of num has child(ren): address (%p) | value  (%d)</font>\n", &node->value, node->value);
            break;

        case T_NODE_SUC_DELETED:
            fprintf (LOG_FILE, "<font color = orange size=5>Node was removed: address (%p) </font>\n", &node->value);
            break;

        case T_UNSUPPORTED_OPER:
            fprintf (LOG_FILE, "<font color = #8DB6CD size=6> Here is unsupported oper </font>\n\n");
            fprintf (stderr, "Error, check logs\n");
            break;

        case S_START_OF_BR_SEQ:
            fprintf (LOG_FILE, "<font color = #e9ff70 size=5>I have found opening bracket in %s: %s: %d </font>\n", file_name, func_name, num_of_line);

        case S_START_OF_NEG_SEQ:
            fprintf (LOG_FILE, "<font color = #e9ff70 size=5>I have found beginnig of negative seq in %s: %s: %d </font>\n", file_name, func_name, num_of_line);

    }
}

void write_extra_logs (const char* fmt,...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(LOG_FILE, fmt, args);
    va_end(args);
}

int pic_log (const char* label, const char* name_of_pic)
{
    fprintf (LOG_FILE, "\n\n<font color = #8DB6CD size = 6>%s</font>\n\n", label);
    fprintf (LOG_FILE, "<img src = %s width = ""800"" height = ""350"">\n", name_of_pic);
    return 0;
}

int log_distr ()
{
    fclose (LOG_FILE);
    return 0;
}
