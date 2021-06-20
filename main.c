#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "cread_line.h"
#include "command.h"

#include "my_command.h"

#ifdef WIN32
#define mysleep(x) Sleep(x)
#else
#define mysleep(x) usleep(x*1000)
#endif

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

int readline_test(void)
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

// 回退键测试
// 注：必须回退到适应位置
void test()
{
    int cnt = 0;
    printf("time:%2d%% ", cnt);
    while(1)
    {
        printf("\b\b\b\b%2d%% ", cnt++);
        if (cnt == 100)
            break;
        mysleep(1000);
    }
}

// IP地址转换测试
void test2()
{
    char *end;
    int i;
    char* addr = "40:5f:c2:a6:18:4b";
    char* ip = "172.18.10.156";
    unsigned char enetaddr[16];

    for (i = 0; i < 6; ++i) {
        enetaddr[i] = addr ? strtoul(addr, &end, 16) : 0;
        if (addr)
            addr = (*end) ? end + 1 : end;
    }
    enetaddr[i] = '\0';

    for (i = 0; i < 6; ++i)
    {
        printf("%x ", enetaddr[i]);
    }

    for (i = 0; i < 4; ++i) {
        enetaddr[i] = ip ? strtoul(ip, &end, 10) : 0;
        if (ip)
            ip = (*end) ? end + 1 : end;
    }
    enetaddr[i] = '\0';
    for (i = 0; i < 4; ++i)
    {
        printf("%d ", enetaddr[i]);
    }

    printf("\n");
}

// 检测按键
void test3()
{
    printf("test3:\n");
    while (1)
    {
        if (mytstc())
        {
            printf("you hit a key.\n");
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    //test();
    //test2();
    //test3();
    readline_test();

    return 0;
}

