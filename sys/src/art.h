#include <stdint.h>
#include <stdio.h>

extern FILE *inputfile;
uint32_t fsize;

void startArt();
int startParsing(char *code);
char *tsr(char *sr, int rw);