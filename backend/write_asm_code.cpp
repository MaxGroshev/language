#include "backend.h"

int write_asm_code (tree_node_t* prog_tree, FILE* prog_file)
{
    if (prog_tree->left != NULL)
    {
        write_asm_code (prog_tree->left, prog_file);
    }
    if (prog_tree->right != NULL)
    {
        write_asm_code (prog_tree->right, prog_file);
    }

    switch (prog_tree->node_type)
    {
        case TYPE_NUM:
            fprintf (prog_file, "push %lg\r\n", prog_tree->value);
            break;
        case OP_EQ:
            fprintf (prog_file, "pop [%d]\r\n", prog_tree->left->num_of_var);
            break;
        case TYPE_VAR:
            fprintf (prog_file, "push [%d]\r\n", prog_tree->num_of_var);
            break;
        case TYPE_FUNC:
        {
//             if (strcmp ("meow", prog_tree->name) == 0)
//             {
//                 fprintf (prog_file, "#%s", prog_tree->name);
//                 break;
//             }
            if      (prog_tree->value == LIB_PRINT)   fprintf (prog_file, "out\r\n");
            else if (prog_tree->value == LIB_WRITELN) fprintf (prog_file, "in\r\n");
            else if (prog_tree->value == LIB_SQR)     fprintf (prog_file, "sqrt\r\n");
           // else fprintf (prog_file, "func_%s", prog_tree->name);
            break;
        }



        //case OP_COMMA:      fprintf (prog_file, ",");          break; // do not know
        case OP_ADD:        fprintf (prog_file, "add\r\n");      break;
// 	    case OP_GART_N:     fprintf (prog_file, ";");      break;
        case OP_SUB:        fprintf (prog_file, "sub\r\n");      break;
        case OP_MUL:        fprintf (prog_file, "mul\r\n");      break;
        case OP_DIV:        fprintf (prog_file, "div\r\n");      break;
//         case OP_POW:        fprintf (prog_file, "^");      break;


        case OP_IF:         fprintf (prog_file, ":1\n");     break;
//         case OP_LESS:       fprintf (prog_file, "<");      break;
//         case OP_ABOVE:      fprintf (prog_file, ">");      break;

//         case OP_ABOVE_EQ:   fprintf (prog_file, ">=");     break;
//         case OP_LESS_EQ:    fprintf (prog_file, "<=");     break;
        case OP_N_COMP_EQ:    fprintf (prog_file, "je :1 \n");     break;
//
//         case OP_RETURN:     fprintf (prog_file, "return"); break;
    }

    return 0;
}
