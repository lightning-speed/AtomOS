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
			for (int i = 0; i < fsize(f); i++)
			{
				putchar(fgetc(f));
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