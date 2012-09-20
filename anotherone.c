#include <stddef.h>
#include <string.h>
#include "command.h"
#include "common.h"
#include "cread_line.h"

#undef cmd_puts
#undef cmd_printf
#undef cmd_putc

/* just wrap the functions, in case that someone need implement them */
#define cmd_puts    myputs
#define cmd_printf  myprintf
#define cmd_putc    myputc

#define HELPINDENT 8

struct cmd_s {
    const char *name;
    const char *help;
    void	(*handler) (int argc, char* argv[]);
};

typedef struct cmd_s cmd_t;

static void do_help(int argc, char* argv[]);
static void do_foo(int argc, char* argv[]);
static void do_exit(int argc, char* argv[]);

cmd_t cmdtab[] = {
    {
        "foo",
         "foo test",
         do_foo,
    },
    {
        "exit",
        "exit program",
        do_exit,
    },
    { "help",
    "print help information",
    do_help },
    { 0, 0, 0 }
};

cmd_t *getcmd(char *name)
{
    const char *p;
    char *q;
    cmd_t *c, *found;
    int nmatches, longest;

    longest = 0;
    nmatches = 0;
    found = 0;
    for (c = cmdtab; (p = c->name) != NULL; c++) {
        for (q = name; *q == *p++; q++)
            if (*q == 0)		/* exact match? */
                return (c);
        if (!*q) {
            return ((cmd_t *)-1);
        }
    }

    return ((cmd_t *)-1);
}

static int parse_line(char *line, char *argv[])
{
    int nargs = 0;
    char *cp;

    for (cp = line; *cp;) {
        while ((*cp == ' ') || (*cp == '\t'))
            cp++;
        if (*cp == '\0')
        {
            argv[nargs] = NULL;
            return nargs;
        }
        argv[nargs++] = cp;

       while ((*cp != '\0') && (*cp != ' ') && (*cp != '\t'))
            cp++;
        if (*cp == '\0')
        {
            argv[nargs] = NULL;
            return nargs;
        }
        *cp++ = '\0';
    }

    return nargs;
}

int run_command1(const char *cmd)
{
    int argc;
    char *argv[CONFIG_SYS_MAXARGS + 1];	/* NULL terminated	*/
    char cmdbuf[CB_SIZE];	/* working copy of cmd		*/
    cmd_t *c;

    if (!cmd || !*cmd)
    {
        return -1;	/* empty command */
    }

    if (strlen(cmd) >= CB_SIZE)
    {
        cmd_puts ("## Command too long!\n");
        return -1;
    }

    strcpy(cmdbuf, cmd);

    /* Extract arguments */
    if ((argc = parse_line(cmdbuf, argv)) == 0)
    {
        return -1;	/* no command at all */
    }

    /* Look up command in command table */
    c = getcmd(argv[0]);
    if (c == (cmd_t *)-1)
    {
        cmd_printf("Unknown command '%s' - try 'help'\n", argv[0]);
        return -1;	/* give up after bad command */
    }

    /* OK - call function to do the command */
    (c->handler)(argc, argv);

    return 0;
}

static void do_help(int argc, char* argv[])
{
    cmd_t *c;

    if (argc == 1) {
        for (c = cmdtab; c->name; c++)
            cmd_printf("%-*s\t%s\n", HELPINDENT, c->name, c->help);
        return;
    }
    while (--argc > 0) {
        char *arg;
        arg = *++argv;
        c = getcmd(arg);
        if (c == (cmd_t *)-1)
            cmd_printf("No help command %s\n", arg);
        else
            cmd_printf("%s\n", c->help);
    }
}

static void do_foo(int argc, char* argv[])
{
    int i = 0;
    cmd_printf("in foo.\n");
    for(i = 0; i < argc; i++)
    {
        cmd_printf("argv[%d] = %s\n", i, argv[i]);
    }
}

static void do_exit(int argc, char* argv[])
{
    exit(0);
}

