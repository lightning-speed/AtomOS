#include <ELF.h>
#include <CGA.h>
#include <Memory.h>
#include <Scheduler.h>
#include <Serial.h>

namespace ELF
{
	ELF_prog *load(fnode *f)
	{
		char *fc = f->content;
		Elf32_Ehdr *header = (Elf32_Ehdr *)fc;
		if (header->e_ident[0] != 0x7F || header->e_ident[1] != 'E' || header->e_ident[2] != 'L' || header->e_ident[3] != 'F')
		{
			Serial::log("\nELF_LOADER: error: file is not elf\n");
			return nullptr;
		}
		if ((Elf_Type)header->e_type != ET_EXEC)
		{
			Serial::log("\nELF_LOADER: error: elf is not executable\n");
			return nullptr;
		}

		uint32_t prog_entry = header->e_entry;
		for (uintptr_t i = 0; i < (uint32_t)header->e_shentsize * header->e_shnum; i += header->e_shentsize)
		{
			Elf32_Shdr *shdr = (Elf32_Shdr *)((uintptr_t)header + (header->e_shoff + i));

			if (shdr->sh_type == SHT_NOBITS)
			{
				memset((char *)(shdr->sh_addr), 0x0, shdr->sh_size);
			}
			//LOADING SECTIONS INTO THE MEMORY
			else if (shdr->sh_addr != 0)
			{
				if (shdr->sh_addr < PROGRAM_SPACE_START)
				{
					//if some program's setcion if located for kernel side addresses
					Serial::log("\nELF_LOADER: error: Program's Section Overlaps Kernel Space\n");
					return nullptr;
				}
				memcpy((char *)(shdr->sh_addr), (char *)((uintptr_t)header + shdr->sh_offset), shdr->sh_size);
			}
		}
		ELF_prog *out = (ELF_prog *)malloc(sizeof(ELF_prog));
		out->name = (string)f->name;
		out->entryPoint = (void *)prog_entry;
		return out;
	}
};