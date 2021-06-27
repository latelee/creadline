#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rl_cread_line.h"
#include "rl_command.h"

#include "my_command.h"

#include "cmd.h"

#define PROMPT          "NotAShell> "

// int abortboot(int delay)
// {
//     int abort = 0;
//     int i = 0;

//     printf("Hit any key to stop autoboot: %2d ", delay);

//     while ((delay > 0) && (!abort))
//     {
//         --delay;
//         for (i = 0; !abort && i < 100; ++i)
//         {
//             if (mytstc())
//             {
//                 abort = 1;
//                 delay = 0;
//                 mygetc();
//                 break;
//             }
//             mysleep(10);
//         }
//         printf("\b\b\b%2d ", delay);
//     }
//     myputc('\n');

//     return abort;
// }

int readline_cmd(int argc, char ** argv)
{
    static char lastcommand[CB_SIZE] = {0};
    int len;

    // 嵌入式串口终端支持，但一般发行Linux不支持，暂舍去
    // if (!abortboot(5))
    // {
    //     printf("Aotu run.\n");
    //     return 0;
    // }
    // printf("You abort.\n");

    cmd_init();
    while (1)
    {
        len = readline(PROMPT, lastcommand);
        if (len >= CB_SIZE)
        {
                printf("command line too large.\n");
                continue;
        }
        else if (len == -1)
        {
            printf("<INTERRUPT>\n");
            continue;
        }
        else
        {
            run_command(lastcommand);
        }
    }
    return 0;
}