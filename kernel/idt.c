#include "idt.h"
extern void keyboard_handler(void);

static struct idt_entry idt[256];
static struct idt_ptr idtp;

void idt_set_gate(
    int n,
    uint32_t handler)
{
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init(void)
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < 256; i++)
    {
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type_attr = 0;
        idt[i].offset_high = 0;
    }

    idt_set_gate(33, (uint32_t)keyboard_handler);

    __asm__ volatile(
        "lidt %0"
        :
        : "m"(idtp));
}