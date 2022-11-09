#include <SysInterface.h>
#include <VFS.h>

namespace SysInterface{
	uint32_t syscall_open(char * name,int flags,short mode){
		fnode * node;
		switch(mode){
			case 1:
			 node = VFS::open(name,"r");
			 break;
			case 2:
			 node = VFS::open(name,"w");
			 break;
			case 3:
			 node = VFS::open(name,"wr");
			 break;
			default:
			 node = nullptr;
			 break;    
		}
		return (uint32_t)VFS::OpenedFiles.indexOf(node);
	}
	int syscall_close(uint32_t fdnum){
		fnode * node = VFS::fromFNUM(fdnum);
		VFS::close(node);
		return 1;
	}
 	size_t syscall_read(uint32_t fdnum,char * buff,size_t count){
 		fnode * node = VFS::fromFNUM(fdnum);
 		VFS::read(buff,count,node);
 		return count;
	}
	size_t syscall_write(uint32_t fdnum,const char * buff,size_t count){
		size_t ret = 0;
		if(fdnum==0){
			for(size_t i = 0;i<count;i++)
				CGA::printChar(buff[i]);
				ret = count;
		}
		else{

		}
		return ret;

	}	
};