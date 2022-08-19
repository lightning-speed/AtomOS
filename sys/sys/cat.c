#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "atom.h"
int main(int argc, char **argv)
{
	if (argc > 1)
	{
		FILE *f = fopen(argv[1], "r");
		if (f != NULL)
		{
			int fsize;
			fseek(f, 0, SEEK_END);
			fsize = ftell(f);
			fseek(f, 0, SEEK_SET);
			for (int i = 0; i < fsize; i++)
			{
				putchar(getc(f));
			}
			fclose(f);
		}
		else
			printf("file doesn't exist");
	}
	else
		printf("Usage: cat [file name]");
	return 0;
}