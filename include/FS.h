#pragma once
#include <stdint.h>
#include <String.h>

#define CLUSTER_SIZE 512
#define MAX_CLUSTERS 1024 * 20
typedef struct
{
	char name[32];
	uint32_t size;
	bool isOpen;
	char *content;
	bool writable;
	uint32_t cap;
} file_t;
typedef struct
{
	file_t **files;
	uint32_t fileCount;
} Drive;
namespace FS
{
	extern Drive *drive;
	file_t *createFile(string name);
	void init();
	file_t *openFile(string name);
	bool doesExists(file_t *file);
	bool doesExists(char *name);
	void mount(Drive *drive);
	char read(uint32_t index, file_t *file);
	void read_s(char *ptr, uint32_t len, file_t *file);
	void close(file_t *file);
	bool write(file_t *file, char data);
	uint32_t getSize(file_t *file);
};
typedef file_t *File;