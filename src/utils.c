#include <stdio.h>
#include "utils.h"

// Tool to flush stdin
void flushStdin()
{
    char c;
    do
        c = getchar();
    while (c != '\n' && c != EOF);
}