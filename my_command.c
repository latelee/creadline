#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "my_command.h"

#include "common.h"

/* just a foolish command table */
cmd_tbl_t cmd_table[] = 
{
    {"help", CONFIG_SYS_MAXARGS, do_help, "print help info."},
    {"print", 2, do_print, "print the env."},
    {"exit", 1, do_exit, "exit..."},
    {NULL, 0, NULL, NULL},
};

int do_help(int argc, char* argv[])
{
    _do_help(cmd_table, argc, argv);
    return 0;
}


int do_print(int argc, char* argv[])
{

    myprintf("in %s argc: %d %s %s\n", __FUNCTION__, argc, argv[0], argv[1]);
    return 0;
}

int do_exit(int argc, char* argv[])
{
    exit(0);
    return 0;
}