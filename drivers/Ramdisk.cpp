#include <Ramdisk.h>
#include <CGA.h>
#include <Memory.h>
#include <VFS.h>
char *Ramdisk::start;
uint32_t Ramdisk::size;

// JUST IGNORE THIS CODE, FOR THE SAKE OF YOUR LIFE

void Ramdisk::init()
{

	char *disk = Ramdisk::start;
	fnode *tree = (fnode *)malloc(sizeof(fnode));
	tree->size = 0;
	tree->type = DIR;
	VFS::mount(tree);
	for (uint32_t i = 0;;)
	{
		file_t *file = (file_t *)(disk + i);
		if (file->name[0] == 0)
			return;
		fnode *node = VFS::open(file->name, "w");
		VFS::setBuffer(node, (char *)((uint32_t)(file->content) + disk), file->size);
		i += sizeof(file_t);
		VFS::close(node);
	}
}