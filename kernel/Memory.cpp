#include <Memory.h>
#include <CGA.h>
#include <Serial.h>
char pages_free[max_pages];
char pages_size[max_pages];
uint32_t m_start;
uint32_t m_end;
uint32_t map_start;
void mm_init()
{
	map_start = 0x200000;
	for (int i = 0; i < max_pages; i++)
	{
		pages_free[i] = 1;
	}
	if (Serial::verboseOn)
		Serial::log("Memory setup [Done]\n");
}

void memcpy(char *to, char *from, uint32_t size_to_move)
{
	for (uint32_t i = 0; i < size_to_move; i++)
	{
		to[i] = from[i];
	}
}
void memset(char *p, char c, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		p[i] = c;
	}
}
char *getPage(int page_number)
{
	return (char *)(map_start + (page_number * mm_page_size));
}
uint32_t size_to_pages_count(uint32_t size)
{

	uint32_t out = size / mm_page_size;
	if (size % mm_page_size != 0)
		out++;
	return out;
}
uint32_t addr_to_page_index(char *addr)
{
	uint32_t ind = (uint32_t)addr;
	ind -= map_start;
	if (ind != 0)
		ind /= mm_page_size;
	return (uint32_t)ind;
}
char *malloc(uint32_t size)
{
	//to prevent time waste

	if (size == 0)
		return nullptr;
	for (uint32_t i = 0; i < max_pages; i++)
	{
		if (pages_free[i])
		{
			bool possible = 1;
			uint32_t pages_required = size_to_pages_count(size) + 1;

			//Finding if there are required pages in a row

			for (uint32_t j = 0; j < pages_required; j++)
			{
				if (!pages_free[i + j])
				{
					possible = 0;
					break;
				}
			}
			if (possible)
			{

				//Marking pages "IN USE" to prevent use of them by others

				for (uint32_t j = 0; j < pages_required; j++)
				{
					pages_free[i + j] = 0;
				}

				//Page size will be size/256 + 256 or the total pages taken in the alloc
				pages_size[i] = pages_required;

				//Converting the page number to address
				memset(getPage(i), 0, size);
				return getPage(i);
			}
		}
	}
	return nullptr;
}
void free(char *addr)
{
	uint32_t page_number = addr_to_page_index(addr);
	if (page_number >= max_pages || (uint32_t)addr < map_start || (uint32_t)addr > (max_pages * mm_page_size) + (uint32_t)map_start)
	{
		return;
	}
	for (int i = 0; i < pages_size[page_number]; i++)
	{
		pages_free[i + page_number] = 1;
	}
}
bool is_page_free(uint32_t index)
{
	if (pages_free[index])
		return 1;
	return 0;
}
char *realloc(char *addr, uint32_t size)
{
	char *new_addr = malloc(size);
	memcpy(new_addr, addr, size);
	free(addr);
	return new_addr;
}
int memcmp(char *a, char *b, int len)
{
	for (int i = 0; i < len; i++)
		if (a[i] != b[i])
			return 1;
	return 0;
}