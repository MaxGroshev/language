#pragma once
#include <time.h>
#include <stdarg.h>

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int log_init    (const char* log_file_name);
int pic_log     (const char* label, const char* name_of_pic);
int log_distr   ();
