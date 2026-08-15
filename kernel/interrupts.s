BITS 32

section .text

global keyboard_handler
extern keyboard_handler_c

keyboard_handler:
    pusha

    call keyboard_handler_c

    popa

    mov al, 0x20
    out 0x20, al

    iretd