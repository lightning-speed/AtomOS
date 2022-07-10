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
		if (d->d_type != DT_DIR)
			printf("%s\n", d->d_name);
	}
	close(dir);
	dir = opendir(path);
	while ((d = readdir(dir)) != NULL)
	{
		if (d->d_type != DT_REG)
			printf("%s%c\n", d->d_name, '/');
	}
	close(dir);
}
int main(int argc, char **argv)
{
	cbuffer = malloc(2048);
	Window win = (Window)syscall(12, 0, 0, (300 << 8) | 100);
	win->x = 100;
	win->y = 100;
	setColor(((255 << 24) | (0 * 27 << 16) | (0 * 17 << 8) | 0 * 10));
	fillRect(win, 0, 0, 300, 100);
	swap_buffer_pos(win, 0, 0, 300, 100);
	for (int e = 0; e < 100; e++)
	{
		setColor(((255 << 24) | (e * 27 << 16) | (e * 17 << 8) | e * 10));
		fillRect(win, e, e, 100, 100);
		swap_buffer_pos(win, e, e, 100, 100);
	}
	printf("Atom cmd [Version 1.0.0]\nCopyright (c) 2022 Peko.  All rights reserved.\n\nType help for commands\n");
	set_screen(0, 0x0f);
	putchar('\n');
	while (1)
	{
		for (int i = 0; i < 2048; i++)
		{
			cbuffer[i] = 0;
		}
		set_screen(0, 0x0e);
		printf("root@atom# ");
		set_screen(0, 0x0f);
		scanf("%s", cbuffer);
		if (!strcmp(cbuffer, "exit"))
		{
			free(cbuffer);
			return 0;
		}
		int res = system(cbuffer);
		int n = strlen(cbuffer);
		for (int i = 0; i < n; i++)
			if (cbuffer[i] == ' ')
				cbuffer[i] = 0;
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
				putchar('\n');
			}
			else if (!strcmp(cbuffer, "ls"))
			{
				char *path = cbuffer + strlen(cbuffer) + 1;
				if (strlen(path) == 0)
					ls_path(".");
				else
				{
					ls_path(path);
				}
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
			else if (!strcmp(cbuffer, "touch"))
			{
				FILE *f = fopen(cbuffer + strlen(cbuffer) + 1, "w");
				fclose(f);
			}
			else if (res == 0)
			{
				printf("%s%s\n", "command not found: ", cbuffer);
			}
	}
}
