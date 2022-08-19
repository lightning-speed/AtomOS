#pragma once
#include <stdint.h>
enum ftype_t
{
	DIR = 1,
	FILE = 2
};
typedef struct
{
	ftype_t type;
	uintptr_t children[126];
	uintptr_t parent;
	char name[32];
	uint32_t size;
	char *content;
	uint32_t cap;
	bool open;
	uint32_t fileCounter;

} fnode;

namespace VFS
{
	extern fnode *vfs_node;
	void init();
	void mount(fnode *vfs_node);
	int del(fnode *file);
	fnode *getChild(char *name, fnode *parent);
	fnode *find(char *path);
	fnode *createFile(char *name);
	fnode *openTree(char *path, char *prot);
	fnode *open(char *path, char *prot);
	int read(fnode *f, uint32_t index);
	uint32_t write(fnode *f, int data);
	int setBuffer(fnode *node, char *buff, uint32_t size);
	int close(fnode *node);
	bool exists(char *pathc);
	char *getNodeName(fnode *node, int index);
	void read(char *ptr, uint32_t size, fnode *f);
	fnode *findFileUsingPaths(char *name);
};
