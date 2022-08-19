#include <stdio.h>
#include <string.h>
#include "art.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (inputfile == NULL)
        {
            inputfile = fopen(argv[i], "rb+");
        }
    }
    startArt();
}