#include <Memory.h>
#include <String.h>
#include <CGA.h>
#include <GDT.h>
#include <IDT.h>
#include <PIC.h>
#include <Keyboard.h>
#include <PIT.h>
#include <Scheduler.h>
#include <FS.h>
#include <Ramdisk.h>
#include <Serial.h>
#include <VFS.h>
#include <ACPI.h>
#include <multiboot.h>
#include <FB.h>
#include <BitmapImage.h>
#include <Syscall.h>
#include <SysInterface.h>
#include <RMm.h>
#include <ELF.h>
#include <Runtime.h>

void *__stack_chk_fail_local;

void randomize_mem(uint32_t from, uint32_t till);
void kernel_stage2();
process_t *kernel_stage2proc;
void meow(){

	for(int i  = 0;;i++){
		Scheduler::sleep(100);
		CGA::print("shit");
	}
}
extern "C" int kmain(uint32_t mb_sig, uint32_t mb_addr)
{
	inInterrupt = true;
	RMm::init();
	CGA::clearScreen();
	mm_init();
	gdt_install();
	idt_install();
	PIC::init();
	Serial::setup();
	Serial::verboseOn = true;

	//first thread is continued to where eip was
	thread_t *kernel_stage1 = Scheduler::createThread(nullptr,nullptr, nullptr);



	Scheduler::start();

	Keyboard::init();
	Syscall::init();
	install_interrupt_gates();

	if (mb_sig == 0x2BADB002 && mb_addr < 0x100000)

	{
		//after checking the signature and address
		Serial::log("Multiboot Verified\n");
		struct multiboot_info *mbinfo = (multiboot_info *)mb_addr;
		multiboot_module_t *mod = (multiboot_module_t *)mbinfo->mods_addr;
		Ramdisk::start = (char *)mod->mod_start;
		Serial::log((string) "Ramdisk loaded at:" + (int)Ramdisk::start + "\n");
		Ramdisk::size = mod->mod_start - (uint32_t)Ramdisk::start;
	}
	kernel_stage2proc = ProcessManager::create("Kernel_Proc",0,nullptr,nullptr,(void *)kernel_stage2);
	ProcessManager::start(kernel_stage2proc);
	enableInt();

	Scheduler::selfKill();
	return 0;
}

void kernel_stage2()
{
	ACPI::init();
	CGA::printf((char *)"ACPI [Done]\n", 0x0d);
	VFS::init();
	//Initfs will be mounted in the ramdisk init
	Ramdisk::init();
	CGA::printf((char *)"Filesystem Mounted [Done]\n", 0x09);
	Runtime::exec("f.exe",0,nullptr,nullptr);
	for(;;){
		
	}

}

void randomize_mem(uint32_t from, uint32_t till)
{

	for (uint32_t i = from; i < till; i++)
	{
		if (i != 0)
			((char *)0x0)[i] = i;
		else
		{
			((char *)0x0)[i] = 1;
		}
	}
}