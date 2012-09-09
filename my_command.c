#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "my_command.h"

/* just a foolish command table */
cmd_tbl_t cmd_table[] = 
{
    {"help", 10, do_help, "help info"},
    {"foo", 10, do_foo, "foo info"},
    {"foobar", 10, do_bar, "bar info"},
    {"test", 10, do_test, "test info"},
    {"exit", 1, do_exit, "exit the program"},
    {NULL, 0, NULL, NULL},
};

int do_help(int argc, char * const argv[])
{
    _do_help(cmd_table, sizeof(cmd_table)/sizeof(cmd_tbl_t), argc, argv);
    return 0;
}

int do_foo(int argc, char * const argv[])
{
    int i = 0;

    printf("do_foo argc: %d\n", argc);
    for (i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    return 0;
}

int do_bar(int argc, char * const argv[])
{
    int i = 0;

    printf("do_bar argc: %d\n", argc);
    for (i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    return 0;
}

int do_test(int argc, char * const argv[])
{
    int i = 0;

    printf("do_test argc: %d\n", argc);
    for (i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    return 0;
}

int do_exit(int argc, char * const argv[])
{
    exit(0);
}