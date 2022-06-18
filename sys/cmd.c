#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "atom.h"
char *cbuffer;
void ls_path(char *path)
{
	DIR *dir = opendir(path);
	if (dir == NULL)
	{
		printf("no such dir");
		return;
	}
	dirent *d;
	while ((d = readdir(dir)) != NULL)
	{
		printf("%s%c\n", d->d_name, '/' * (d->d_type == DT_DIR));
	}
	close(dir);
}
int main(int argc, char **argv)
{
	cbuffer = malloc(2048);
	printf("Atom cmd [Version 1.0.0]\nCopyright (c) 2022 Lightning-Speed.  All rights reserved.\n\nType help for commands(help is not a command btw)\n");
	set_screen(0, 0x0f);
	while (1)
	{
		for (int i = 0; i < 2048; i++)
		{
			cbuffer[i] = 0;
		}
		set_screen(0, 0x0e);
		printf("\nroot@atom# ");
		set_screen(0, 0x0f);
		scanf("%s", cbuffer);
		putchar('\n');
		int res = system(cbuffer);
		if (strlen(cbuffer) != 0)
			if (!strcmp(cbuffer, "uwufetch"))
			{
				set_screen(0, 0x0d);
				printf("\n   /\\\\\\\\\\\\\n   /\\\\\\\\\\\\\\\\\\\n   \\\\\\//////\\\\\\\n   \\/\\\\\\    \\/\\\\\\\n    \\/\\\\\\\\\\\\\\\\\\\\\\\\\n     \\/\\\\\\//////\\\\\\\n      \\/\\\\\\    \\/\\\\\\\n       \\/\\\\\\    \\/\\\\\\\n        \\///     \\///\n");
				set_screen(0, 0x0f);
				set_screen(1, 23);
				set_screen(4, 4);
				printf("Kernel: Atom Kernel (32-bit)");
				set_screen(1, 23);
				set_screen(4, 1);
				printf("Version: 1.1.2");
				set_screen(1, 23);
				set_screen(4, 1);
				printf("OS: Atom OS");
				set_screen(4, -6);
			}
			else if (!strcmp(cbuffer, "ls"))
			{
				char *path = cbuffer + strlen(cbuffer) + 1;
				if (path == NULL)
					ls_path(".");
				else
				{
					ls_path(path);
				}
			}
			else if (!strcmp(cbuffer, "exit"))
			{
				free(cbuffer);
				return 0;
			}
			else if (!strcmp(cbuffer, "time"))
			{
				printf("Time is: %i:%i:%i", get_hours(), get_minutes(), get_seconds());
			}
			else if (!strcmp(cbuffer, "date"))
			{
				printf("%i-%i-%i", get_days(), get_months(), get_years());
			}
			else if (!strcmp(cbuffer, "cls"))
			{
				set_screen(6, 0);
			}
			else if (!strcmp(cbuffer, "mkdir"))
			{
				fnode *dirf = (fnode *)fopen(cbuffer + strlen(cbuffer) + 1, "w");
				dirf->type = dir;
			}
			else if (res == 0)
			{
				printf("%s%s", "command not found: ", cbuffer);
			}
	}
}
