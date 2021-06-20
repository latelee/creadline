/**
 * @file   command.c
 * @author Late Lee <latelee@163.com>
 * @date   Thu Sep 06 08:52:36 2012
 * 
 * @brief  
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "cread_line.h"
#include "common.h"

/* just wrap the functions, in case that someone need implement them */
#define cmd_puts    myputs
#define cmd_printf  myprintf
#define cmd_putc    myputc

/**************************************************************************/

cmd_tbl_t *find_cmd (const char* cmd);

int cmd_usage(cmd_tbl_t *cmdtp);

int _do_help (cmd_tbl_t *cmd_start,
              int argc, char* argv[]);

static cmd_tbl_t* cmd_table;
static int cmd_table_len = 0;

/*
 register command
*/
void register_command(cmd_tbl_t* table, int len)
{
    cmd_table = table;
    cmd_table_len = len;
}


int do_help(int argc, char* argv[])
{
    _do_help(cmd_table, argc, argv);
    return 0;
}

/*
 * Use puts() instead of printf() to avoid printf buffer overflow
 * for long help messages
 */

int _do_help (cmd_tbl_t *cmd_start,
              int argc, char* argv[])
{
    int i;
    int rcode = 0;
    cmd_tbl_t* cmdtp;

    if (argc == 1)
    {    /*show list of commands */
        //cmd_tbl_t *cmd_array[cmd_items];
        //int i, j, swaps;

        ///* Make array of commands from .uboot_cmd section */
        //cmdtp = cmd_start;
        //for (i = 0; i < cmd_items; i++)
  //      {
        //    cmd_array[i] = cmdtp++;
        //}

        ///* Sort command list (trivial bubble sort) */
        //for (i = cmd_items - 1; i > 0; --i)
  //      {
        //    swaps = 0;
        //    for (j = 0; j < i; ++j)
  //          {
        //        if (strcmp (cmd_array[j]->name,
        //                cmd_array[j + 1]->name) > 0)
  //              {
        //            cmd_tbl_t *tmp;
        //            tmp = cmd_array[j];
        //            cmd_array[j] = cmd_array[j + 1];
        //            cmd_array[j + 1] = tmp;
        //            ++swaps;
        //        }
        //    }
        //    if (!swaps)
        //        break;
        //}

        /* print short help (usage) */
        for (i = 0; i < cmd_table_len; i++)
        {
            cmdtp = cmd_start + i;
            cmd_printf("%-*s- %s\n", CONFIG_SYS_HELP_CMD_WIDTH,
                   cmdtp->name, cmdtp->usage);
        }
        return 0;
    }
    /*
     * command help (long version)
     */
    for (i = 1; i < argc; ++i) {
        if ((cmdtp = find_cmd(argv[i])) != NULL) {
            rcode |= cmd_usage(cmdtp);
        } else {
            cmd_printf ("Unknown command '%s' - try 'help'"
                " without arguments for list of all"
                " known commands\n\n", argv[i]
                    );
            rcode = 1;
        }
    }
    return rcode;
}

/***************************************************************************
 * find command table entry for a command
 */
cmd_tbl_t *find_cmd_tbl (const char *cmd, cmd_tbl_t *table, int table_len)
{
    cmd_tbl_t *cmdtp;
    cmd_tbl_t *cmdtp_temp = table;    /*Init value */
    //const char *p;
    int len;
    int n_found = 0;
    int i = 0;

    /*
     * Some commands allow length modifiers (like "cp.b");
     * compare command name only until first dot.
     */
    //len = ((p = strchr(cmd, '.')) == NULL) ? (int)strlen (cmd) : (int)(p - cmd);
    len = (int)strlen(cmd);

    // for (cmdtp = table;
    //      cmdtp != table + table_len;
    //      cmdtp++)
    for (i = 0; i < cmd_table_len; i++)    
    {
        cmdtp = table + i;
        if (strncmp (cmd, cmdtp->name, len) == 0)
        {
            if (len == strlen (cmdtp->name))
                return cmdtp;    /* full match */

            cmdtp_temp = cmdtp;    /* abbreviated command ? */
            n_found++;
        }
    }
    if (n_found == 1)
    {            /* exactly one match */
        return cmdtp_temp;
    }

    return NULL;    /* not found or ambiguous command */
}

cmd_tbl_t* find_cmd_tbl2(const char* cmd, cmd_tbl_t *table)
{
    const char *p;
    char *q;
    cmd_tbl_t *c, *found;
    int nmatches, longest;

    longest = 0;
    nmatches = 0;
    found = 0;
    for (c = table; (p = c->name) != NULL; c++)
    {
        for (q = cmd; *q == *p++; q++)
            if (*q == 0)        /* exact match? */
                return (c);
        if (!*q)
        {            /* the name was a prefix */
            if (q - cmd > longest)
            {
                longest = q - cmd;
                nmatches = 1;
                found = c;
            }
            else if (q - cmd == longest)
                nmatches++;
        }
    }
    if (nmatches > 1)
        return NULL;

    return (found);
}

// exact match version
cmd_tbl_t* find_cmd_tbl3(const char* cmd, cmd_tbl_t *table)
{
    const char *p;
    char *q;
    cmd_tbl_t *c, *found;
    int nmatches, longest;

    longest = 0;
    nmatches = 0;
    found = 0;
    for (c = table; (p = c->name) != NULL; c++)
    {
        for (q = cmd; *q == *p++; q++)
            if (*q == 0)        /* exact match? */
                return (c);
        if (!*q)
        {
            return NULL;
        }
    }
    return NULL;
}

