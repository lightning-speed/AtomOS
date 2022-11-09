int syscall(unsigned int eax,unsigned int ebx,unsigned int ecx,unsigned int edx);

int main(){
	int fnum = syscall(5,"f.exe",0,3);
	char buff[50];
	syscall(3,fnum,buff,30);
	syscall(4,0,buff,30);
	for(;;){}

}


int syscall(unsigned int eax,unsigned int ebx,unsigned int ecx,unsigned int edx){
	  asm volatile (
        "mov %0, %%eax\n\t"//Move num to eax
        "mov %1, %%ebx\n\t"//Move param1 to ebx
        "mov %2, %%ecx\n\t"//Move param2 to ecx
        "mov %3, %%edx\n\t"//Move param3 to edx
        "int $0x40"//Call interrupt. Data in eax, ebx, ecx and edx
        : //No output params
        : "g" (eax), "g" (ebx), "g" (ecx), "g" (edx)//Input params
        : "%eax", "%ebx", "%ecx", "%edx" //This handles register state pushing and popping?
    );
	asm("movl %%ecx, %0" : "=r"(ecx) :);
	  return ecx;
}