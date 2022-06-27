#include <Memory.h>
#include <String.h>
#include <CGA.h>
#include <GDT.h>
#include <IDT.h>
#include <IO.h>
#include <PIC.h>
#include <Keyboard.h>
#include <PIT.h>
#include <Scheduler.h>
#include <FS.h>
#include <Ramdisk.h>
#include <Syscall.h>
#include <Runtime.h>
#include <CMOS.h>
#include <Serial.h>
#include <Sound.h>
#include <VFS.h>
#include <ACPI.h>
#include <multiboot.h>
#include <FB.h>
#include <Mouse.h>
#include "../Glib/Window.h"

void randomize_mem(uint32_t from, uint32_t till);
void kernel_stage2();
process_t *kernel_stage2proc;
extern "C" int kmain(uint32_t mb_sig, uint32_t mb_addr)
{
	//As Malloc is required everywhere we initalize it first
	mm_init();

	gdt_install();
	idt_install();
	PIC::init();
	CGA::print("PIC [Done]\n", 0x0e);
	Serial::setup();
	CGA::print("Serial [Done]\n", 0x0d);
	Serial::verboseOn = true;
	//first thread is continued to where eip
	thread_t *kernel_stage1 = Scheduler::create(nullptr, nullptr);
	Scheduler::start();
	CGA::print("Scheduler [Done]\n", 0x0c);
	Keyboard::init();
	CGA::print("Keyboard [Done]\n", 0x0b);
	Syscall::init();
	CGA::print("Syscalls [Done]\n", 0x0a);
	Mouse::init();
	install_interrupt_gates();
	if (mb_sig == 0x2BADB002 && mb_addr < 0x100000)

	{
		//after checking the signature and address
		Serial::log("Multiboot Verified\n");
		struct multiboot_info *mbinfo = (multiboot_info *)mb_addr;
		multiboot_module_t *mod = (multiboot_module_t *)mbinfo->mods_addr;
		Ramdisk::start = (char *)mod->mod_start;
		FB::init((char *)(mbinfo->framebuffer_addr), mbinfo->framebuffer_width, mbinfo->framebuffer_height, mbinfo->framebuffer_pitch);
		CGA::init();
	}

	kernel_stage2proc = Scheduler::createProcess("kernel_stage2", nullptr, (void *)kernel_stage2);
	enableInt();
	//Kills this thread as no longer nedded
	Scheduler::killThread(kernel_stage1);
	while (1)
		;
}

void kernel_stage2()
{
	ACPI::init();
	CGA::print("ACPI [Done]\n", 0x0d);
	VFS::init();
	//Initfs will be mounted in the ramdisk init
	Ramdisk::init();
	CGA::print("Filesystem Mounted [Done]\n", 0x09);
	fnode *fontFile = VFS::open("unifont.bin", "r");
	FB::loadFont(fontFile);
	VFS::close(fontFile);
	CGA::clearScreen();
	int index;
	/*for (int i = 1; i < 255; i += 1)
	{
		index = 0;
		for (int j = 0; j < 400; j++)
		{
			for (int r = 0; r < 400; r++)
			{
				((uint32_t *)w.buffer)[(j * FB::width) + r] = 0xffffffff / i;
			}
		}
		w.repaint(0, 0, 400, 400);
	}
	for (;;)
	{
	}*/
	Runtime::exec("cmd.exe", 0, nullptr, nullptr);
	for (;;)
	{
		int ans = 0;
		for (int i = 0; i < max_pages; i++)
		{
			ans += is_page_free(i);
		}
		for (int i = 0; i < 300; i++)
			asm("hlt");
		Serial::log(ans);
	}
	Scheduler::killProcess(kernel_stage2proc);
	while (1)
	{
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