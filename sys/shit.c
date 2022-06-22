
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
struct shit
{
	int a;
	int b;
	int c;
};
int main()
{
	FILE *sh = fopen("shit.c", "rb");
	char *ptr = malloc(1024);
	for (int i = 0; i < 2; i++)
	{
		fread(ptr, 1, 20, sh);
		printf(ptr);
	}
}