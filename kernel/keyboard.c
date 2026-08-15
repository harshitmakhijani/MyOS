#include <stdint.h>
#include "ports.h"

static volatile uint16_t *video_memory = (uint16_t *)0xB8000;

/*
 * PS/2 Set 1 scan-code table.
 * Index = scan code
 * Value = ASCII character
 */
static const char keyboard_map[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',

    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0,   'a', 's',

    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',

    'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' '
};

static int cursor = 80;  // Start on second row

void keyboard_handler_c(void)
{
    uint8_t scan_code = inb(0x60);

    // Ignore key-release events for now
    if (scan_code & 0x80)
        return;

    if (scan_code >= 128)
        return;

    char c = keyboard_map[scan_code];

    if (c == 0)
        return;

    if (c == '\n')
    {
        cursor = ((cursor / 80) + 1) * 80;
        return;
    }

    if (c == '\b')
    {
        if (cursor > 80)
        {
            cursor--;

            video_memory[cursor] =
                ((uint16_t)0x07 << 8) | ' ';
        }

        return;
    }

    video_memory[cursor] =
        ((uint16_t)0x0F << 8) | c;

    cursor++;
}