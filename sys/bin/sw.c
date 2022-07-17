#include <stdio.h>
#include <stdlib.h>
#include "atom.h"

int main(int argc, char **argv)
{
    printf("Press s for start or stop,r for rest, l for lap and e for exit\n");
    uint32_t t = 0;
    char running = 0;
    for (;;)
    {
        char c = syscall(2, stdin, 0, 0);
        if (c == 'S' || c == 's')
        {
            running = !running;
        }
        else if (c == 'R' || c == 'r')
        {
            t = 0;
        }
        else if (c == 'L' || c == 'l')
        {
            putchar('\n');
        }
        else if (c == 'e' || c == 'E')
        {
            exit(0);
        }
        if (running)
        {
            sleep(100);
            t++;
            printf("%i:%i:%i.%i\n", t / 60 / 60 / 10, t / 60 / 10, t / 10, t % 10);
            set_screen(4, 1);
        }
    }
}