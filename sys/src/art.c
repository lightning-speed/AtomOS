#include "art.h"
#include <string.h>
#include <stdlib.h>
FILE *inputfile = NULL;
char *code;

void startArt()
{
    if (inputfile == NULL)
        return;

    fseek(inputfile, 0, SEEK_END);
    fsize = ftell(inputfile);
    fseek(inputfile, 0, SEEK_SET);
    code = (char *)malloc(fsize + 100);

    fread(code + 2, fsize, 1, inputfile);
    startParsing(code);
    fclose(inputfile);
}
