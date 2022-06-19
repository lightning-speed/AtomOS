#include <stdint.h>

#pragma once

#define max_pages 10000
#define mm_page_size 512

extern uint32_t m_start;
extern uint32_t m_end;

bool is_page_free(uint32_t index);
char *malloc(uint32_t size);
void free(char *addr);
void mm_init();
extern "C" void memcpy(char *to, char *from, uint32_t size_to_move);
extern "C" void memset(char *p, char c, uint32_t size);
char *realloc(char *addr, uint32_t size);
int memcmp(char *a, char *b, int len);