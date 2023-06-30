#include "str_func.h"

//=====================================COMPARE_NUM_OF_ELEM_SYMBOLS================================================================
int l_strncomp (const char* str1, const char* str2, size_t num_of_elem, int skip_space, int* pos_in_buf)
{
    if (skip_space == STR_NOT_SKIP_SPACE)
    {
        for (int i = 0; i < num_of_elem; i++)
        {
            if ((str1 + i == NULL) || (str2 + i == NULL) || (*(str1 + i) != *(str2 + i)))
            {
                return 0;
            }
        }
        return 1;
    }
    else if (skip_space == STR_SKIP_SPACE)
    {
        int begin_pos = *pos_in_buf;
        int read_elem = 0;
        for (int comp_elem = 0; comp_elem < num_of_elem; read_elem++, comp_elem++)
        {
            if ((*(str1 + read_elem) == 32 || *(str1 + read_elem) == '\n') && comp_elem == 0)
            {
                comp_elem--;
                continue;
            }
            if ((str1 + read_elem == NULL) || (str2 + comp_elem == NULL) || (*(str1 + read_elem) != *(str2 + comp_elem)))
            {
                *pos_in_buf = begin_pos;
                return 0;
            }
        }
        while (*(str1 + read_elem) == 32)
        {
            read_elem++;
        }
        *pos_in_buf = begin_pos + read_elem;
        return 1;
    }
    return 0;
}

//=======================CONVERT_STR_TO_DOUBLE===============================================================================

double my_strtod (const char* str, int* pos_in_buf)
{
    int point_flag = 0;
    char value_in_str[BUF_OF_64_ELEM];
    int i = 0;
    for (; (str[i] >= '0' && str[i] <= '9') || str[i] == '.'; i++)
    {
        // if ((str[0] == '0' && str[1] != '.') || (i == BUF_OF_64_ELEM) || (str[i] == '.' && point_flag != 0))
        // {
        //     fprintf (stderr, "Wrong form of number");
        // }; // not str to error

        if (str[i] == '.') point_flag = 1;
        value_in_str[i] = str[i];
    }
    value_in_str[i] = '\0';
    *pos_in_buf += i;

    double value = 0;
    sscanf (value_in_str, "%lg", &value);
    printf ("I get this num: %lg\n", value);
    return value;
}
