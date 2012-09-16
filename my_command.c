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
    {"set", 3, do_set, "set env for net boot, type 'set' for more info"},
    {"boot", 1, do_boot, "boot from net"},
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
    return 0;
}

int do_set(int argc, char * const argv[])
{ 
    if (argc == 1)
    {
        myprintf("set bootfile [boot file].\n");
        myprintf("set fileformat [NAME|BLOB|ELF].\n");
        myprintf("set serverip [server ip].\n");
        myprintf("set ipaddr   [board ip].\n");
        myprintf("set gateway  [gateway].\n");
        myprintf("set netmask  [netmask].\n");
        myprintf("set ethaddr  [mac addr].\n");
        myprintf("set dhcp     [0|1].\n");
    }
    return 0;
}

int do_boot(int argc, char * const argv[])
{
    return 0;
}

int do_exit(int argc, char * const argv[])
{
    exit(0);
}