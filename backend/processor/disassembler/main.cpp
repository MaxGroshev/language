#include "disassembler.h"

int main ()
{
    int* code_of_com = read_bin_file ();
    disasm_com (code_of_com);

    free (code_of_com);
    return 0;
}
