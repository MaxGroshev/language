#include "assembler.h"

int main ()
{
    FILE*  word_com = fopen (NAME_OF_PROG, "r");
    MY_ASSERT (word_com != NULL);
    size_t count_of_com   = 0;
    size_t count_of_token = 0;
    int    labels[10]     = {};
    memset (labels, -1, 10 * sizeof (int));

    char*  asm_prog = read_com_asm  (word_com);
    token* commands = read_word_com (&count_of_com, &count_of_token, labels, asm_prog);
    translate_com (commands, count_of_com, count_of_token, labels, asm_prog);

    fclose (word_com);
    free   (commands);
    free   (asm_prog);
}
