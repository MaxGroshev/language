#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
char* gets_the_text(FILE *onegin, char* memory);
int main()
{

    FILE *onegin = fopen ("onegin.txt", "r");
    assert(onegin != NULL);

    for(int i = 0; i < 10; i++)
    {
        char* memory = 0;
        char *string;
        string = gets_the_text(onegin, memory);
        printf("%s\n", string);
        free(string);
    }

    fclose (onegin);
}

 //==============================================================================================================

 char* gets_the_text(FILE *onegin, char* memory)
{

    memory = (char*)calloc(50, sizeof(char));
    fgets(memory, 50, onegin);
    return strdup(memory);
    free(memory);


}
