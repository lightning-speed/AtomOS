#pragma once

#include <stdint.h>
#include <CGA.h>

namespace SysInterface{
	uint32_t syscall_open(char * name,int flags,short mode);
	int syscall_close(uint32_t fdnum);
	size_t syscall_read(uint32_t fd,char * buff,size_t count);
	size_t syscall_write(uint32_t fd,const char * buff,size_t count);
};