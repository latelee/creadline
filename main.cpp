#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#endif

// #include "rl_common.h"

#include "cmd.h"
#include "cmdall.h"

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

// // 检测按键
// void test3()
// {
//     printf("test3:\n");
//     while (1)
//     {
//         if (mytstc())
//         {
//             printf("you hit a key.\n");
//             break;
//         }
//     }
// }

int main(int argc, char* argv[])
{
    //test();
    //test2();
    //test3();
    
    char* p;
	char* cmdname = *argv;
    if ((p = strrchr (cmdname, '/')) != NULL)
    {
		cmdname = p + 1;
	}

	if (strcmp(cmdname, "a.out") == 0)
    {
		if (readline_cmd(argc, argv) != 0)
			return -1;

		return 0;
	}
    else if (strcmp(cmdname, "a_all.out") == 0)
    {
		if (readline_cmd_allone(argc, argv) != 0)
			return -1;

		return 0;
	}


    return 0;
}

