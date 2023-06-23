#include "CPU.h"

void RAM_init (RAM* T_RAM)
{
    T_RAM->capacity = 1000;
    T_RAM->data = (int*) calloc (T_RAM->capacity, sizeof (int));
    MY_ASSERT (T_RAM->data != NULL);

    memset (T_RAM->data, 1, T_RAM->capacity * sizeof (int));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------

void RAM_clean (RAM* T_RAM)
{
    free (T_RAM->data);
}

