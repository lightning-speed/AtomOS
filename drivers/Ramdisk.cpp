#include <Ramdisk.h>
#include <CGA.h>
#include <Memory.h>
#include <VFS.h>
#include <Serial.h>
char *Ramdisk::start;
uint32_t Ramdisk::size;

// JUST IGNORE THIS CODE, FOR THE SAKE OF YOUR LIFE

void Ramdisk::init()
{

	char *disk = Ramdisk::start;
	for (uint32_t i = 0;;i+=sizeof(file_t))
	{
		file_t *file = (file_t *)(disk + i);
		if(file->name[0]==0)return;
		fnode *node = VFS::open(file->name, "w");
		VFS::setBuffer(node, (char *)((uint32_t)(file->content) + disk), file->size);
		VFS::close(node);
	}
}