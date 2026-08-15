void kernel_main(void)
{
    char *video_memory = (char *)0xB8000;

    const char *message = "Hello from MyOS!";

    for (int i = 0; message[i] != '\0'; i++)
    {
        video_memory[i * 2] = message[i];
        video_memory[i * 2 + 1] = 0x07;
    }

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}