cmd_tbl_t *find_cmd (const char* cmd)
{
    return find_cmd_tbl(cmd, cmd_table, cmd_table_len);
}

int cmd_usage(cmd_tbl_t *cmdtp)
{
    cmd_printf("%s - %s\n\n", cmdtp->name, cmdtp->usage);

#ifdef    CONFIG_SYS_LONGHELP
    cmd_printf("Usage:\n%s ", cmdtp->name);

    if (!cmdtp->help)
    {
        cmd_puts ("- No additional help available.\n");
        return 1;
    }

    cmd_puts (cmdtp->help);
    cmd_putc ('\n');
#endif    /* CONFIG_SYS_LONGHELP */
    return 1;
}

int parse_line (char *line, char *argv[])
{
    int nargs = 0;

#ifdef DEBUG_PARSER
    cmd_printf ("parse_line: \"%s\"\n", line);
#endif
    while (nargs < CONFIG_SYS_MAXARGS)
    {
        /* skip any white space */
        while ((*line == ' ') || (*line == '\t'))
        {
            ++line;
        }

        if (*line == '\0')
        {    /* end of line, no more args    */
            argv[nargs] = NULL;
#ifdef DEBUG_PARSER
            cmd_printf ("parse_line: nargs=%d\n", nargs);
#endif
            return (nargs);
        }

        argv[nargs++] = line;    /* begin of argument string    */

        /* find end of string */
        while (*line && (*line != ' ') && (*line != '\t'))
        {
            ++line;
        }

        if (*line == '\0')
        {    /* end of line, no more args    */
            argv[nargs] = NULL;
#ifdef DEBUG_PARSER
            cmd_printf ("parse_line: nargs=%d\n", nargs);
#endif
            return (nargs);
        }

        *line++ = '\0';        /* terminate current arg     */
    }

    cmd_printf ("** Too many args (max. %d) **\n", CONFIG_SYS_MAXARGS);

#ifdef DEBUG_PARSER
    cmd_printf ("parse_line: nargs=%d\n", nargs);
#endif
    return (nargs);
}

int run_command (const char *cmd)
{
    cmd_tbl_t *cmdtp;
    char cmdbuf[CB_SIZE];    /* working copy of cmd        */
    char *token;            /* start of token in cmdbuf    */
    char *sep;            /* end of token (separator) in cmdbuf */
    //char finaltoken[CB_SIZE];
    char *str = cmdbuf;
    char *argv[CONFIG_SYS_MAXARGS + 1];    /* NULL terminated    */
    int argc, inquotes;
    int repeatable = 1;
    int rc = 0;

#ifdef DEBUG_PARSER
    cmd_printf ("[RUN_COMMAND] cmd[%p]=\"", cmd);
    cmd_puts (cmd ? cmd : "NULL");    /* use puts - string may be loooong */
    cmd_puts ("\"\n");
#endif

    //clear_ctrlc();        /* forget any previous Control C */

    if (!cmd || !*cmd)
    {
        return -1;    /* empty command */
    }

    if (strlen(cmd) >= CB_SIZE)
    {
        cmd_puts ("## Command too long!\n");
        return -1;
    }

    strcpy (cmdbuf, cmd);

    /* Process separators and check for invalid
     * repeatable commands
     */

#ifdef DEBUG_PARSER
    cmd_printf ("[PROCESS_SEPARATORS] %s\n", cmd);
#endif
    while (*str) {

        /*
         * Find separator, or string end
         * Allow simple escape of ';' by writing "\;"
         */
        for (inquotes = 0, sep = str; *sep; sep++)
        {
            if ((*sep=='\'') &&
                (*(sep-1) != '\\'))
                inquotes=!inquotes;

            if (!inquotes &&
                (*sep == ';') &&    /* separator        */
                ( sep != str) &&    /* past string start    */
                (*(sep-1) != '\\'))    /* and NOT escaped    */
                break;
        }

        /*
         * Limit the token to data between separators
         */
        token = str;
        if (*sep)
        {
            str = sep + 1;    /* start of command for next pass */
            *sep = '\0';
        }
        else
            str = sep;    /* no more commands for next pass */
#ifdef DEBUG_PARSER
        cmd_printf ("token: \"%s\"\n", token);
#endif

        /* Extract arguments */
        if ((argc = parse_line (token, argv)) == 0)
        {
            rc = -1;    /* no command at all */
            continue;
        }

        /* Look up command in command table */
        if ((cmdtp = find_cmd(argv[0])) == NULL)
        {
            cmd_printf ("Unknown command '%s' - try 'help'\n", argv[0]);
            rc = -1;    /* give up after bad command */
            continue;
        }

        /* found - check max args */
        if (argc > cmdtp->maxargs)
        {
            cmd_usage(cmdtp);
            rc = -1;
            continue;
        }

        /* OK - call function to do the command */
        if ((cmdtp->cmd) (argc, argv) != 0)
        {
            rc = -1;
        }

        //repeatable &= cmdtp->repeatable;

        ///* Did the user stop this? */
        //if (had_ctrlc ())
        //    return -1;    /* if stopped then not repeatable */
    }

    //return rc ? rc : repeatable;
    return rc;
}
