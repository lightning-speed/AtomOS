#pragma once
#include <stdint.h>
#include <string.h>
void set_screen(uint32_t type, uint32_t val);
uint32_t syscall(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx);
enum ftype_t
{
	dir = 1,
	file = 2
};
typedef struct
{
	enum ftype_t type;
	uintptr_t children[126];
	uintptr_t parent;
	char name[32];
	uint32_t size;
	char *content;
	uint32_t cap;
	char open;
} fnode;
int get_seconds();
int get_minutes();
int get_hours();
int get_days();
int get_months();
int get_years();
uint32_t fsize(FILE *f);
fnode *getChild(char *name, fnode *parent);
fnode *getChild(char *name, fnode *parent)
{
	for (uint32_t i = 0; i < parent->size; i++)
	{
		if (!strcmp(name, ((fnode *)parent->children[i])->name))
		{
			return (fnode *)(parent->children[i]);
		}
	}
	return (void *)0;
}

void cd(char *str);
void ls();
fnode *current_node;
fnode *root_node;
void cd(char *in)
{
	char str[50];
	memcpy(str, in, 50);
	if (current_node == (void *)0)
	{
		current_node = (fnode *)fopen(".", "r");
		root_node = current_node;
		current_node->open = 0;
	}
	char *temp = strtok(str, "/");
	while (temp != (char *)0)
	{
		if (!strcmp(temp, ".."))
		{
			if ((fnode *)current_node->parent != (void *)0)
				current_node = (fnode *)current_node->parent;
		}
		else
		{
			fnode *node = getChild(temp, current_node);
			if (node != (void *)0 && node->type == dir)
				current_node = node;
			else
				printf("no such file");
		}
		temp = strtok((char *)0, "/");
	}
}
void ls()
{
	if (current_node == (void *)0)
	{
		current_node = (fnode *)fopen(".", "r");
		root_node = current_node;
		current_node->open = 0;
	}
	for (uint32_t i = 0; i < current_node->size; i++)
	{
		printf("%s%c\n", ((fnode *)current_node->children[i])->name, (((fnode *)current_node->children[i])->type == dir) * '/');
	}
}
char *getenv(const char *of)
{
	if (!strcmp(of, "PWD"))
	{
		return ((char **)syscall(10, 0, 0, 0))[0];
	}
	if (!strcmp(of, "HOME"))
	{
		return ((char **)syscall(10, 0, 0, 0))[1];
	}
	if (!strcmp(of, "PATH"))
	{
		return ((char **)syscall(10, 0, 0, 0))[2];
	}
}
void ls_path(char *path)
{
	cd(path);
	ls();
	current_node = root_node;
}
