#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rl_command.h"
#include "my_command.h"

/* 定义命令列表 */
cmd_tbl_t my_cmd_table[] = 
{
    // do_help_default为默认函数，可重新实现
    {"help", CONFIG_SYS_MAXARGS, do_help_default, "print help info."},
    {"print", 2, do_print, "print the env."},
    {"exit", 1, do_exit, "exit..."},
    {"quit", 1, do_exit, "exit..."},
};

// 初始化，注册命令
void cmd_init()
{
    int len = sizeof(my_cmd_table) / sizeof(my_cmd_table[0]);
    register_command(my_cmd_table, len);
}

cmd_tbl_t* find_table(const char* cmdname)
{
    int i = 0;
    int len = sizeof(my_cmd_table) / sizeof(my_cmd_table[0]);
    for (i = 0; i < len; i++)
    {
        if (!strcmp(my_cmd_table[i].name, cmdname))
        {
            return &my_cmd_table[i];
        }
    }
    return NULL;
}

int do_print(int argc, char* argv[])
{
    int i = 0;
    for (i = 0; i < argc; i++)
    printf("in %s argc: %d/%d %s\n", __FUNCTION__, i, argc, argv[i]);
    return 0;
}

int do_exit(int argc, char* argv[])
{
    exit(0);
    return 0;
}