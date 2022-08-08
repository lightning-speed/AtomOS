#include <stdio.h>
#include <stdlib.h>
#include "atom.h"
uint32_t t = 0;
void draw()
{
    set_screen(1, 0);
    printf("%i:%i:%i.%i\n", t / 60 / 60 / 10, t / 60 / 10, t / 10 % 60, t % 10);
    set_screen(4, 1);
}
int main(int argc, char **argv)
{
    printf("Press s for start or stop,r for rest, l for lap and e for exit\n");
    t = 0;
    char running = 0;
    for (;;)
    {
        char c = syscall(2, stdin, 0, 0);
        if (c == 'S' || c == 's')
        {
            running = !running;
            draw();
        }
        else if (c == 'R' || c == 'r')
        {
            t = 0;
            draw();
        }
        else if (c == 'L' || c == 'l')
        {
            draw();
            putchar('\n');
        }
        else if (c == 'e' || c == 'E')
        {
            exit(0);
            draw();
        }
        if (running)
        {
            sleep(100);
            t++;
            draw();
        }
    }
}