#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "my_command.h"

#include "common.h"

/* just a foolish command table */
cmd_tbl_t cmd_table[] = 
{
    {"help", CONFIG_SYS_MAXARGS, do_help, "print help info."},
    {"print", 1, do_print, "print the env."},
    {"exit", 1, do_exit, "exit..."},
    {NULL, 0, NULL, NULL},
};

int do_help(int argc, char * const argv[])
{
    _do_help(cmd_table, sizeof(cmd_table)/sizeof(cmd_tbl_t), argc, argv);
    return 0;
}


int do_print(int argc, char * const argv[])
{
    myprintf("in %s\n", __FUNCTION__);
    return 0;
}

int do_exit(int argc, char * const argv[])
{
    exit(0);
    return 0;
}