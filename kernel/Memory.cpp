#include <Memory.h>
#include <CGA.h>
#include <Serial.h>
#include <RMm.h>
char pages_free[max_pages];
char pages_size[max_pages];
SpinLock memSpinLock;
uint32_t m_start;
uint32_t m_end;
uint32_t map_start;

bool legacy_mode = true;

void mm_init()
{
	//1 MB of padding from heap_start

	map_start = 0x1900000;
	if(!legacy_mode)
		map_start = (uint32_t)(RMm::request((process_t*)1)->start);
	memset((char *)map_start,0,0x500000);
	for (int i = 0; i < max_pages; i++)
	{
		pages_free[i] = 1;
	}
	if (Serial::verboseOn)
	{
		//Serial::log("Memory setup [Done]\n");
		//Serial::log((string) "Heap Start at: " + (int)map_start + "\n");
	}
}
void *memmove(char *to, char *from, uint32_t size_to_move)
{
	for (uint32_t i = 0; i < size_to_move; i++)
	{
		to[i] = from[i];
	}
	return nullptr;
}
void *memcpy(char *to, char *from, uint32_t size_to_move)
{
	for (uint32_t i = 0; i < size_to_move; i++)
	{
		to[i] = from[i];
	}
	return nullptr;

}
void *memset(char *p, char c, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		p[i] = c;
	}
	return nullptr;

}
char *getPage(uint32_t page_number)
{
	return (char *)(map_start + (page_number * mm_page_size));
}
uint32_t size_to_pages_count(uint32_t size)
{

	uint32_t out = size / mm_page_size;
	return out;
}
uint32_t addr_to_page_index(char *addr)
{
	uint32_t ind = (uint32_t)addr;
	ind -= map_start;
	ind /= mm_page_size;
	return (uint32_t)ind;
}
char *malloc(uint32_t size)
{

	memSpinLock.acquire();
	char * out = (char *)map_start;
	map_start+=size+100;
	memSpinLock.release();
	return out;
	//I WILL REMOVE THIS IN THE END (FOR SOME SPECIFIC REASONS)

	/*if (size == 0)
		return nullptr;
	uint32_t pages_required = size_to_pages_count(size) + 1;
	for (uint32_t i = 0; i < max_pages; i++)
	{
			bool possible = true;

			//Finding if there are required pages in a row

			for (uint32_t j = 0; j < pages_required; j++)
			{
				if (!pages_free[i + j])
				{
					possible = false;
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
	Serial::log("\nKmemory fulll!\n");
	return nullptr;*/
}
void free(char *addr)
{
	return;

	//I WILL REMOVE THIS IN THE END (FOR SOME SPECIFIC REASONS)


	/*if ((uint32_t)addr < map_start || (uint32_t)addr > map_start + 0x400000)
		return;
	uint32_t page_number = addr_to_page_index(addr);

	char n = pages_size[page_number];
	for (char i = 0; i < n; i++)
	{
		pages_free[i + page_number] = 1;
	}
	pages_size[page_number] = 0;*/
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
uint32_t mem_used(){
	uint32_t ans = 0;
	for(uint32_t i = 0;i<max_pages;i++){
		if(pages_free[i]==0){
			ans++;
		}
	}
	ans*=mm_page_size;
	return ans;
}