#pragma once
#include <stdio.h>

enum string_func_cond
{
    STR_NOT_SKIP_SPACE = 0,
    STR_SKIP_SPACE     = 1,
};

#define BUF_OF_64_ELEM 64

//=============================================================================================

int    l_strncomp       (const char* str1, const char* str2, size_t num_of_elem, int skip_space, int* pos_in_buf);
double my_strtod        (const char* str, int* pos_in_buf);
