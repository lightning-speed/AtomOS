#include <VFS.h>
#include <String.h>
#include <CGA.h>
#include <Memory.h>
#include <Serial.h>

fnode *VFS::vfs_node;
namespace VFS
{
	void init()
	{
		vfs_node = (fnode *)malloc(sizeof(fnode));
	}
	void mount(fnode *vn)
	{
		if (vn == nullptr)
			return;
		for (uint32_t i = 0; i < vn->size; i++, vfs_node->size++)
		{
			vfs_node->children[vfs_node->size] = vn->children[i];
		}
		free((char *)vn);
		Serial::log("New FS node mounted");
	}
	int rename(fnode *file, char *name)
	{
		memcpy(file->name, name, 32);
	}
	int del(fnode *file)
	{
		free(file->content);
		free((char *)file);
		fnode *parent = (fnode *)file->parent;
		for (uint32_t i = 0; i < parent->size; i++)
		{
			//Finding the file in array
			if ((fnode *)parent->children[i] == file)
			{
				for (int j = i; j < parent->size; j++)
					parent->children[j] = parent->children[j + 1];
				parent->size--;
				return 1;
			}
		}
		return 0;
	}
	fnode *getChild(char *name, fnode *parent)
	{
		for (uint32_t i = 0; i < parent->size; i++)
		{
			if (!strcmp(name, ((fnode *)parent->children[i])->name))
			{
				return (fnode *)(parent->children[i]);
			}
		}
		return nullptr;
	}
	bool exists(char *pathc)
	{
		char path[40];
		if (strlen(pathc) == 0)
			return false;
		memcpy(path, pathc, 40);
		char *name = strtok(path, "/");
		fnode *file = vfs_node;
		while (name != nullptr)
		{
			file = getChild(name, file);
			if (file == nullptr)
				return false;
			name = strtok(nullptr, "/");
		}
		return true;
	}
	fnode *createFile(char *name)
	{
		fnode *node = (fnode *)malloc(sizeof(fnode));
		memcpy(node->name, name, strlen(name));
		node->size = 0;
		return node;
	}
	fnode *openTree(char *pathc, char *prot)
	{
		char path[40];
		memcpy(path, pathc, 40);
		char *name = strtok(path, "/");
		fnode *file = nullptr;
		fnode *last = vfs_node;
		while (name != nullptr)
		{
			file = getChild(name, last);
			//IF NOT THERE, CREATE ONE
			fnode *n = nullptr;
			if (file == nullptr)
			{
				if (!(!strcmp(prot, "w") || !strcmp(prot, "a")))
					return nullptr;
				n = createFile(name);
				n->size = 0;
				n->type = FILE;
				n->parent = (uintptr_t)last;
				file = n;
				last->children[last->size] = (uintptr_t)n;
				last->size++;
			}
			last = file;
			name = strtok(nullptr, "/");
			if (name != nullptr && n != nullptr)
				n->type = DIR;
		}

		return file;
	}
	fnode *open(char *path, char *prot)
	{
		if (!strcmp(path, "."))
		{
			return vfs_node;
		}
		//IF FILE EXISTS IT WONT CREATE NEW IT WILL JUST GIVE THE EXISTING NODE
		fnode *out = openTree(path, prot);
		if (out != nullptr)
		{
			out->open = true;
		}
		return out;
	}
	uint32_t write(fnode *f, int data)
	{
		if (f != nullptr)
			if (f->open)
			{
				if (f->cap <= f->size)
				{
					f->content = realloc(f->content, 1024);
					f->cap += 1024;
				}
				f->content[f->size] = data;
				f->size++;
			}
	}
	int read(fnode *f, uint32_t index)
	{
		if (f->open)
			return f->content[index];
	}
	int setBuffer(fnode *node, char *buff, uint32_t size)
	{
		if (node != nullptr)
		{
			node->content = buff;
			node->size = size;
			node->cap = size;
			return 1;
		}
		return 0;
	}
	int close(fnode *node)
	{
		if (node != nullptr)
		{
			node->open = false;
			return 1;
		}
		return 0;
	}
	char *getNodeName(fnode *node, int index)
	{
		return ((fnode *)node->children[index])->name;
	}
	void read(char *ptr, uint32_t size, fnode *f)
	{
		if (f != nullptr && f->open)
			memcpy(ptr, f->content, size);
	}
};