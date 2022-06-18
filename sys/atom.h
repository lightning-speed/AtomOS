#pragma once
#include <stdint.h>
#include <string.h>
#define DT_DIR 1
#define DT_REG 2
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
	uint32_t fileCounter;
} fnode;
typedef struct
{
	enum ftype_t d_type;
	uintptr_t children[126];
	uintptr_t parent;
	char d_name[32];
	uint32_t size;
	char *content;
	uint32_t cap;
	char open;
	uint32_t fileCounter;
} dirent;
typedef FILE DIR;
DIR *opendir(const char *path);
dirent *readdir(DIR *dir);
int close(DIR *dir);
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
