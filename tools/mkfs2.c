#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct
{
	char name[32];
	uint32_t size;
	bool isOpen;
	char *content;
	bool writable;
	uint32_t cap;
} file_t;

char *disk;
int index = 0;
int content_index = 0;
void addFile(char *name);
void writefile();
char *outputfilename = "output";
int exists(const char *fname)
{
	FILE *file;
	if ((file = fopen(fname, "r")))
	{
		fclose(file);
		return 1;
	}
	return 0;
}
int main(int argc, char **argv)
{
	disk = (char *)malloc(1024 * 1024 * 8);
	for (int i = 0; i < 1024 * 1024 * 8; i++)
	{
		disk[i] = 0;
	}
	for (int i = 1; i < argc; i++)
	{
		if (i == argc - 1)
		{
			outputfilename = argv[i];
			printf(outputfilename);
		}
		else
			addFile(argv[i]);
	}
	writefile();
}
void addFile(char *name)
{
	file_t *file = (file_t *)(disk + index);
	memcpy(file->name, name, strlen(name));
	FILE *f = fopen(name, "rb");
	if (!f)
	{
		printf("\nFile doesn't exist");
		exit(-1);
	}
	fseek(f, 0L, SEEK_END);
	uint32_t size = ftell(f);
	fseek(f, 0L, SEEK_SET);
	file->size = size;
	file->cap = size;
	file->writable = 0;
	file->content = (char *)(10024 + content_index);
	for (uint32_t i = 0; i < size; i++)
	{
		disk[10024 + content_index + i] = fgetc(f);
	}
	index += sizeof(file_t);
	content_index += size;
	fclose(f);
}
void writefile()
{
	FILE *f = fopen(outputfilename, "wb+");
	for (int i = 0; i < 10024 + content_index; i++)
	{
		fputc(disk[i], f);
	}
	fclose(f);
	printf("%d", sizeof(file_t));
}