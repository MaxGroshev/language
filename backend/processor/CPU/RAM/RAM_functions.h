#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"

struct RAM
{
    int*   data;
    size_t capacity;
};

void RAM_init  (RAM* T_RAM);
void RAM_clean (RAM* T_RAM);
