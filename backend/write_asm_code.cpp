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
            fprintf (prog_file, "push %lg\n", prog_tree->value);
            break;
        case OP_EQ:
            fprintf (prog_file, "pop [%d]\n", prog_tree->left->num_of_var);
            break;
        case TYPE_VAR:
            fprintf (prog_file, "push [%d]\n", prog_tree->num_of_var);
            break;
        case TYPE_FUNC:
        {
//             if (strcmp ("meow", prog_tree->name) == 0)
//             {
//                 fprintf (prog_file, "#%s", prog_tree->name);
//                 break;
//             }
            if (prog_tree->value == LIB_PRINT)   fprintf (prog_file, "out\n");
            else if (prog_tree->value == LIB_WRITELN) fprintf (prog_file, "in\n");
           // else fprintf (prog_file, "func_%s", prog_tree->name);
            break;
        }

        //case OP_COMMA:      fprintf (prog_file, ",");          break; // do not know
        case OP_ADD:        fprintf (prog_file, "add\n");      break;
// 	    case OP_GART_N:     fprintf (prog_file, ";");      break;
//         case OP_SUB:        fprintf (prog_file, "-");      break;
//         case OP_MUL:        fprintf (prog_file, "*");      break;
//         case OP_DIV:        fprintf (prog_file, "/");      break;
//         case OP_POW:        fprintf (prog_file, "^");      break;
//
//         case OP_LESS:       fprintf (prog_file, "<");      break;
//         case OP_ABOVE:      fprintf (prog_file, ">");      break;

//         case OP_ABOVE_EQ:   fprintf (prog_file, ">=");     break;
//         case OP_LESS_EQ:    fprintf (prog_file, "<=");     break;
//         case OP_COMP_EQ:    fprintf (prog_file, "==");     break;
//
//         //case OP_BEGIN:      fprintf (prog_file, ";");      break;
//         case OP_END:        fprintf (prog_file, ";");      break;
//         case OP_IF:         fprintf (prog_file, "if");     break;
//         case OP_RETURN:     fprintf (prog_file, "return"); break;
    }

    return 0;
}
