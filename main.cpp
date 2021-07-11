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

// 定义入口函数
typedef int (*main_cmd_func)(int argc, char* argv[]);
typedef struct main_cmd_tbl_tt {
    const char    *name;
    main_cmd_func cmd;
} main_cmd_tbl_t;

static main_cmd_tbl_t my_cmd_table[] = 
{
    {"a.out", readline_cmd},
    {"a_all.out", readline_cmd_allone},
};

// 查找入口函数，如果找不到，默认使用第1项，即始终会运行程序
main_cmd_tbl_t* find_table(const char* cmdname)
{
    int i = 0;
    int len = sizeof(my_cmd_table) / sizeof(my_cmd_table[0]);
    for (i = 0; i < len; i++)
    {
        if (!strcmp(my_cmd_table[i].name, cmdname))
        {
            break;
        }
    }
    if (i >= len) i = 0;
    return &my_cmd_table[i];
}

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

    main_cmd_tbl_t* tbl = find_table(cmdname);
    return tbl->cmd(argc, argv);
}

