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
	Serial::setup();
	Serial::verboseOn = true;
	//first thread is continued to where eip
	thread_t *kernel_stage1 = Scheduler::create(nullptr, nullptr);
	Scheduler::start();
	Keyboard::init();
	Syscall::init();
	Mouse::init();
	install_interrupt_gates();

	if (mb_sig == 0x2BADB002 && mb_addr < 0x100000)

	{
		//after checking the signature and address
		Serial::log("Multiboot Verified\n");
		struct multiboot_info *mbinfo = (multiboot_info *)mb_addr;
		multiboot_module_t *mod = (multiboot_module_t *)mbinfo->mods_addr;
		Ramdisk::start = (char *)mod->mod_start;
		Ramdisk::size = mod->mod_start - (uint32_t)Ramdisk::start;
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
	/*fnode *icon = VFS::open("icon", "r");
	uint8_t *c = (uint8_t *)icon->content;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			uint32_t p;
			char *rp = (char *)&p;
			rp[0] = c[3];
			rp[1] = c[1];
			rp[2] = c[2];
			rp[3] = c[0];
			FB::setPixel(i, j, p);
			c += 4;
		}
	}
	for (;;)
	{
	}*/
	Runtime::exec("bin/cmd.exe", 0, nullptr, nullptr);
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