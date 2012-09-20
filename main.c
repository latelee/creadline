#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif
#include "cread_line.h"
#include "command.h"

#define mysleep Sleep

int abortboot(int delay)
{
    int abort = 0;
    int i = 0;

    myprintf("Hit SPACE key to stop autoboot: %2d ", delay);

    while ((delay > 0) && (!abort))
    {
        --delay;
        for (i = 0; !abort && i < 100; ++i)
        {
            if (mytstc())
            {
                abort = 1;
                delay = 0;
                mygetc();
                break;
            }
            mysleep(10);
        }
        myprintf("\b\b\b%2d ", delay);
    }
    myputc('\n');

    return abort;
}

int readline_test(void)
{
    static char lastcommand[CB_SIZE] = {0};
    int len;

    //if (!abortboot(5))
    //{
    //    myprintf("Aotu run.\n");
    //    return 0;
    //}
    //myprintf("You abort.\n");

    while (1)
    {
        len = readline(PROMPT, lastcommand);
        if (len > 0)
        {
            //printf("len: %d\n", len);
            if (len >= CB_SIZE)
            {
                myprintf("command line too large.\n");
                break;
            }
            //strcpy(lastcommand, console_buffer);
            //printf("[echo]: %s\n", lastcommand);
        }

        else if (len == 0)
        {
            //printf("nothing input.\n");
            // do nothing
        }

        if (len == -1)
        {
            myputs("<INTERRUPT>\n");
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
        Sleep(1000);
    }

}

void test2()
{
    char *end;
    int i;
    char* addr = "40:5f:c2:a6:18:4b";
    char* ip = "172.18.10.156";
    unsigned char enetaddr[16];

    for (i = 0; i < 6; ++i) {
        enetaddr[i] = addr ? simple_strtoul(addr, &end, 16) : 0;
        if (addr)
            addr = (*end) ? end + 1 : end;
    }
    enetaddr[i] = '\0';

    for (i = 0; i < 6; ++i)
    {
        printf("%x ", enetaddr[i]);
    }

    for (i = 0; i < 4; ++i) {
        enetaddr[i] = ip ? simple_strtoul(ip, &end, 10) : 0;
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

int main(int argc, char* argv[])
{
    test2();
    initIBL();
    readline_test();
    //myputs("hello world.\r");
    //myprintf("hello\n");
    return 0;
}

