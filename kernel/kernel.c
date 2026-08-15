#include "gdt.h"
#include "idt.h"
#include "pic.h"

void kernel_main(void)
{
    char *video_memory = (char *)0xB8000;

    const char *message = "MyOS v0.2";

    for (int i = 0; message[i] != '\0'; i++)
    {
        video_memory[i * 2] = message[i];
        video_memory[i * 2 + 1] = 0x07;
    }

    gdt_init();
    pic_remap();
    idt_init();

    __asm__ volatile("sti");

    while (1)
    {
        __asm__ volatile("hlt");
    }
}