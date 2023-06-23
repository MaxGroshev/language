struct RAM
{
    int*   data;
    size_t capacity;
};

void RAM_init  (RAM* T_RAM);
void RAM_clean (RAM* T_RAM);
